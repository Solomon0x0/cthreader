#include "CThreader/ThreadPool.hpp"
#include <algorithm>

namespace CT {

    ThreadPool::ThreadPool() noexcept {}

    void ThreadPool::Stop(const CThreaderStopFlag _flag) noexcept {
        switch (_flag) {
        case CThreaderStopFlag::CLOSE_AFTER_COMPLETING_PROCESSED_TASKS:
        {
            m_cv.notify_all();

            for (auto& t : m_threads) {
                t.request_stop();
            }

            break;
        }
        case CThreaderStopFlag::CLOSE_AFTER_COMPLETING_THE_TASKS:
        default:
        {
            std::unique_lock<std::mutex> lk(m_sleepMx);
            m_cv.notify_all();
            lk.unlock();

            auto queues_empty = [this]() noexcept {
                return m_qHigh.empty() && m_qMedium.empty() && m_qLow.empty();
            };

            while (!queues_empty()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            for (auto& t : m_threads) {
                t.request_stop();
            }
            m_cv.notify_all();

            break;
        }
        }
    }

    void ThreadPool::Start() noexcept {
        if (!m_threads.empty())
            return;

        m_threads.reserve(m_threadCount);
        for (size_t i = 0; i < m_threadCount; ++i) {
            m_threads.emplace_back([this](std::stop_token st) {
                WorkerLoop(st);
            });
        }
    }

    void ThreadPool::Kill(const CThreaderStopFlag _flag) noexcept {
        Stop(_flag);

        std::vector<std::jthread> to_join;
        to_join.swap(m_threads);
    }

    void ThreadPool::ClearTasks() noexcept {
        Task tmp;
        while (m_qHigh.try_pop(tmp)) { }
        while (m_qMedium.try_pop(tmp)) { }
        while (m_qLow.try_pop(tmp)) { }
    }

    void ThreadPool::Initialize(size_t _threadCount) noexcept {
        m_threadCount = std::max<size_t>(_threadCount, 1);
        
        m_qHigh.reserve(256);
        m_qMedium.reserve(512);
        m_qLow.reserve(1024);

        m_results.resize(1024);
        m_resultsSize.store(1024, std::memory_order_relaxed);
    }

    void ThreadPool::EnsureResultCapacity(uint64_t id) {
        uint64_t cur = m_resultsSize.load(std::memory_order_acquire);
        if (id < cur) {
            return;
        }

        const auto s = ShardOf(id);
        std::scoped_lock grow(m_resultsGrowMx);

        std::lock_guard<SpinLock> g(m_resultShards[s].lock);
        cur = m_resultsSize.load(std::memory_order_relaxed);
        if (id < cur) {
            return;
        }

        size_t newCap = std::max<size_t>(cur * 2, static_cast<size_t>(id + 1));
        m_results.resize(newCap);
        m_resultsSize.store(newCap, std::memory_order_release);
    }

    void ThreadPool::PushTask(Task&& _task, TaskLevel _taskLevel) noexcept {
        const uint64_t id = _task.GetTaskId();
        EnsureResultCapacity(id);

        switch (_taskLevel) {
        case TaskLevel::High: {
            m_qHigh.push(std::move(_task));
            break;
        }
        case TaskLevel::Medium: {
            m_qMedium.push(std::move(_task));
            break;
        }
        default:                 {
            m_qLow.push(std::move(_task));
            break;
        }
        }

        m_cv.notify_one();
    }

    void ThreadPool::WorkerLoop(std::stop_token st) {
        while (!st.stop_requested()) {
            Task t;
            bool got = false;

            if (!got) got = m_qHigh.try_pop(t);
            if (!got) got = m_qMedium.try_pop(t);
            if (!got) got = m_qLow.try_pop(t);

            if (!got) {
                std::unique_lock lk(m_sleepMx);
                m_cv.wait(lk, [&] {
                    return st.stop_requested()
                        || !m_qHigh.empty()
                        || !m_qMedium.empty()
                        || !m_qLow.empty();
                });
                continue;
            }

            try {
                std::any r = t.Execute();

                const uint64_t id = t.GetTaskId();
                const size_t shard = ShardOf(id);
                {
                    std::lock_guard<SpinLock> g(m_resultShards[shard].lock);
                    if (id >= m_resultsSize.load(std::memory_order_acquire)) {
                        EnsureResultCapacity(id);
                    }
                    m_results[id].SetValue(std::move(r));
                }
            }
            catch (...) {
				std::print("Task ID {} execution threw an exception.\n", t.GetTaskId());
            }
        }
    }

    std::expected<TaskResult, CThreaderError> ThreadPool::GetResult(uint64_t _taskId) noexcept {
        if (_taskId >= m_resultsSize.load(std::memory_order_acquire)) {
            return std::unexpected(CThreaderError::TaskNotFound);
        }

        const size_t shard = ShardOf(_taskId);
        std::lock_guard<SpinLock> g(m_resultShards[shard].lock);
        const TaskResult& slot = m_results[_taskId];
        if (!slot.HasValue()) {
            return std::unexpected(CThreaderError::TaskNotFound);
        }

        return slot;
    }
}

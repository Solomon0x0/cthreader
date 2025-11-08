#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <optional>
#include <any>
#include <expected>
#include <atomic>
#include <condition_variable>
#include <array>
#include <print>

#include "Task.hpp"
#include "TaskResult.hpp"
#include "Utils.hpp"
#include "CpuRelax.hpp"

namespace CT {
    struct alignas(64) SpinLock {
        std::atomic_flag flag = ATOMIC_FLAG_INIT;

        void lock() noexcept {
            uint32_t spins = 1;
            while (flag.test_and_set(std::memory_order_acquire)) {
                for (uint32_t i = 0; i < spins; ++i) {
                    CpuRelax();
                }

                spins = (spins < (1u << 12)) ? (spins << 1) : (1u << 12);

                if (spins == (1u << 12)) {
                    std::this_thread::yield();
                }
            }
        }
        void unlock() noexcept {
            flag.clear(std::memory_order_release);
        }
    };

    template<typename T>
    class MPMCQueueLite {
    public:
        void push(T&& v) {
            std::lock_guard<SpinLock> g(m_lock);
            m_q.emplace_back(std::move(v));
        }

        bool try_pop(T& out) {
            std::lock_guard<SpinLock> g(m_lock);
            if (m_q.empty()) {
                return false;
            }

            out = std::move(m_q.front());
            m_q.erase(m_q.begin());
            return true;
        }

        bool empty() const {
            std::lock_guard<SpinLock> g(m_lock);
            return m_q.empty();
        }

        void reserve(size_t n) {
            m_q.reserve(n);
        }

    private:
        mutable SpinLock m_lock;
        std::vector<T> m_q;
    };

    class ThreadPool {
    public:
        ThreadPool() noexcept;
        ~ThreadPool() noexcept { Kill(CThreaderStopFlag::CLOSE_AFTER_COMPLETING_PROCESSED_TASKS); };

        void Initialize(size_t _threadCount) noexcept;
        void PushTask(Task&& _task, TaskLevel _taskLevel) noexcept;
        std::expected<TaskResult, CThreaderError> GetResult(uint64_t _taskId) noexcept;
        void Stop(const CThreaderStopFlag _flag) noexcept;
        void Start() noexcept;
        void Kill(const CThreaderStopFlag _flag) noexcept;
        void ClearTasks() noexcept;
    private:
        void WorkerLoop(std::stop_token _st);

        size_t m_threadCount{ 0 };

        MPMCQueueLite<Task> m_qHigh;
        MPMCQueueLite<Task> m_qMedium;
        MPMCQueueLite<Task> m_qLow;

        std::mutex m_sleepMx;
        std::condition_variable m_cv;

        static constexpr size_t kShardCount = 64;
        struct Shard {
            SpinLock lock;
        };
        std::array<Shard, kShardCount> m_resultShards;

        std::mutex m_resultsGrowMx;
        std::vector<TaskResult> m_results;
        std::atomic<uint64_t> m_resultsSize{ 0 };

        std::vector<std::jthread> m_threads;

        static constexpr size_t ShardOf(uint64_t id) noexcept {
            return (id * 11400714819323198485ull) & (kShardCount - 1); // Fibonacci
        }

        void EnsureResultCapacity(uint64_t id);
    };
}

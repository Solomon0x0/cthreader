#include "CThreader/CThreader.hpp"

namespace CT {
    CThreader::CThreader() noexcept : m_taskIdCounter(0) {}

	CThreader::~CThreader() noexcept { Stop(CThreaderStopFlag::CLOSE_AFTER_COMPLETING_PROCESSED_TASKS); }

    void CThreader::Stop(const CThreaderStopFlag _flag) noexcept {
		m_threadPool.Stop(_flag);
    }

    void CThreader::Start() noexcept {
		m_threadPool.Start();
    }

    void CThreader::Kill(const CThreaderStopFlag _flag) noexcept {
		m_threadPool.Kill(_flag);
    }

    void CThreader::ClearTasks() noexcept {
		m_threadPool.ClearTasks();
    }

    std::expected<void, CThreaderError> CThreader::Initialize(std::optional<std::size_t> _threadCount) noexcept {
        const std::size_t threadCount = _threadCount.value_or(std::thread::hardware_concurrency());
        
        if (threadCount == 0) {
            return std::unexpected(CThreaderError::CThreaderNotInitialized);
        }

        m_threadPool.Initialize(threadCount);
        return {};
    }

    uint64_t CThreader::Enqueue(Task&& _task, TaskLevel _taskLevel) noexcept {
        const uint64_t taskId = m_taskIdCounter.fetch_add(1, std::memory_order_relaxed) + 1;
        _task.SetTaskId(taskId);
        m_threadPool.PushTask(std::move(_task), _taskLevel);
        return taskId;
    }

    std::expected<TaskResult, CThreaderError> CThreader::GetResult(const uint64_t& _taskId) noexcept {
        return m_threadPool.GetResult(_taskId);
    }
}

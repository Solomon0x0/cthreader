#pragma once
#include <optional>
#include <thread>
#include <functional>
#include <any>
#include <expected>
#include <atomic>

#include "ThreadPool.hpp"
#include "Task.hpp"
#include "Utils.hpp"

namespace CT {
    class CThreader {
    public:
        CThreader() noexcept;
        ~CThreader() noexcept;

        std::expected<void, CThreaderError> Initialize(std::optional<std::size_t> _threadCount = std::nullopt) noexcept;
        uint64_t Enqueue(Task&& _task, TaskLevel _taskLevel = TaskLevel::Low) noexcept;
        [[nodiscard]] std::expected<TaskResult, CThreaderError> GetResult(const uint64_t& _taskId) noexcept;
		void Stop(const CThreaderStopFlag _flag = CThreaderStopFlag::CLOSE_AFTER_COMPLETING_THE_TASKS) noexcept;
        void Start() noexcept;
        void Kill(const CThreaderStopFlag _flag = CThreaderStopFlag::CLOSE_AFTER_COMPLETING_PROCESSED_TASKS) noexcept;
		void ClearTasks() noexcept;

    private:
        ThreadPool m_threadPool;
        alignas(64) std::atomic<uint64_t> m_taskIdCounter;
    };
}

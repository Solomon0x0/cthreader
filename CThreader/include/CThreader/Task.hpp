#pragma once
#include <functional>
#include <optional>
#include <string>
#include <any>
#include <tuple>
#include <type_traits>
#include <utility>

namespace CT {
    enum class TaskLevel : uint64_t { Low = 0, Medium = 1, High = 2 };

    class Task {
    public:
        template<typename Callable, typename... Args>
        Task(Callable&& func, Args&&... args) noexcept;

        Task() noexcept = default;

        Task(Task&&) noexcept = default;
        Task& operator=(Task&&) noexcept = default;
        Task(const Task&) = delete;
        Task& operator=(const Task&) = delete;

        std::any Execute() const;
        void SetTaskId(uint64_t _taskId) noexcept;
        uint64_t GetTaskId() const noexcept;

    private:
        std::function<std::any()> m_task;
        uint64_t m_taskId{ 0 };
    };
}

#include "Task.ipp"

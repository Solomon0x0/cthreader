#pragma once
#include <optional>
#include <any>
#include <stdexcept>

namespace CT {
    class TaskResult {
    public:
        TaskResult(std::optional<std::any>&& v) noexcept;
        TaskResult() noexcept = default;
        ~TaskResult() = default;

        bool HasValue() const noexcept;
        void SetValue(std::any&& v) noexcept;
        std::any GetValue() const;
        const std::any& GetValueRef() const noexcept;

        explicit operator bool() const noexcept;

    private:
        std::optional<std::any> m_value;
    };
}

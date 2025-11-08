#include "CThreader/TaskResult.hpp"

namespace CT {
	TaskResult::TaskResult(std::optional<std::any>&& _value) noexcept
		: m_value(std::move(_value)) { }

	bool TaskResult::HasValue() const noexcept {
		return m_value.has_value();
	}

	void TaskResult::SetValue(std::any&& _value) noexcept {
		m_value = std::move(_value);
	}

	std::any TaskResult::GetValue() const {
		if (m_value)
			return m_value.value();

		throw std::runtime_error("No value present in TaskResult.");
	}

	const std::any& TaskResult::GetValueRef() const noexcept {
		static const std::any empty;
		return m_value ? *m_value : empty;
	}

	TaskResult::operator bool() const noexcept {
		return HasValue();
	}
}
#include "CThreader/Task.hpp"

namespace CT {
	std::any Task::Execute() const {
		if (m_task)
			return m_task();

		return std::any{};
	}

	void Task::SetTaskId(uint64_t _taskId) noexcept {
		m_taskId = _taskId;
	}

	std::uint64_t Task::GetTaskId() const noexcept {
		return m_taskId;
	}
} 

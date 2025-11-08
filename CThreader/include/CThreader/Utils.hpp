#pragma once

namespace CT {
	enum class CThreaderError {
		CThreaderNotInitialized,
		TaskNotFound,
	};

	enum class CThreaderStopFlag {
		CLOSE_AFTER_COMPLETING_PROCESSED_TASKS = 0, // mevcut iþ bitsin, yeni iþ baþlamasýn
		CLOSE_AFTER_COMPLETING_THE_TASKS = 1  // kuyruktaki tüm iþler bitsin
	};
}

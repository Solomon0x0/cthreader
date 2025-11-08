#pragma once
#include <tuple>
#include <type_traits>
#include <utility>

namespace CT {
    template<typename Callable, typename... Args>
    Task::Task(Callable&& func, Args&&... args) noexcept {
        using ResultType = std::invoke_result_t<Callable, Args...>;
        auto t_args = std::make_tuple(std::forward<Args>(args)...);

        m_task = [fn = std::forward<Callable>(func), tuple = std::move(t_args)]() -> std::any {
            if constexpr (std::is_void_v<ResultType>) {
                std::apply(fn, tuple);
                return std::any{};
            }
            else {
                return std::any(std::apply(fn, tuple));
            }
        };
    }
}

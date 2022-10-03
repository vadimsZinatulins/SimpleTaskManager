#pragma once

#include <memory>
#include <type_traits>

#include "TaskFilter.h"
#include "TaskJoiner.h"
#include "Task.h"

namespace stm {

namespace internal {

template<typename Function_t, typename ...Args_t>
auto make_task_ptr(int dependenciesCount, Function_t function, Args_t ...args) {
	using Result_t = std::invoke_result_t<Function_t, typename Args_t::element_type::R...>;

	return std::make_shared<Task<Result_t>>(dependenciesCount, function, args...);
}

}
	
template<typename Function_t, typename ...Args_t>
auto make_task(Function_t function, Args_t ...args) {
	const auto dependenciesCount { sizeof...(Args_t) };

	auto task { 
		std::apply(
			[&function, dependenciesCount] (auto ...nonVoidTasks) { return internal::make_task_ptr(dependenciesCount, function, nonVoidTasks...); }, 
			internal::filterNonVoidTasks(args...)
		) 
	};

	if constexpr (dependenciesCount == 0) {
		Scheduler::getInstance().schedule(task);
	} else {
		(..., args->addOnSchedule([=] { task->signalDependency(); }));
	}

	using TaskType = std::shared_ptr<Task<typename decltype(task)::element_type::R>>;
	return internal::TaskJoiner<TaskType>()(task);
}


}

#pragma once

#include <memory>
#include <type_traits>

#include "TaskJoiner.h"
#include "Task.h"

namespace TaskManager {
	
template<typename Function_t, typename ...Args_t>
auto make_task(Function_t function, Args_t ...args) {
	using Result_t = std::invoke_result_t<Function_t, typename Args_t::element_type::R...>;
	using TaskType = std::shared_ptr<Task<Result_t>>;

	const auto depCount { sizeof...(Args_t) };

	auto task { std::make_shared<Task<Result_t>>(depCount, function, args...) };

	if constexpr (depCount == 0) {
		Scheduler::getInstance().schedule(task);
	} else {
		(..., args->addOnSchedule([=] { task->signalDependency(); }));
	}

	return TaskJoiner<TaskType>()(task);
}


}

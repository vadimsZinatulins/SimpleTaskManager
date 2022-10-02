#pragma once

#include <memory>

#include "Task.h"

namespace TaskManager {

template<typename T>
using TaskPtr = std::shared_ptr<Task<T>>;

template<typename Function_t, typename ...Args_t>
auto make_task(Function_t function, Args_t ...args);

template<typename T>
struct TaskJoiner {
	T operator()(T t) {
		return t;
	}
};

template<typename T>
struct TaskJoiner<TaskPtr<TaskPtr<T>>> {
	auto operator()(auto task) {
		return make_task([=] (auto x) { return x->result(); }, task);
	}
};

}

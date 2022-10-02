#pragma once

#include <memory>

namespace TaskManager {

template<typename T>
struct Task;

template<typename T>
struct TaskTraits {
	static constexpr bool isTask = false;
};

template<typename T>
struct TaskTraits<std::shared_ptr<Task<T>>> {
	static constexpr bool isTask = true;
};

}

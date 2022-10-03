#pragma once

#include <memory>

namespace stm {

template<typename T>
struct Task;

}

namespace stm::internal {

template<typename T>
struct TaskTraits {
	static constexpr bool isTask = false;
};

template<typename T>
struct TaskTraits<std::shared_ptr<Task<T>>> {
	static constexpr bool isTask = true;
};

}

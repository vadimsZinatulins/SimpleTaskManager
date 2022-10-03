#pragma once

#include <tuple>

namespace stm::internal {

template<typename T>
struct FilterOutVoidType {
	static constexpr bool value { true };
};

template<>
struct FilterOutVoidType<void> {
	static constexpr bool value { false };
};

template<typename ...Args_t>
auto filterNonVoidTasks(Args_t ...args) {
	if constexpr(sizeof...(Args_t)) {
		return std::apply([] (auto first, auto ...rest) {
			auto filteredRest { filterNonVoidTasks(rest...) };

			if constexpr(FilterOutVoidType<typename decltype(first)::element_type::R>::value){
				return std::tuple_cat(std::tuple(first), filteredRest);
			} else {
				return filteredRest;
			}
		}, std::tuple(args...));
	} else {
		return std::tuple();
	}

}

}

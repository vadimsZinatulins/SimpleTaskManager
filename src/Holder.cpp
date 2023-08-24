#include "SimpleTaskManager/Holder.h"

namespace stm::internal {

void Holder::attach(std::function<void()> action) {
	auto lock { std::lock_guard(m_mutex) };

	if(m_transferred) {
		m_transferred->attach(action);
	} else if(m_scheduled) {
		action();
	} else {
		m_actions.push_back(action);
	}
}

void Holder::schedule() {
	{
		auto lock { std::lock_guard(m_mutex) };
		m_scheduled = true;
	}

	for(auto &action : m_actions) {
		action();
	}
}

void Holder::transfere(Holder *destination) {
	{
		auto lock { std::lock_guard(m_mutex) };
		m_transferred = destination;
	}

	for(auto &action : m_actions) {
		destination->attach(action);
	}
}

}

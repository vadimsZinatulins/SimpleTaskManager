#pragma once

#include <functional>
#include <mutex>
#include <vector>

namespace stm::internal {

class Holder {
public:
	Holder() = default;
	~Holder() = default;

	void attach(std::function<void()> action);
	void schedule();
	void transfere(Holder *destination);
private:
	std::mutex m_mutex;
	std::vector<std::function<void()>> m_actions;
	Holder *m_transferred { nullptr };
	bool m_scheduled { false };

};

}

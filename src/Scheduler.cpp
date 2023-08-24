#include "SimpleTaskManager/Scheduler.h"
#include "SimpleTaskManager/Task.h"

#include <memory>

namespace stm {

Scheduler &Scheduler::getInstance() {
	static Scheduler instance;

	return instance;
}

void Scheduler::initialize() {
	auto body { [&] {
		while(true) {
			auto task { m_queue.pop() };

			if(!task) {
				return;
			}

			(*task)();
		}
	} };

	m_threads.resize(std::thread::hardware_concurrency());

	for(auto &thread : m_threads) {
		thread = std::thread(body);
	}
}

void Scheduler::terminate() {
	for(auto i = 0u; i < m_threads.size(); ++i) {
		m_queue.push({});
	}

	for(auto &thread : m_threads) {
		thread.join();
	}
}

void Scheduler::schedule(std::shared_ptr<ITask> task) {
	m_queue.push(task);
}

}

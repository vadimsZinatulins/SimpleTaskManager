#include "SimpleTaskManager/TaskQueue.h"

#include <memory>
#include <mutex>

namespace stm::internal {

void TaskQueue::push(std::shared_ptr<ITask> task) {
	auto lock { std::unique_lock(m_mutex) };

	m_queue.push(task);
	m_cv.notify_one();
}

std::shared_ptr<ITask> TaskQueue::pop() {
	auto lock { std::unique_lock(m_mutex) };
	
	m_cv.wait(lock, [this] { return !m_queue.empty(); });

	auto front { m_queue.front() };
	m_queue.pop();

	return front;
}

}

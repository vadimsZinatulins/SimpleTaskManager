#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace stm {

struct ITask;

class TaskQueue {
public:
	TaskQueue() = default;
	~TaskQueue() = default;

	void push(std::shared_ptr<ITask> task);
	std::shared_ptr<ITask> pop();
private:
	std::queue<std::shared_ptr<ITask>> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
};

}

#pragma once

#include <vector>
#include <thread>
#include <memory>

#include "TaskQueue.h"

namespace stm {

struct ITask;

class Scheduler {
public:
	static Scheduler &getInstance();

	void initialize();
	void terminate();

	void schedule(std::shared_ptr<ITask> action);
private:
	internal::TaskQueue m_queue;
	std::vector<std::thread> m_threads;
};

}

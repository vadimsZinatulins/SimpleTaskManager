#include <iostream>

#include "Scheduler.h"
#include "make_task.h"

int main(int argc, char *argv[]) {
	TaskManager::Scheduler::getInstance().initialize();

	auto t { TaskManager::make_task([] { return TaskManager::make_task([] { return 11; }); }) };
	auto t1 { TaskManager::make_task([] { return 4; } ) };
	auto t2 { TaskManager::make_task([] { return 6; }) };
	auto t3 { TaskManager::make_task([](int a, int b, int c) { return a + b + c; }, t1, t2, t) };

	std::cout << t3->result() << std::endl;

	TaskManager::Scheduler::getInstance().terminate();

	return 0;
}

#pragma once

#include <functional>
#include <memory>
#include <future>
#include <atomic>

#include "Holder.h"
#include "TaskTraits.h"
#include "Scheduler.h"

namespace stm {

struct ITask {
	ITask() = default;
	virtual ~ITask() = default;

	virtual void operator()() = 0;
};

template<typename Result_t>
class Task : public ITask, public std::enable_shared_from_this<Task<Result_t>> {
public:
	using R = Result_t;

	template<typename Function_t, typename ...Args_t>
	Task(int dependencyCount, Function_t body, Args_t ...args) : 
		m_unscheduledDependencies(dependencyCount),
   		m_taskExecutor([=] (std::promise<Result_t> &promise) { promise.set_value(body(args->result()...)); }) {
			m_future = m_promise.get_future();
	   	}

	void operator()() override {
		m_taskExecutor(m_promise);

		if constexpr (internal::TaskTraits<Result_t>::isTask) {
			m_holder.transfere(result()->getHolder());
		} else {
			m_holder.schedule();
		}
	}

	Result_t result() { return m_future.get(); }

	void addOnSchedule(std::function<void()> action) {
		m_holder.attach(action);
	}

	internal::Holder *getHolder() {
		return &m_holder;
	}

	void signalDependency() {
		if(!--m_unscheduledDependencies) {
			Scheduler::getInstance().schedule(this->shared_from_this());
		}
	}
private:
	std::function<void(std::promise<Result_t>&)> m_taskExecutor;
	std::promise<Result_t> m_promise;
	std::shared_future<Result_t> m_future;
	std::atomic<int> m_unscheduledDependencies { 0 };
	internal::Holder m_holder;
};

template<>
class Task<void> : public ITask, public std::enable_shared_from_this<Task<void>> {
public:
	using R = void;

	template<typename Function_t, typename ...Args_t>
	Task(int dependencyCount, Function_t body, Args_t ...args) : 
		m_unscheduledDependencies(dependencyCount),
   		m_taskExecutor([=] (std::promise<int> &promise) { body(args->result()...); promise.set_value(1); }) {
			m_future = m_promise.get_future();
	   	}

	void operator()() override {
		m_taskExecutor(m_promise);

		m_holder.schedule();
	}

	void result() { m_future.get(); }

	void addOnSchedule(std::function<void()> action) {
		m_holder.attach(action);
	}

	internal::Holder *getHolder() {
		return &m_holder;
	}

	void signalDependency() {
		if(!--m_unscheduledDependencies) {
			Scheduler::getInstance().schedule(this->shared_from_this());
		}
	}
private:
	std::function<void(std::promise<int>&)> m_taskExecutor;
	std::promise<int> m_promise;
	std::shared_future<int> m_future;
	std::atomic<int> m_unscheduledDependencies { 0 };
	internal::Holder m_holder;
};

}

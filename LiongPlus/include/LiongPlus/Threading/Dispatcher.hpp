// File: Dispatcher.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <functional>
#include <map>
#include <mutex>
#include <thread>
#include <queue>
#include <utility>
#include "_"

_L_NS_BEG_THREADING

class Dispatcher
{
private:
	using dispatched_func_t = std::function<void()>;
	using task_queue_t = std::queue<dispatched_func_t>;

	std::thread::id _ThreadId;
	task_queue_t _TaskQueue;
	std::mutex _Mutex;

	static std::map<size_t, std::shared_ptr<Dispatcher>> _ManagedDispatchers;

public:
	Dispatcher();
	Dispatcher(const Dispatcher&) = delete;
	Dispatcher(Dispatcher&&) = delete;
	~Dispatcher();

	Dispatcher& operator=(const Dispatcher&) = delete;
	Dispatcher& operator=(Dispatcher&&) = delete;

	const std::thread::id ThreadId() const;
	void Invoke(const dispatched_func_t& func);
	void Invoke(dispatched_func_t&& func);

	static std::shared_ptr<Dispatcher>& Current();

	static bool HasRegistered();

	static void Register();
	static void Unregister();
	
	static void DoDispatchedTasks();
};

struct DispatcherGuard
{
	DispatcherGuard();
	~DispatcherGuard();
};

_L_NS_END_THREADING

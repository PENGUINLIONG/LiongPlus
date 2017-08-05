// File: Dispatcher.cpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/Threading/Dispatcher.hpp"

_L_NS_BEG_THREADING

//
// Dispatcher
//

std::map<size_t, std::shared_ptr<Dispatcher>> Dispatcher::_ManagedDispatchers;

Dispatcher::Dispatcher() :
	_ThreadId(std::this_thread::get_id()),
	_TaskQueue()
{
}
Dispatcher::~Dispatcher()
{
}

const std::thread::id Dispatcher::ThreadId() const
{
	return _ThreadId;
}
void Dispatcher::Invoke(const dispatched_func_t& func)
{
	std::lock_guard<std::mutex> lock(_Mutex);
	_TaskQueue.push(func);
}
void Dispatcher::Invoke(dispatched_func_t&& func)
{
	std::lock_guard<std::mutex> lock(_Mutex);
	_TaskQueue.push(std::forward<dispatched_func_t>(func));
}

inline size_t getCurrentThreadIdHash()
{
	return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

std::shared_ptr<Dispatcher>& Dispatcher::Current()
{
	return _ManagedDispatchers.at(getCurrentThreadIdHash());
}

bool Dispatcher::HasRegistered()
{
	return _ManagedDispatchers.find(getCurrentThreadIdHash()) != _ManagedDispatchers.end();
}

void Dispatcher::Register()
{
	_ManagedDispatchers.insert(std::make_pair<size_t, std::shared_ptr<Dispatcher>>(
		getCurrentThreadIdHash(), std::make_shared<Dispatcher>()
	));
}
void Dispatcher::Unregister()
{
	_ManagedDispatchers.erase(getCurrentThreadIdHash());
}

void Dispatcher::DoDispatchedTasks()
{
	std::lock_guard<std::mutex> lock(Dispatcher::Current()->_Mutex);
	auto& queue = Dispatcher::Current()->_TaskQueue;
	while (!queue.empty())
	{
		if (queue.front())
			queue.front()();
		queue.pop();
	}
}

//
// DispatcherGuard
//

DispatcherGuard::DispatcherGuard()
{
	Dispatcher::Register();
}
DispatcherGuard::~DispatcherGuard()
{
	Dispatcher::Unregister();
}

_L_NS_END_THREADING

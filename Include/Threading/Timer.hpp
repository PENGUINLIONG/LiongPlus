// File: Timer.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <chrono>
#include <memory>
#include <thread>
#include "_"
#include "Dispatcher.hpp"
#include "../Event.hpp"

/*
 * Implementation of a timer.
 * [Timer] can be used as simply a timer or as an accurate clock. The clock functionality requires
 *   that the target thread has already registered to [Dispatcher] and loops through dispatched
 *   tasks. Otherwise, the clock events will not be correctly triggered.
 * [Timer] must only be instantiated by make_shared().
 */

_L_NS_BEG_THREADING

template<typename TDuration>
struct TimeElapsedEventArgs
	: public EventArgs
{
	std::chrono::steady_clock::time_point StartTime;
	std::chrono::steady_clock::time_point LastTrigger;
	TDuration ActualInterval;
};

using namespace std::chrono;

template<typename TDuration>
class Timer : public std::enable_shared_from_this<Timer<TDuration>>
{
public:
	using time_point_type = std::chrono::steady_clock::time_point;

private:
	TDuration _Interval;
	TDuration _Nap;
	time_point_type _Beg;
	time_point_type _LastTrigger;
	std::unique_ptr<std::thread> _ClockThread;
	bool _ShouldStop = false;

public:
	Timer() :
		_Interval(),
		_Nap(1ms),
		_Beg(),
		_LastTrigger(),
		_ClockThread(),
		_ShouldStop()
	{}

	template<typename TDur>
	void SetInterval(const TDur& interval)
	{
		_Interval = std::chrono::duration_cast<TDuration>(interval);
	}
	template<typename TDur>
	void SetNap(const TDur& nap)
	{
		_Nap = std::chrono::duration_cast<TDuration>(nap);
	}

	time_point_type Start()
	{
		using namespace std::chrono;

		auto& dispatcher = Dispatcher::Current();
		_Beg = std::chrono::steady_clock::now();
		//if (_Interval == 0ms) return _Beg;
		_LastTrigger = _Beg;

		_ShouldStop = false;
		_ClockThread = std::make_unique<std::thread>([&]() {
			for (;;)
			{
				if (_ShouldStop)
					return;
				auto now = std::chrono::steady_clock::now();
				if (now - _LastTrigger < _Interval)
					std::this_thread::sleep_for(_Nap);
				else
				{
					TimeElapsedEventArgs<TDuration> args;
					args.StartTime = _Beg;
					args.LastTrigger = _LastTrigger;
					args.ActualInterval = std::chrono::duration_cast<TDuration>(now - _LastTrigger);
					dispatcher->Invoke([=]() {
						TimeElapsedEventArgs<TDuration> args_ = args;
						Elapse(shared_from_this(), args_); 
					});
					_LastTrigger = now;
				}
			}
		});
		return _Beg;
	}
	TDuration Stop()
	{
		if (_ClockThread == nullptr)
			return std::chrono::steady_clock::now();
		_ShouldStop = true;
		_ClockThread->join();
		_ClockThread = nullptr;
		return std::chrono::steady_clock::now();
	}

	Event<std::shared_ptr<Timer>, TimeElapsedEventArgs<TDuration>> Elapse;
};

_L_NS_END_THREADING

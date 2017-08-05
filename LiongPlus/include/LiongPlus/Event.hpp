// File: Event.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <functional>
#include <list>
#include <type_traits>
#include "_"

_L_NS_BEG

struct EventArgs
{
};

template<typename TSender, typename TEventArgs>
class Event
{
private:
	using TargetType = std::function<void(const TSender&, TEventArgs&)>;

	std::list<TargetType> _Targets;
public:
	Event() = default;
	Event(const Event&) = default;
	Event(Event&&) = default;

	Event& operator=(const Event&) = default;
	Event& operator=(Event&&) = default;
	inline Event& operator=(const TargetType& target)
	{
		_Targets.clear();
		_Targets.emplace_back(target);
		return *this;
	}
	inline Event& operator=(TargetType&& target)
	{
		_Targets.clear();
		_Targets.emplace_back(std::forward<TargetType>(target));
		return *this;
	}

	inline void operator()(const TSender& sender, TEventArgs& args) const
	{
		if (_Targets.empty()) return;
		for (auto& target : _Targets)
			target(sender, args);
	}

	inline Event& operator+=(const TargetType& target)
	{
		_Targets.emplace_back(target);
		return *this;
	}
	inline Event& operator+=(TargetType&& target)
	{
		_Targets.emplace_back(std::forward<TargetType>(target));
		return *this;
	}

	inline Event& operator-=(const TargetType& target)
	{
		_Targets.remove(target);
		return *this;
	}
};

_L_NS_END

// File: IObserver.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include <stdexcept>
#include "_"

_L_NS_BEG

template<typename T>
class IObserver
{
public:
	virtual void OnCompleted() = 0;
	virtual void OnError(const std::exception& e) = 0;
	virtual void OnNext(T value) = 0;
};

_L_NS_END

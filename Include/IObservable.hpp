// File: IObservable.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include <stdexcept>
#include "_"
#include "IObserver.hpp"

_L_NS_BEG

template<typename T>
class IObservable
{
public:
	virtual void Subscribe(const IObserver<T>& observer) = 0;
	virtual void Unsubscribe(const IObserver<T>& observer) = 0;
};

_L_NS_END

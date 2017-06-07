// File: Slice.hpp
// Author: Rendong Liang (Liong)
#include "Slice.hpp"

_L_NS_BEG

//
// Slice<TIter>
//

template<typename TIter>
Slice<TIter>::Slice(TIter beg, TIter end, size_t step) :
	_Begin(beg), _End(end), _Step(step) {}

template<typename TIter>
typename Slice<TIter>::Iterator Slice<TIter>::begin() const
{
	return Iterator(_Begin, _End, _Step);
}

template<typename TIter>
typename Slice<TIter>::Iterator Slice<TIter>::end() const
{
	return Iterator(_End, _End, _Step);
}

//
// Slice<TIter>::Iterator
//

template<typename TIter>
Slice<TIter>::Iterator::Iterator(TIter cur, TIter end, size_t step) :
	_Cur(cur), _End(end), _Step(step) {}

template<typename TIter>
bool Slice<TIter>::Iterator::operator==(const Iterator& instance)
{
	return _Cur == instance._Cur;
}

template<typename TIter>
typename Slice<TIter>::Iterator& Slice<TIter>::Iterator::operator++()
{
	size_t step = _Step;
	while (step--) if ((_Cur++) == _End) break;
	return *this;
}
template<typename TIter>
typename Slice<TIter>::Iterator Slice<TIter>::Iterator::operator++(int)
{
	Iterator temp = *this;
	++(*this);
	return temp;
}

template<typename TIter>
decltype(*TIter) Slice<TIter>::Iterator::operator*() const
{
	return *TIter;
}
template<typename TIter>
decltype(TIter.operator->()) Slice<TIter>::Iterator::operator->() const
{
	return TIter.operator->();
}

_L_NS_END

// File: Slice.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"

_L_NS_BEG

template<typename TIter>
class Slice
{
private:
	TIter _Begin, _End;
	size_t _Step;
public:
	class Iterator
	{
		friend class Slice<TIter>;
	private:
		TIter _Cur, _End;
		size_t _Step;

		Iterator(TIter cur, TIter end, size_t step);
	public:
		Iterator(const Iterator&) = default;
		Iterator(Iterator&&) = default;

		Iterator& operator=(const Iterator&) = default;
		Iterator& operator=(Iterator&&) = default;
		
		bool operator==(const Iterator& instance);

		Iterator& operator++();
		Iterator operator++(int);
		decltype(*TIter) operator*() const;
		decltype(TIter.operator->()) operator->() const;
	};

	Slice(TIter beg, TIter end, size_t step = 1);
	
	Iterator begin() const;
	Iterator end() const;
};

_L_NS_END

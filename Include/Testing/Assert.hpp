// File: Assert.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include "_"

_L_NS_BEG_TESTING
class Assert
{
public:
	template<typename TA, typename TB>
	static void Equals(const TA& actual, const TB& expectance)
	{
		return Discriminate(actual == expectance);
	}

	template<typename TA, typename TB>
	static void NotEquals(const TA& actual, const TB& expectance)
	{
		return Discriminate(actual != expectance);
	}
private:
	static void Discriminate(bool isTrue);
};
_L_NS_END_TESTING

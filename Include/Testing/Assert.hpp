// File: Assert.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Assert
#define _L_Assert
#include "../Fundamental.hpp"
#include "UnitTest.hpp"

namespace LiongPlus
{
	namespace Testing
	{
		class Assert
		{
		public:
			template<typename T>
			static void Equals(T actual, T expectance)
			{
				return Discriminate(actual == expectance);
			}

			template<typename T>
			static void NotEquals(T actual, T expectance)
			{
				return Discriminate(actual != expectance);
			}
		private:
			static void Discriminate(bool isTrue);
		};
	}
}

#endif

// File: Logger.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Logger
#define _L_Logger
#include "../Fundamental.hpp"
#include "../String.hpp"
#include "../Text/StringBuilder.hpp"
#include "UnitTest.hpp"
#include "../DateTime.hpp"

using namespace LiongPlus::Text;

namespace LiongPlus
{
	namespace Testing
	{
		class Logger
		{
		public:
			static void Log(String label, String msg);

			static void Info(String msg);
			static void Warn(String msg);
			static void Excp(String msg);
			static void Sver(String msg);
		};
	}
}
#endif
// File: Logger.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Logger
#define _L_Logger
#include "../Fundamental.hpp"
#include "UnitTest.hpp"
#include "../DateTime.hpp"

namespace LiongPlus
{
	namespace Testing
	{
		class Logger
		{
		public:
			static void Log(std::string label, std::string msg);

			static void Info(std::string msg);
			static void Warn(std::string msg);
			static void Excp(std::string msg);
			static void Sver(std::string msg);
		};
	}
}
#endif
// File: Logger.hpp
// Author: Rendong Liang (Liong)
#include "Logger.hpp"

using namespace LiongPlus::Text;

namespace LiongPlus
{
	namespace Testing
	{
		void Logger::Log(String label, String msg)
		{
			UnitTest::Results.Last().Log.AppendLine(String::Concat({ DateTime::Now().ToString(), label, msg }));
		}

		void Logger::Info(String msg)
		{
			Log(_LT(" [INFO] "), msg);
		}
		void Logger::Warn(String msg)
		{
			Log(_LT(" [WARN] "), msg);
		}
		void Logger::Excp(String msg)
		{
			Log(_LT(" [EXCP] "), msg);
		}
		void Logger::Sver(String msg)
		{
			Log(_LT("[SVER] "), msg);
		}
	}
}
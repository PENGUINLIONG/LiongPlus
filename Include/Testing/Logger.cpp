// File: Logger.hpp
// Author: Rendong Liang (Liong)
#include "Logger.hpp"

namespace LiongPlus
{
	namespace Testing
	{
		void Logger::Log(std::string label, std::string msg)
		{
			*UnitTest::Results.Last().Log <<  DateTime::Now().ToString() << label << msg;
		}

		void Logger::Info(std::string msg)
		{
			Log("[INFO]", msg);
		}
		void Logger::Warn(std::string msg)
		{
			Log("[WARN]", msg);
		}
		void Logger::Excp(std::string msg)
		{
			Log("[EXCP]", msg);
		}
		void Logger::Sver(std::string msg)
		{
			Log("[SVER]", msg);
		}
	}
}
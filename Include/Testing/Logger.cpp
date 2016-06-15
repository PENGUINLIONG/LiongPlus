// File: Logger.cpp
// Author: Rendong Liang (Liong)
#include "Logger.hpp"

namespace LiongPlus
{
	namespace Testing
	{
		void Logger::Log(std::string label, std::string msg)
		{
			*UnitTest::Results.Last().Log <<  DateTime::Now().To_L_String() << label << msg;
		}

		void Logger::Info(std::string msg)
		{
<<<<<<< HEAD
			Log("[INFO]", msg);
=======
			Log(_LT(" [INFO] "), msg);
>>>>>>> master
		}
		void Logger::Warn(std::string msg)
		{
<<<<<<< HEAD
			Log("[WARN]", msg);
=======
			Log(_LT(" [WARN] "), msg);
>>>>>>> master
		}
		void Logger::Excp(std::string msg)
		{
<<<<<<< HEAD
			Log("[EXCP]", msg);
=======
			Log(_LT(" [EXCP] "), msg);
>>>>>>> master
		}
		void Logger::Sver(std::string msg)
		{
<<<<<<< HEAD
			Log("[SVER]", msg);
=======
			Log(_LT("[SVER] "), msg);
>>>>>>> master
		}
	}
}
// File: Logger.hpp
// Author: Rendong Liang (Liong)
#pragma

#include <string>
#include "_"

_L_NS_BEG_TESTING
class Logger
{
public:
	static void Log(std::string label, std::string msg);

	static void Info(std::string msg);
	static void Warn(std::string msg);
	static void Excp(std::string msg);
	static void Sver(std::string msg);
};
_L_NS_END_TESTING

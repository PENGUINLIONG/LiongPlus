// File: FileUtils.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <string>
#include "_"
#include "../Buffer.hpp"
#include "../BufferPool.hpp"

_L_NS_BEG_FILEFORMAT

namespace FileUtils
{
	Buffer ReadAllToBuffer(const std::string& path);
	std::string ReadAllToString(const std::string& path);
}

_L_NS_END_FILEFORMAT

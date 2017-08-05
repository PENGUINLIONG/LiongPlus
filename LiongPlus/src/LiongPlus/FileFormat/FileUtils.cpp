// File: FileUtils.hpp
// Author: Rendong Liang (Liong)
#include <fstream>
#include "LiongPlus/FileFormat/FileUtils.hpp"

_L_NS_BEG_FILEFORMAT

namespace FileUtils
{
	using namespace std;

	Buffer ReadAllToBuffer(const std::string& path)
	{
		ifstream fs(path, ios::binary | ios::ate);
		if (fs.fail())
			throw std::runtime_error("Unable to open file.");
		size_t size = fs.tellg();
		fs.seekg(ios::beg, 0);
		Buffer buffer(size);
		fs.read((char*)buffer.Field(), size);
		fs.close();
		return buffer;
	}
	std::string ReadAllToString(const std::string& path)
	{
		ifstream fs(path, ios::binary | ios::ate);
		if (fs.fail())
			throw std::runtime_error("Unable to open file.");
		size_t size = fs.tellg();
		fs.seekg(ios::beg, 0);
		std::string str(size, '\0');
		fs.read((char*)str.data(), size);
		fs.close();
		return str;
	}
}

_L_NS_END_FILEFORMAT

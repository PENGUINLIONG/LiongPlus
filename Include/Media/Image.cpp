// File: Image.cpp
// Author: Rendong Liang (Liong)
#include "Media/Image.hpp"

namespace LiongPlus
{
	namespace Media
	{
		Image* Image::FromMemory(MemoryStream stream)
		{
			//if (stream.ReadByte() == 'B')
			throw std::logic_error("Not implemented yet.");
		}
	}
}
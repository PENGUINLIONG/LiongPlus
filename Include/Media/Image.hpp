// File: Image.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "../Buffer.hpp"

_L_NS_BEG_MEDIA

enum PixelFormat
{
	Alpha, Red, Green, Blue,
	Rgb, Bgr,
	Rgba
};

class Image
{
public:
	Buffer Data;
	int Width;
	int Height;
	PixelFormat Format;
};

_L_NS_END_MEDIA

#pragma once
#include "_"
#include "../Media/Color.hpp"
#include "Matrix4x4.hpp"

_L_NS_BEG_GRAPHICS

Vector4 ClampColor(const Media::Color& color)
{
	return {
		float(color.Red) / 255., float(color.Green) / 255.,
		float(color.Blue) / 255., float(color.Alpha) / 255.
	};
}

Media::Color RestoreColor(const Vector4& color)
{
	return {
		unsigned char(color.X * 255),
		unsigned char(color.Y * 255),
		unsigned char(color.Z * 255),
		unsigned char(color.W * 255)
	};
}

_L_NS_END_GRAPHICS

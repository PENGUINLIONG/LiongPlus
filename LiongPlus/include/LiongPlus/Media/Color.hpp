// File: Color.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <cstdint>
#include "_"
#include "../Matrix4x4.hpp"

_L_NS_BEG_MEDIA

struct Color
{
	uint8_t Red, Green, Blue, Alpha = 255;

	uint8_t operator()(size_t pos) const;
	uint8_t& operator()(size_t pos);
};

namespace ColorUtils
{
	// Alpha channel is also count.
	enum ColorSpace
	{
		Rgb,
		YCbCr,
	};

	extern Vector4 ToFloatColor(const Color& color);
	extern Color ToIntColor(const Vector4& color);

	/*
	 * Details about conversion can be found here:
	 *   https://www.w3.org/Graphics/JPEG/jfif3.pdf
	 */
	template<ColorSpace From, ColorSpace To, typename T>
	T Convert(const T& color);
}

_L_NS_END_MEDIA

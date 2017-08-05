// File: Color.cpp
// Author: Rendong Liang (Liong)
#include <stdexcept>
#include "LiongPlus/Media/Color.hpp"

_L_NS_BEG_MEDIA

uint8_t Color::operator()(size_t pos) const
{
	switch (pos)
	{
		case 0: return Red;
		case 1: return Green;
		case 2: return Blue;
		case 3: return Alpha;
		default: throw std::runtime_error("Index out of range");
	}
}
uint8_t& Color::operator()(size_t pos)
{
	switch (pos)
	{
		case 0: return Red;
		case 1: return Green;
		case 2: return Blue;
		case 3: return Alpha;
		default: throw std::runtime_error("Index out of range");
	}
}

namespace ColorUtils
{
	Vector4 ToFloatColor(const Color& color)
	{
		return {
			Float(color.Red) / Float(255.),
			Float(color.Green) / Float(255.),
			Float(color.Blue) / Float(255.),
			Float(color.Alpha) / Float(255.),
		};
	}

	Color ToIntColor(const Vector4& color)
	{
		auto clamped = color.Clamp(0., 1.);
		return {
			uint8_t(clamped.X * 255.),
			uint8_t(clamped.Y * 255.),
			uint8_t(clamped.Z * 255.),
			uint8_t(clamped.W * 255.),
		};
	}

	template<>
	Vector4 Convert<Rgb, YCbCr, Vector4>(const Vector4& color)
	{
		return {
			(Float).299 * color.X +
			(Float).587 * color.Y +
			(Float).114 * color.Z,

			(Float)-.1687 * color.X -
			(Float).3313 * color.Y +
			(Float).5 * color.Z + (Float).5,

			(Float).5 * color.X -
			(Float)0.4187 * color.Y -
			(Float).0813 * color.Z + (Float).5,

			color.W
		};
	}
	template<>
	Vector4 Convert<YCbCr, Rgb, Vector4>(const Vector4& color)
	{
		return {
			color.X + (Float)1.402 * (color.Z - (Float).5),
			color.X - (Float).34414 * (color.Y - (Float).5) - (Float).71414 * (color.Z - (Float).5),
			color.X + (Float)1.772 * (color.Y - (Float).5),
			color.W
		};
	}
	template<>
	Color Convert<Rgb, YCbCr, Color>(const Color& color)
	{
		return ToIntColor(Convert<Rgb, YCbCr>(ToFloatColor(color)));
	}
	template<>
	Color Convert<YCbCr, Rgb, Color>(const Color& color)
	{
		return ToIntColor(Convert<YCbCr, Rgb>(ToFloatColor(color)));
	}
}

_L_NS_END_MEDIA

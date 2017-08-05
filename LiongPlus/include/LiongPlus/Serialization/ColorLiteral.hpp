// File: ColorLiteral.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <string>
#include "_"
#include "../Media/Color.hpp"
#include "../SafeError.hpp"

_L_NS_BEG_SERIALIZATION

namespace ColorLiteral
{
	enum ColorLiteralParseErrorCode : uint_fast8_t
	{
		Success_Perfect         = 0b0'000'0000,
		// Non-hexdigit occured.
		Error_InvalidChar       = 0b1'000'0001,
		Error_UnsupportedFormat = 0b1'000'0010,
		Error_EmptyString       = 0b1'000'0011,

		Error_Mask              = 0b1'000'0000,
	};

	using ColorLiteralParseError = SafeError<ColorLiteralParseErrorCode, uint_fast8_t>;

	Media::Color Parse(const std::string& str);

	ColorLiteralParseError TryParse(const std::string& str, Media::Color& to) noexcept;
}

_L_NS_END_SERIALIZATION

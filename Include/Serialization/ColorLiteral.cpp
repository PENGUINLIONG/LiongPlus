// File: ColorLiteral.cpp
// Author: Rendong Liang (Liong)
#include "ColorLiteral.hpp"
#include "../AnsiCharUtils.hpp"

_L_NS_BEG_SERIALIZATION

namespace ColorLiteral
{
	namespace details
	{
		using namespace std;
		using namespace Media;

		unsigned char transHex(char hexChar)
		{
			switch (hexChar)
			{
			case '0': return 0;
			case '1': return 1;
			case '2': return 2;
			case '3': return 3;
			case '4': return 4;
			case '5': return 5;
			case '6': return 6;
			case '7': return 7;
			case '8': return 8;
			case '9': return 9;
			case 'A': case 'a': return 10;
			case 'B': case 'b': return 11;
			case 'C': case 'c': return 12;
			case 'D': case 'd': return 13;
			case 'E': case 'e': return 14;
			case 'F': case 'f': return 15;
			default: return 0xFF;
			}
		}

		void parseSharpRGB(const string& pos, Color& to)
		{
			unsigned char red, blue, green;
			red = transHex(pos[0]);
			blue = transHex(pos[1]);
			green = transHex(pos[2]);
			to ={
				unsigned char(red * 17),
				unsigned char(blue * 17),
				unsigned char(green * 17),
				unsigned char(255) };
		}
		void parseSharpRRGGBB(const string& pos, Color& to)
		{
			unsigned char red1, blue1, green1, red2, blue2, green2;
			red1 = transHex(pos[0]);
			red2 = transHex(pos[1]);
			blue1 = transHex(pos[2]);
			blue2 = transHex(pos[3]);
			green1 = transHex(pos[4]);
			green2 = transHex(pos[5]);
			to ={
				unsigned char(red1 * 16 + red2),
				unsigned char(blue1 * 16 + blue2),
				unsigned char(green1 * 16 + green2),
				unsigned char(255)
			};
		}
		void parseSharpRGBA(const string& pos, Color& to)
		{
			unsigned char red, blue, green, alpha;
			red = transHex(pos[0]);
			blue = transHex(pos[1]);
			green = transHex(pos[2]);
			alpha = transHex(pos[3]);
			to ={
				unsigned char(red * 17),
				unsigned char(blue * 17),
				unsigned char(green * 17),
				unsigned char(alpha * 17)
			};
		}
		void parseSharpRRGGBBAA(const string& pos, Color& to)
		{
			unsigned char red1, blue1, green1, alpha1,
				red2, blue2, green2, alpha2;
			red1 = transHex(pos[0]);
			red2 = transHex(pos[1]);
			blue1 = transHex(pos[2]);
			blue2 = transHex(pos[3]);
			green1 = transHex(pos[4]);
			green2 = transHex(pos[5]);
			alpha1 = transHex(pos[6]);
			alpha2 = transHex(pos[7]);
			to ={
				unsigned char(red1 * 16 + red2),
				unsigned char(blue1 * 16 + blue2),
				unsigned char(green1 * 16 + green2),
				unsigned char(alpha1 * 16 + alpha2) };
		}

		/*
		* Parse heximal color code.
		* e.g. #FFFFFF or #123 (which equals to #112233)
		* Params:
		*   $pos: The iterator AFTER the sharp sign(#).
		*   $end: The iterator at the end of string to parse.
		*   to: The parsed color object is returned through reference.
		* Returns:
		*   True is the parsing is successfully finished.
		*   If the format of heximal color code is unknown, or unsupported, false will be returned.
		*/
		ColorLiteralParseError parseHexColor(const string::const_iterator& pos, const string::const_iterator& end, Color& to)
		{
			auto hexCode = AnsiCharUtils::CaptureNonWhiteSpace<std::string>(pos, end);
			if (!AnsiCharUtils::ConditionalEquals(
				hexCode.begin(), hexCode.end(), &AnsiCharUtils::IsHexDigit))
				return Error_InvalidChar;
			switch (hexCode.size())
			{
			case 3:
				details::parseSharpRGB(hexCode, to);
				break;
			case 4:
				details::parseSharpRGBA(hexCode, to);
				break;
			case 6:
				details::parseSharpRRGGBB(hexCode, to);
				break;
			case 8:
				details::parseSharpRRGGBBAA(hexCode, to);
				break;
			default:
				return Error_UnsupportedFormat;
			}
			return Success_Perfect;
		}
	}

	Media::Color Parse(const std::string& str)
	{
		Media::Color color;
		if (!TryParse(str, color).IsSuccess())
			throw std::runtime_error("Failed in parsing color literal.");
		return color;
	}

	ColorLiteralParseError TryParse(const std::string& str, Media::Color& to) noexcept
	{
		auto beg = AnsiCharUtils::SeekForNonWhiteSpace(str.begin(), str.end());
		auto end = str.end();
		if (end == beg) return Error_EmptyString;

		if ('#' == *beg)
			return details::parseHexColor(beg + 1, end, to);
		else return Error_UnsupportedFormat;
	}
}

_L_NS_END_SERIALIZATION

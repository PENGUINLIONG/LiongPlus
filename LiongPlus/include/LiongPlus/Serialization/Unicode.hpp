// File: Unicode.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <codecvt>
#include <locale>
#include <string>
#include <type_traits>
#include "_"
#include "../SafeError.hpp"

_L_NS_BEG_SERIALIZATION

namespace Unicode
{
	enum UnicodeErrorCode
	{
		Success_Perfect               = 0b0'000'0000,

		Error_UnexpectedEndOfString   = 0b1'000'0001,
		Error_UnsupportedUnicodeRange = 0b1'000'0010,
		Error_UnexpectedNonSurrogate  = 0b1'000'0011,
		Error_Mask                    = 0b1'000'0000,
	};

	using UnicodeError = SafeError<UnicodeErrorCode, uint_fast8_t>;
	
	template<typename TStrIt>
	UnicodeError
		TryFromUtf8(const TStrIt& from, const TStrIt& to,
		std::u32string& rv)  noexcept
	{
		using namespace Unicode;
		rv.clear();
		auto pos = from;
		while (pos < to)
		{
			auto ch = *pos;
			if (((ch >> 7) & 0b1) == 0) // 1 byte.
				rv.push_back(*(pos++));
			else if (((ch >> 5) & 0b111) == 0b110) // 2 byte.
			{
				if (!(pos + 1 < to))
					return Error_UnexpectedEndOfString;
				uint32_t temp =
					((*(pos++) & 0b0001'1111) << 6) |
					(*(pos++) & 0b0011'1111);
				rv.push_back(temp);
			}
			else if (((ch >> 4) & 0b1111) == 0b1110) // 3 byte.
			{
				if (!(pos + 2 < to))
					return Error_UnexpectedEndOfString;
				uint32_t temp =
					((*(pos++) & 0b0000'1111) << 12) |
					((*(pos++) & 0b0011'1111) << 6) |
					(*(pos++) & 0b0011'1111);
				rv.push_back(temp);
			}
			else if (((ch >> 3) & 0b1'1111) == 0b1'1110) // 4 byte.
			{
				if (!(pos + 3 < to))
					return Error_UnexpectedEndOfString;
				uint32_t temp =
					((*(pos++) & 0b0000'0111) << 18) |
					((*(pos++) & 0b0011'1111) << 12) |
					((*(pos++) & 0b0011'1111) << 6) |
					(*(pos++) & 0b0011'1111);
				rv.push_back(temp);
			}
			else
				// Cannot hold more data in [uint32_t].
				return Error_UnsupportedUnicodeRange;
		}
		return Success_Perfect;
	}
	template<typename TStrIt>
	std::u32string
		FromUtf8(
		const TStrIt& from, const TStrIt& to)
	{
		std::u32string rv;
		if (TryFromUtf8(from, to, rv).IsError())
			throw std::runtime_error("Unable to decode UTF-8.");
		return std::move(rv);
	}

	template<typename TStrIt>
	std::enable_if_t<std::is_same_v<typename TStrIt::value_type, char32_t>, UnicodeError>
		TryToUtf8(const TStrIt& from, const TStrIt& to, std::string& out)
	{
		out.clear();
		auto pos = from;
		while (pos != to)
		{
			auto ch = *pos;
			if (ch < 0x80)
			{
				out.push_back(ch);
			}
			else if (ch < 0x08'00)
			{
				out.push_back((ch >> 6) & 0x1F | 0xC0);
				out.push_back(ch & 0x3F | 0x80);
			}
			else if (ch < 0x1'0000)
			{
				out.push_back((ch >> 12) & 0x0F | 0xE0);
				out.push_back((ch >> 6) & 0x3F | 0x80);
				out.push_back(ch & 0x3F | 0x80);
			}
			else if (ch < 0x11'0000)
			{
				out.push_back((ch >> 18) & 0x07 | 0xF0);
				out.push_back((ch >> 12) & 0x3F | 0x80);
				out.push_back((ch >> 6) & 0x3F | 0x80);
				out.push_back(ch & 0x3F | 0x80);
			}
			else
			{
				return Error_UnsupportedUnicodeRange;
			}
			pos++;
		}
		return Success_Perfect;
	}
	template<typename TStrIt>
	std::string
		ToUtf8(const TStrIt& from, const TStrIt& to)
	{
		std::string rv;
		if (TryToUtf8(from, to, rv).IsError())
			throw std::runtime_error("Unable to encode UTF-8.");
		return rv;
	}

	template<typename TStrIt>
	UnicodeError
		TryFromUtf16(const TStrIt& from, const TStrIt& to,
		std::u32string& rv) noexcept
	{
		using namespace Unicode;
		rv.clear();
		auto pos = from;
		uint32_t highSur = 0;

		while (pos < to)
		{
			auto ch = *pos;
			if (ch < 0xD800 || ch >= 0xE000) // UCS-2 compatible.
			{
				if (highSur) return Error_UnexpectedNonSurrogate;
				rv.push_back(ch);
			}
			else if (ch < 0xDC00)
			{
				if (highSur) return Error_UnexpectedNonSurrogate;
				highSur = (ch & 0b11'1111'1111) << 10;
			}
			else // (ch < 0xE000)
			{
				rv.push_back(highSur | (ch & 0b11'1111'1111));
				highSur = 0;
			}

			++pos;
		}

		return Success_Perfect;
	}
	template<typename TStrIt>
	std::u32string
		FromUtf16(
		const TStrIt& from, const TStrIt& to)
	{
		std::u32string rv;
		if (TryFromUtf16(from, to, rv).IsError())
			throw std::runtime_error("Unable to decode UTF-16.");
		return std::move(rv);
	}

	template<typename TStrIt>
	UnicodeError
		TryToUtf16(const TStrIt& from, const TStrIt& to, std::u16string& rv)
	{
		auto pos = from;
		while (pos != to)
		{
			auto ch = *pos;
			if (ch < 0x1'0000) // Single wide-char.
				rv.push_back(char16_t(ch));
			else
			{
				ch -= 0x1'0000;
				if (ch >> 20)
					// Out of UTF-16 capability.
					return Error_UnsupportedUnicodeRange;
				// Multi wide-char
				wchar_t hi = 0xD800 +
					((ch >> 10) & 0b11'1111'1111);
				rv.push_back(hi);
				wchar_t lo = 0xDC00 +
					(ch & 0b11'1111'1111);
				rv.push_back(lo);
			}
			pos++;
		}
		return Success_Perfect;
	}
	template<typename TStrIt>
	std::u16string
		ToUtf16(const TStrIt& from, const TStrIt& to)
	{
		std::u16string rv;
		if (TryToUtf16(from, to, rv).IsError())
			throw std::runtime_error("Unable to encode UTF-8.");
		return rv;
	}
}

_L_NS_END_SERIALIZATION

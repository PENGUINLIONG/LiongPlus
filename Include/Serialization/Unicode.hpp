// File: Unicode.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <vector>
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
	UnicodeError FromUtf8(const TStrIt& from, const TStrIt& to, std::vector<uint32_t>& rv)
	{
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
}

_L_NS_END_SERIALIZATION

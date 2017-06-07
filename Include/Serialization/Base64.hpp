// File: Base64.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <string>
#include "_"
#include "../Buffer.hpp"
#include "../SafeError.hpp"

_L_NS_BEG_SERIALIZATION

/*
 * About Base64 Encoding:
 *   base64 encode binary data to literals.
 *   Valid Base64 characters are:
 *     A-Z a-z 1-9 + / = (64 representations, = for zero padding)
 *   Each 6 bit is represented by one base64 char.
 */
namespace Base64
{
	enum Base64ErrorCode : uint_fast8_t
	{
		Success_Perfect      = 0b0'000'0000,

		Error_InvalidChar    = 0b1'000'0001,
		Error_CannotFillByte = 0b1'000'0010,
		Error_ZeroSize       = 0b1'000'0011,
		Error_Mask           = 0b1'000'0000,
	};
	using Base64Error = SafeError<Base64ErrorCode, uint_fast8_t>;

	Base64Error TryEncode(const void* raw, size_t size, std::string& to) noexcept;
	std::string Encode(const void* raw, size_t size);

	Base64Error CalculateDecodedSize(const std::string& base64, size_t& to) noexcept;
	Base64Error TryDecode(const std::string& base64, void* to) noexcept;
	Buffer Decode(const std::string& base64);
}

_L_NS_END_SERIALIZATION

// File: Base64.cpp
// Author: Rendong Liang (Liong)
#include <stdexcept>
#include "Base64.hpp"
#include "../AnsiCharUtils.hpp"

_L_NS_BEG_SERIALIZATION

namespace Base64
{
	namespace details
	{
		size_t calcEncodedLen(size_t size)
		{
			size *= 4;
			// Remainder.
			size_t rem = size % 3;
			// Align to 6 bits.
			switch (size % 6)
			{
			case 0:
				return size / 3;
			case 1:
				return (size + 6) / 3;
			case 2:
				return (size + 3) / 3;
			}
		}

		Byte mapBase64Char(Byte byte)
		{
			return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz"
				"0123456789"
				"+/"[byte];
		}
		Byte revertBase64Char(Byte ch)
		{
			if (ch >= 'A' && ch <= 'Z')
				return ch - 'A';
			if (ch >= 'a' && ch <= 'z')
				return ch - 'a' + 26;
			if (ch >= '0' && ch <= '9')
				return ch - '0' + 52;
			if (ch == '+') return 62;
			if (ch == '/') return 63;
			if (ch == '=' || ch == '\0') return 0xFE;
			return 0xFF;
		}
	}

	Base64Error TryEncode(const void* raw, size_t size, std::string& to) noexcept
	{
		if (size == 0)
			return Error_ZeroSize;

		auto data = reinterpret_cast<const Byte*>(raw);
		// Keep place for '\0'.
		std::string rv(details::calcEncodedLen(size) + 1, 0);
		size_t rvIndex = 0;
		Byte cache;

		for (size_t i = 0; i < size; ++i)
		{
			switch (i % 3)
			{
			case 0:
				rv[rvIndex++] =
					details::mapBase64Char(data[i] >> 2);
				cache = data[i] & 0b0000'0011;
				break;
			case 1:
				rv[rvIndex++] =
					details::mapBase64Char(cache << 4 | data[i] >> 4);
				cache = data[i] & 0b0000'1111;
				break;
			case 2:
				rv[rvIndex++] =
					details::mapBase64Char(cache << 2 | data[i] >> 6);
				rv[rvIndex++] =
					details::mapBase64Char(data[i] & 0b0011'1111);
				break;
			}
		}
		// Zero padding.
		switch (rvIndex % 3)
		{
		case 2:
			rv[rvIndex++] = details::mapBase64Char(cache << 4);
			rv[rvIndex++] = '=';
			rv[rvIndex++] = '=';
			break;
		case 1:
			rv[rvIndex++] = details::mapBase64Char(cache << 2);
			rv[rvIndex++] = '=';
			break;
		}
		to = std::move(rv);
		return Success_Perfect;
	}

	std::string Encode(const void* raw, size_t size)
	{
		std::string base64;
		if (!TryEncode(raw, size, base64).IsSuccess())
			throw std::logic_error("Unable to encode to base64");
		return std::move(base64);
	}

	Base64Error CalculateDecodedSize(const std::string& base64, size_t& to) noexcept
	{
		auto size = base64.find_first_of('=') * 3;
		switch (size % 4)
		{
		case 0:
			to = size / 4;
			return Success_Perfect;
		case 2:
			to = size / 4 + 1;
			return Success_Perfect;
		case 3:
			to = size / 4 + 2;
			return Success_Perfect;
		default:
			return Error_CannotFillByte;
		}
	}

	Base64Error TryDecode(const std::string& base64, void* to) noexcept
	{
		auto data = reinterpret_cast<const Byte*>(base64.data());
		auto target = reinterpret_cast<Byte*>(to);
		auto size = base64.size();
		if (size == 0) return Error_ZeroSize;
		size_t dataIndex = 0;
		Byte cache;

		for (size_t i = 0; dataIndex < size; ++i)
		{
			switch (i % 3)
			{
			case 0:
				cache = details::revertBase64Char(data[dataIndex++]);
				if (cache == 0xFF) return Error_InvalidChar;
				if (cache == 0xFE) return Success_Perfect;
				target[i] = cache << 2;
				cache = details::revertBase64Char(data[dataIndex++]);
				if (cache == 0xFF) return Error_InvalidChar;
				if (cache == 0xFE) return Success_Perfect;
				target[i] |= cache >> 4;
				break;
			case 1:
				target[i] = cache << 4;
				cache = details::revertBase64Char(data[dataIndex++]);
				if (cache == 0xFF) return Error_InvalidChar;
				if (cache == 0xFE) return Success_Perfect;
				target[i] |= cache >> 2;
				break;
			case 2:
				target[i] = cache << 6;
				cache = details::revertBase64Char(data[dataIndex++]);
				if (cache == 0xFF) return Error_InvalidChar;
				if (cache == 0xFE) return Success_Perfect;
				target[i] |= cache;
				break;
			}
		}
		return Success_Perfect;
	}

	Buffer Decode(const std::string& base64)
	{
		size_t size = 0;
		if (!CalculateDecodedSize(base64, size).IsSuccess())
			throw std::logic_error("Failed in calculating decoded base64 size.");
		Buffer buffer(size);
		if (!TryDecode(base64, buffer.Field()).IsSuccess())
			throw std::logic_error("Failed in decoding base64.");
		return std::move(buffer);
	}
}

_L_NS_END_SERIALIZATION

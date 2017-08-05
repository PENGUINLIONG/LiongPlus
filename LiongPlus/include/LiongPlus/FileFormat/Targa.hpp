// File: Targa.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <cstdint>
#include "_"
#include "../SafeError.hpp"
#include "../Media/Image.hpp"
#include "../MemoryReader.hpp"

_L_NS_BEG_FILEFORMAT

namespace Targa
{
	enum TargaResolutionErrorCode
	{
		Success_Perfect              = 0b0'000'0000,

		// The file is shorter than the size required to store header
		//   fields.
		Error_FileTooShort           = 0b1'000'0001,
		// Magic number doesn't match.
		Error_MagicNumberMismatched  = 0b1'000'0010,
		Error_UnsupportedPixelFormat = 0b1'000'0011,
		Error_Mask                   = 0b1'000'0000,
	};
	using TargaResolutionError = SafeError<TargaResolutionErrorCode, uint8_t>;

	TargaResolutionError
		TryResolveFully(const void* raw, size_t size, Media::Image& out, Array<Byte>& idOut);
	TargaResolutionError
		TryResolve(const void* raw, size_t size, Media::Image& out);
	Media::Image
		Resolve(const void* raw, size_t size);
}

_L_NS_END_FILEFORMAT

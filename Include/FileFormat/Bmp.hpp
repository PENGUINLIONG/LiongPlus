// File: Bmp.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "../Media/Image.hpp"
#include "../SafeError.hpp"

_L_NS_BEG_FILEFORMAT

/*
 * Provide resolution of Windows Device Independent Bitmap files.
 *
 * Structure of BMP Files (Used fields are marked $):
 *   File Header:
 *     0x00 $ [2] Type code of the bmp file. Usually "BM"
 *     0x02   [4] Size of the bmp file (in bytes).
 *     0x06   [2] Useless.
 *     0x08   [2] Useless.
 *     0x0C $ [4] Offset to the pixel data.
 *   DIB (Info) Header:
 *     0x00   [4] Size of this header.
 *     0x04 $ [4 unsigned] Width of the image.
 *     0x08 $ [4 unsigned] Height of the image.
 *     0x0A   [2] Number of color planes. Always 1.
 *     0x0C   [2] Bits per pixel.
 *     0x10   [4] Type of compression. For 24-bit it should be 0.
 *     0x14   [4] Image size. Can be 0 when it uses 24-bit pixels.
 *     0x18   [4 unsigned] Number of pixels per meter. (Horizontal)
 *     0x1C   [4 unsigned] Number of pixels per meter. (Veritical)
 *     0x20   [4] Number of colors used in palette. 0 for 24-bit.
 *     0x24   [4] Number of important colors used in palette. 0 for
 *       24-bit.
 */
namespace Bmp
{
	enum BmpResolutionErrorCode : uint_fast8_t
	{
		Success_Perfect              = 0b0'000'0000,

		// The file is shorter than the size required to store header
		//   fields.
		Error_FileTooShort           = 0b1'000'0001,
		// Magic number doesn't match.
		Error_MagicNumberMismatched  = 0b1'000'0010,
		// Only 24-bit BGR bmp is resoluted currently.
		Error_UnsupportedPixelFormat = 0b1'000'0011,
		Error_Mask                   = 0b1'000'0000,
	};

	using BmpResolutionError = SafeError<BmpResolutionErrorCode, uint_fast8_t>;

	BmpResolutionError TryResolve(const void* raw, size_t size, Media::Image& out) noexcept;
	Media::Image Resolve(const void* raw, size_t size);
}

_L_NS_END_FILEFORMAT

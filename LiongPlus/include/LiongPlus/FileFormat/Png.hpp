// File: Png.hpp
// Author: Rendong Liang (Liong)
#include <cstdint>
#include "_"
#include "../Media/Image.hpp"
#include "../SafeError.hpp"

_L_NS_BEG_FILEFORMAT

/*
 * This PNG resolution module is based on libPNG.
 */
namespace Png
{
	enum PngResolutionErrorCode
	{
		Success_Perfect               = 0b0'000'0000,

		// The file is shorter than the size required to store its
		//   magic number.
		Error_FileTooShort            = 0b1'000'0001,
		// Magic number doesn't match.
		Error_MagicNumberMismatched   = 0b1'000'0010,
		// Internal error from libpng.
		Error_InternalError           = 0b1'000'0011,
		// Channel count is not 1, 3 or 4.
		Error_UnsupportedChannelCount = 0b1'000'0100,
		Error_Mask                    = 0b1'000'0000,
	};

	using PngResolutionError = SafeError<PngResolutionErrorCode, uint8_t>;

	PngResolutionError
		TryResolve(const void* raw, size_t size, Media::Image& out);
	Media::Image
		Resolve(const void* raw, size_t size);
}

_L_NS_END_FILEFORMAT

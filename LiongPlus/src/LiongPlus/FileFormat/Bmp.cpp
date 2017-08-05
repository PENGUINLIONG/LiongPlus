// File: Bmp.hpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/FileFormat/Bmp.hpp"

_L_NS_BEG_FILEFORMAT

namespace Bmp
{
	using namespace Media;

	BmpResolutionError TryResolve(const void* raw, size_t size, Image& out) noexcept
	{
		auto file = (const char*)(raw);
		if (size < 54)
			return Error_FileTooShort;
		if (file[0] != 'B' || file[1] != 'M')
			return Error_MagicNumberMismatched;
		if (*((uint16_t*)(file + 28)) != 24)
			return Error_UnsupportedPixelFormat;

		auto length = *((int32_t*)(file + 2));
		auto offset = *((uint32_t*)(file + 10));
		auto width = *((int32_t*)(file + 18));
		auto height = *((int32_t*)(file + 22));
		Buffer pixels(length);
		std::copy(file + offset, file + offset + width * height * 3, pixels.Field());
		out.Data = std::move(pixels);
		out.Width = width;
		out.Height = height;
		out.Format = PixelFormat::Bgr;

		return Success_Perfect;
	}

	Image Resolve(const void* raw, size_t size)
	{
		Image img;
		if (!TryResolve(raw, size, img).IsSuccess())
			throw std::logic_error("Failed in resolving BMP file.");
		return std::move(img);
	}
}

_L_NS_END_FILEFORMAT

// File: Targa.cpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/Fileformat/Targa.hpp"

_L_NS_BEG_FILEFORMAT

namespace Targa
{
	enum TargaImageType
	{
		NoImage              = 0,
		ColorMapped          = 1,
		TrueColor            = 2,
		GrayScale            = 3,
		RunLengthColorMapped = 9,
		RunLengthTrueColor   = 10,
		RunLengthGrayScale   = 11,
	};
	struct TargaHeader
	{
		uint_fast8_t ImageIdSize;
		uint_fast8_t ColorMapType;
		TargaImageType ImageType;
		struct
		{
			uint_fast16_t FirstEntryIndex;
			uint_fast16_t ColorMapLength;
			uint_fast8_t ColorMapEntrySize;
		} ColorMapSpec;
		struct
		{
			uint_fast16_t XOrigin, YOrigin, Width, Height;
			uint_fast8_t PixelDepth, ImageDescriptor;
		} ImageSpec;
	};

	TargaResolutionError
		TryResolveFully(const void* raw, size_t size, Media::Image& out, Array<Byte>& idOut)
	{
		if (size < 4 + 18) // Size of signature and header.
			return Error_FileTooShort;

		TargaHeader header;
		MemoryReader reader{ raw, size };

		reader.TryRead<uint8_t>(header.ImageIdSize);
		reader.TryRead<uint8_t>(header.ColorMapType);
		reader.TryRead<uint8_t>(header.ImageType);

		if (NoImage == header.ImageType)
			return Success_Perfect;

		reader.TryRead<uint16_t>(header.ColorMapSpec.FirstEntryIndex);
		reader.TryRead<uint16_t>(header.ColorMapSpec.ColorMapLength);
		reader.TryRead<uint8_t>(header.ColorMapSpec.ColorMapEntrySize);

		reader.TryRead<uint16_t>(header.ImageSpec.XOrigin);
		reader.TryRead<uint16_t>(header.ImageSpec.YOrigin);
		reader.TryRead<uint16_t>(header.ImageSpec.Width);
		reader.TryRead<uint16_t>(header.ImageSpec.Height);
		reader.TryRead<uint8_t>(header.ImageSpec.PixelDepth);
		reader.TryRead<uint8_t>(header.ImageSpec.ImageDescriptor);

		// Skip Image ID.
		idOut = Array<Byte>(header.ImageIdSize);
		reader.TryReadMany<Byte>(header.ImageIdSize, idOut.begin());

		// TODO: Indexed color expansion for mapped image.
		auto colorMapSize =
			header.ColorMapSpec.ColorMapLength *
			header.ColorMapSpec.ColorMapEntrySize / 8;
		auto colorMap = Buffer(colorMapSize);

		auto imageSize =
			header.ImageSpec.Width *
			header.ImageSpec.Height *
			header.ImageSpec.PixelDepth / 8;
		out.Data = Buffer(imageSize);
		out.Width = header.ImageSpec.Width;
		out.Height = header.ImageSpec.Height;
		reader.TryReadManyByIndex<Byte>(0, imageSize, out.Data);
		switch (header.ImageType)
		{
		case TrueColor:
			if (24 == header.ImageSpec.PixelDepth)
				out.Format = Media::PixelFormat::Rgb;
			else if (32 == header.ImageSpec.PixelDepth)
				out.Format = Media::PixelFormat::Rgba;
			else return Error_UnsupportedPixelFormat;
			break;
		case GrayScale:
			out.Format = Media::PixelFormat::Alpha;
			break;
		default:
			return Error_UnsupportedPixelFormat;
		}
		return Success_Perfect;
	}

	TargaResolutionError
		TryResolve(const void* raw, size_t size, Media::Image& out)
	{
		Array<Byte> _;
		return TryResolveFully(raw, size, out, _);
	}

	Media::Image
		Resolve(const void* raw, size_t size)
	{
		Media::Image rv;
		if (TryResolve(raw, size, rv).IsError())
			throw std::runtime_error("Unable to resolve TARGA file.");
		return rv;
	}
}

_L_NS_END_FILEFORMAT

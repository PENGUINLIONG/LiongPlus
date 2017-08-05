// File: Png.cpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/Array.hpp"
#include "LiongPlus/FileFormat/Png.hpp"
#include "png.h"

_L_NS_BEG_FILEFORMAT

namespace Png
{
	struct ReaderData
	{
		const Byte* Pos;
		const Byte* End;
	};

	void bufferDataReader(png_structp hReader, png_bytep data, png_size_t size)
	{
		auto raw = png_get_io_ptr(hReader);
		if (raw == nullptr) return;

		auto& rData = *(ReaderData*)raw;
		if (rData.Pos + size > rData.End) return;
		
		memcpy(data, rData.Pos, size);
		rData.Pos += size;
	}
	
	PngResolutionError
		TryResolve(const void* raw, size_t size, Media::Image& out)
	{
		if (size < 8) // Size of signature.
			return Error_FileTooShort;
		if (png_sig_cmp(png_const_bytep(raw), 0, 8)) // 0 on fail.
			return Error_MagicNumberMismatched;

		auto hReader = png_create_read_struct(
			PNG_LIBPNG_VER_STRING,
			// Use the default error handler.
			nullptr, nullptr, nullptr);
		if (hReader == nullptr)
			return Error_InternalError;

		auto hInfo = png_create_info_struct(hReader);
		if (hInfo == nullptr)
		{
			png_destroy_read_struct(&hReader, nullptr, nullptr);
			return Error_InternalError;
		}
		if (setjmp(png_jmpbuf(hReader)))
		{
			png_destroy_read_struct(&hReader, &hInfo, nullptr);
			return Error_InternalError;
		}
		ReaderData rData{ (const Byte*)raw, (const Byte*)raw + size };
		png_set_keep_unknown_chunks(hReader, PNG_HANDLE_CHUNK_NEVER,
			nullptr, 0);
		png_init_io(hReader, png_FILE_p(&rData));
		png_set_read_fn(hReader, &rData, &bufferDataReader);

		// Read PNG header.
		png_read_info(hReader, hInfo);
		png_uint_32 width, height;
		int bitDepth, colorType;
		png_get_IHDR(hReader, hInfo,
			&width, &height, &bitDepth, &colorType,
			nullptr, nullptr, nullptr);
		switch (colorType)
		{
		case PNG_COLOR_TYPE_GRAY:
			out.Format = Media::PixelFormat::Alpha;
			break;
		case PNG_COLOR_TYPE_RGB:
			out.Format = Media::PixelFormat::Rgb;
			break;
		case PNG_COLOR_TYPE_RGBA:
			out.Format = Media::PixelFormat::Rgba;
			break;
		default:
			png_destroy_read_struct(&hReader, &hInfo, nullptr);
			return Error_UnsupportedChannelCount;
		}
		out.Width = width;
		out.Height = height;
		auto rowSize = png_get_rowbytes(hReader, hInfo);
		out.Data = Buffer{ height * rowSize };

		// Read image data.
		auto rowPtr = out.Data.Field();
		for (auto i = 0; i < height; ++i)
		{
			png_read_row(hReader, rowPtr, nullptr);
			rowPtr += rowSize;
		}
		png_read_end(hReader, hInfo);

		// Clean up.
		png_destroy_read_struct(&hReader, &hInfo, nullptr);
		return Success_Perfect;
	}

	Media::Image
		Resolve(const void* raw, size_t size)
	{
		Media::Image rv;
		if (TryResolve(raw, size, rv).IsError())
			throw std::runtime_error("Unable to resolve PNG file.");
		return rv;
	}
}

_L_NS_END_FILEFORMAT

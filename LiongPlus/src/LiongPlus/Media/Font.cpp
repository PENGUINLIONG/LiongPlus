// File: Font.cpp
// Author: Rendong Liang (Liong)
#include <atomic>
#include <vector>
#include <set>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "LiongPlus/Media/Font.hpp"

_L_NS_BEG_MEDIA

namespace details
{
	static std::atomic<size_t> _LibRefCount;
	static FT_Library _Lib;
}

Font::Font() :
	_PxSize(0),
	_Face(nullptr),
	_FontFile()
{
	if (0 == details::_LibRefCount)
	{
		if (FT_Init_FreeType(&details::_Lib))
			throw std::runtime_error(
				"Unable to initialize FreeType library.");
	}
	++details::_LibRefCount;
}
Font::~Font()
{
	if (_Face)
	{
		if (FT_Done_Face((FT_Face)_Face))
			throw std::runtime_error("Unable to finalize font face.");
	}
	--details::_LibRefCount;
	if (0 == details::_LibRefCount)
	{

		if (FT_Done_FreeType(details::_Lib))
			throw std::runtime_error(
				"Unable to finalize FreeType library.");
	}
}

Font::FontError Font::Load(Buffer&& fontFile) noexcept
{
	_FontFile = std::forward<Buffer>(fontFile);
	auto& face = (FT_Face&)_Face;
	if (FT_New_Memory_Face(details::_Lib,
		_FontFile.Field(), _FontFile.Length(), 0, &face))
		return Error_FreeTypeError;
	if (FT_Select_Charmap(face, FT_ENCODING_UNICODE))
	{
		FT_Done_Face(face);
		return Error_FreeTypeError;
	}
	return Success_Perfect;
}

Font::FontError Font::SetPointSize(double pt, int dpi) noexcept
{
	auto face = (FT_Face)_Face;
	_PxSize = std::ceil(pt * dpi);
	if (FT_Set_Char_Size(face, pt, pt, dpi, dpi))
		return Error_FreeTypeError;
	return Success_Perfect;
}
Font::FontError Font::SetPixelSize(int px) noexcept
{
	auto face = (FT_Face)_Face;
	_PxSize = px;
	if (FT_Set_Pixel_Sizes(face, px, px))
		return Error_FreeTypeError;
	return Success_Perfect;
}

int Font::GetPixelSize() noexcept
{
	return _PxSize;
}

Font::FontError Font::RenderGlyph(uint32_t ch,
	Image& out, int& xAdvance) const noexcept
{
	auto face = (FT_Face)_Face;
	auto& glyph = face->glyph;
	auto chIndex = FT_Get_Char_Index(face, ch);
	if (0 == chIndex)
		return Error_GlyphNotFound; // Error_GlyphNotFound
	if (FT_Load_Glyph(face, chIndex,
		FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP))
		return Error_FreeTypeError;
	if (FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL))
		return Error_FreeTypeError;
	
	const auto& bitmap = glyph->bitmap;
	out.Data = Buffer(bitmap.rows * bitmap.width);
	out.Width = _PxSize;
	out.Height = _PxSize;
	out.Format = PixelFormat::Alpha;
	
	auto field = out.Data.Field();
	for (int row = 0; row < bitmap.rows; ++row)
	{
		std::memcpy(field + glyph->bitmap_left +
			(glyph->bitmap_top + row) * out.Width,
			bitmap.buffer, bitmap.width);
	}
	xAdvance = glyph->bitmap_left + glyph->metrics.horiAdvance;
	return Success_Perfect;
}
Font::FontError Font::RenderGlyphMap(uint32_t first,
	size_t row, size_t col,
	Image& out, GlyphMetricMap& metrics) const noexcept
{
	Font::FontErrorCode err = Success_Perfect;

	auto face = (FT_Face)_Face;
	auto& glyph = face->glyph;
	const auto& bitmap = glyph->bitmap;

	// Init image return value.
	out.Data = Buffer(bitmap.rows * bitmap.width);
	out.Width = _PxSize * col;
	out.Height = _PxSize * row;
	out.Format = PixelFormat::Alpha;
	// Init metrics return value.
	metrics.clear();

	for (int y = 0; y < row; ++y)
	{
		for (int x = 0; x < col; ++x)
		{
			auto chIndex =
				FT_Get_Char_Index(face, first + x + y * col);
			if (0 == chIndex)
			{ err = Success_IncompleteMap; continue; }
			if (FT_Load_Glyph(face, chIndex,
				FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP))
				return Error_FreeTypeError;
			if (FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL))
				return Error_FreeTypeError;

			auto field = out.Data.Field();
			auto xPos = x * _PxSize;
			auto yPos = y * _PxSize;
			// Copy each row.
			for (auto r = 0; r < bitmap.rows; ++r)
				std::memcpy(field +
					glyph->bitmap_left + xPos +
					(r + yPos) * out.Width,
					bitmap.buffer + r * bitmap.width, bitmap.width);
			// Set up metrics about the glyph.
			metrics[first + x + y * col] ={
				{ xPos, yPos },
				{ glyph->bitmap_left, glyph->bitmap_top },
				{ glyph->advance.x >> 6, glyph->advance.y >> 6 },
				{ long(glyph->bitmap.width), long(glyph->bitmap.rows) },
			};
		}
	}
	return err;
}

size_t calcMinImageMapSideLength(size_t value)
{
	size_t power = 0;
	while (value != 0)
	{
		value >>= 1;
		++power;
	}
	if (power % 2)
		power += 1;
	return 1 << (power / 2);
}

Font::FontError Font::RenderCharSetGlyphMap(
	const std::set<uint32_t>& charSet,
	Image& out, GlyphMetricMap& metrics) const noexcept
{
	Font::FontErrorCode err = Success_Perfect;

	auto face = (FT_Face)_Face;
	auto side = int(calcMinImageMapSideLength(charSet.size()));

	auto& glyph = face->glyph;
	const auto& bitmap = glyph->bitmap;

	auto sideLen = _PxSize * side;
	out ={ Buffer(sideLen * sideLen),
		sideLen, sideLen, PixelFormat::Alpha };
	out.Data.Wipe();
	metrics.clear();

	int pos = 0;
	auto field = out.Data.Field();
	for (auto ch : charSet)
	{
		auto chIndex = FT_Get_Char_Index(face, ch);
		if (0 == chIndex)
		{ err = Success_IncompleteMap; continue; }
		if (FT_Load_Glyph(face, chIndex,
			FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP))
			return Error_FreeTypeError;
		if (FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL))
			return Error_FreeTypeError;

		auto xPos = pos % side * _PxSize;
		auto yPos = pos / side * _PxSize;
		// Copy each row.
		for (auto r = 0; r < bitmap.rows; ++r)
			std::memcpy(field + xPos + (r + yPos) * out.Width,
				bitmap.buffer + r * bitmap.width, bitmap.width);

		// Set up metrics about the glyph.
		metrics[ch] ={
			{xPos, yPos},
			{glyph->bitmap_left, glyph->bitmap_top},
			{glyph->advance.x >> 6, glyph->advance.y >> 6},
			{long(glyph->bitmap.width), long(glyph->bitmap.rows)},
		};
		++pos;
	}
	return err;
}

Font::FontError Font::ExtendCharSetGlyphMap(
	const std::set<uint32_t>& charSet,
	const std::set<uint32_t>& extensionSet,
	Image& out, GlyphMetricMap& metrics, size_t& charAdded) const noexcept
{
	auto face = (FT_Face)_Face;
	auto preSide = calcMinImageMapSideLength(charSet.size());
	auto newSide = calcMinImageMapSideLength(
		charSet.size() + extensionSet.size());
	if (newSide > preSide)
		return Error_NeedFullUpdate;

	// Find the complement set.
	std::vector<uint32_t> complement;
	for (auto ch : charSet)
	{
		if (extensionSet.find(ch) != extensionSet.end())
			complement.push_back(ch);
	}
	// Initialize return values.
	int width = _PxSize;
	int height = _PxSize * int(complement.size());
	out ={ Buffer{ size_t(width * height) }, width, height, PixelFormat::Alpha };
	out.Data.Wipe();
	metrics.clear();
	FontError err = Success_Perfect;

	int offset = 0;
	auto& glyph = face->glyph;
	auto& bitmap = glyph->bitmap;
	auto field = out.Data.Field();
	for (auto ch : complement)
	{
		auto chIndex = FT_Get_Char_Index(FT_Face(_Face), ch);
		if (0 == chIndex)
		{ err = Success_IncompleteMap; continue; }
		if (FT_Load_Glyph(face, chIndex,
			FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP))
			return Error_FreeTypeError;
		if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
			return Error_FreeTypeError;
		// Copy each row.
		for (auto r = 0; r < bitmap.rows; ++r)
			std::memcpy(field + offset * out.Width,
				bitmap.buffer + r * bitmap.width, bitmap.width);
		// Set up metrics about the glyph.
		metrics[ch] ={
			{ 0, offset },
			{ glyph->bitmap_left, glyph->bitmap_top },
			{ glyph->advance.x >> 6, glyph->advance.y >> 6 },
			{ long(glyph->bitmap.width), long(glyph->bitmap.rows) },
		};
		offset += _PxSize;
	}
	return Success_Perfect;
}

_L_NS_END_MEDIA

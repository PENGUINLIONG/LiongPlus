// File: Font.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <map>
#include <set>
#include "_"
#include "../Array.hpp"
#include "../Buffer.hpp"
#include "Image.hpp"
#include "../SafeError.hpp"

_L_NS_BEG_MEDIA

struct GlyphVector
{
	long X, Y;
};

/*
 * FreeType locate characters in rendering based on distance from the
 *   origin. All the following metrics are in pixels.
 *
 * Fields:
 *   $MapPosition: indicates the position of that char in a glyph map.
 *   $Offset: The upper-left corner position in a glyph map slot. Y
 *     component is height from the base line.
 *   $Advance: The minimum distance to draw another glyph and make the
 *     combination readable.
 *   $Size: Sizes of the enclosed area of a meaningful bitmap.
 */
struct GlyphMetrics
{
	GlyphVector MapPosition;
	GlyphVector Offset;
	GlyphVector Advance;
	GlyphVector Size;
};

using GlyphMetricMap = std::map<uint32_t, GlyphMetrics>;

class Font
{
private:
	int _PxSize;
	void* _Face;
	Buffer _FontFile;

public:
	enum ErrorCode
	{
		Success_Perfect          = 0b0'000'0000,
		Success_IncompleteMap    = 0b0'000'0001,

		Error_FreeTypeError      = 0b1'000'0001,
		Error_GlyphNotFound      = 0b1'000'0010,
		Error_Mask               = 0b1'000'0000,
	};
	using Error = SafeError<Font::ErrorCode, uint_fast8_t>;

	Font();
	~Font();

	Error Load(Buffer&& buffer) noexcept;
	Error SetPointSize(double pt, int dpi) noexcept;
	Error SetPixelSize(int px) noexcept;
	int GetPixelSize() noexcept;
	/*
	 * Render a single glyph.
	 * Params:
	 *   $ch: Unicode char to be rendered.
	 *   $out: Output image returned-by-reference. The pixel format is
	 *     always [PixelFormat::Alpha].
	 *   $xAdvance: The total advancement to draw the next glyph
	 *     returned-by-reference.
	 * Returns:
	 *   Font error indicating if rendering was successfully finished.
	 */
	Error RenderGlyph(uint32_t ch,
		Image& out, int& xAdvance) const noexcept;
	/*
	 * Render a glyph map. The glyphs and x-advancement will be
	 *   arranges first through x-direction then y-direction. Glyph
	 *   that are not supported by the current fontface's charset will
	 *   be ignored, and the error code of [Error_IncompleteMap] will
	 *   be returned. Glyphs representing chars from $first to
	 *   ($first + $col * $row) will be rendered.
	 * Params:
	 *   $first: The first char to be rendered. It will show up at
	 *     slot (0, 0).
	 *   $row: The number of rows to be rendered.
	 *   $col: The number of cols to be redered.
	 *   $out: Output image returned-by-reference. The pixel format is
	 *     always [PixelFormat::Alpha].
	 *   $xAdvance: The total advancement to draw the next glyph
	 *     returned-by-reference for each.
	 */
	Error RenderGlyphMap(uint32_t first, size_t row, size_t col,
		Image& out, GlyphMetricMap& metrics) const noexcept;
	Font::Error RenderCharSetGlyphMap(
		const std::set<uint32_t>& charSet,
		Image& out, GlyphMetricMap& metrics) const noexcept;
};


_L_NS_END_MEDIA

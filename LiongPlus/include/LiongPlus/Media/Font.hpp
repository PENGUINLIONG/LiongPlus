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

/*
 * Provide font rendering (rasterization) functionality, using
 *   FreeType.
 *
 * Note:
 *   When you are going to make an animation effect that changes the
 *   glyphs' size, it's prefered to perform transformations on
 *   rendered glyphs rather than re-render the glyphs to different
 *   size. The latter one will lead to serious performance issue.
 */
class Font
{
private:
	int _PxSize;
	void* _Face;
	Buffer _FontFile;

public:
	enum FontErrorCode
	{
		Success_Perfect          = 0b0'000'0000,
		// Indicate that there are characters not included in the font.
		Success_IncompleteMap    = 0b0'000'0001,

		// There is an error thrown from FreeType. Typically, it's because of a wrong invokation order.
		Error_FreeTypeError      = 0b1'000'0001,
		// Unable to find a glyph. This error is thrown when and only when RenderGlyph() is called, and a glyph is not included in the font.
		Error_GlyphNotFound      = 0b1'000'0010,
		// Indicate that the output map image's size has changed. The entire map should be updated.
		Error_NeedFullUpdate     = 0b0'000'0011,
		Error_Mask               = 0b1'000'0000,
	};
	using FontError = SafeError<Font::FontErrorCode, uint_fast8_t>;

	Font();
	~Font();

	FontError Load(Buffer&& buffer) noexcept;
	FontError SetPointSize(double pt, int dpi) noexcept;
	FontError SetPixelSize(int px) noexcept;
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
	FontError RenderGlyph(uint32_t ch,
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
	FontError RenderGlyphMap(uint32_t first, size_t row, size_t col,
		Image& out, GlyphMetricMap& metrics) const noexcept;
	/*
	 * Render a glyph map based on $charset. The output image is a
	 *   square map of chars. Any empty slot is filled with 0x00.
	 *
	 * Params:
	 *   $charSet: Set of chars that will be drawn.
	 *   $out: Output image returned-by-reference. The pixel format is
	 *     always [PixelFormat::Alpha].
	 *   $metrics: Glyph metric information returned-by-reference.
	 */
	Font::FontError RenderCharSetGlyphMap(
		const std::set<uint32_t>& charSet,
		Image& out, GlyphMetricMap& metrics) const noexcept;
	/*
	 * Extend a glyph map based on complement set. The output image is
	 *   linearly arranged, that is arranged by a single column but
	 *   many rows. The output image will not contain any existing
	 *   char in $charSet. It's decided by the user how chars are
	 *   updated.
	 * If $charSet's corresponding image will be unable to contain the
	 *   extension Set directly, an error [Error_NeedFullUpdate] is
	 *   returned and invalid $image and $metrics are rendered.
	 *
	 * Params:
	 *   $charSet: Set of chars that exists in a glyph map already
	 *     renderd before.
	 *   $extensionSet: Set of chars that will be drawn as complement.
	 *   $out: Output image returned-by-reference. The pixel format is
	 *     always [PixelFormat::Alpha].
	 *   $metrics: Glyph metric information returned-by-reference.
	 */
	Font::FontError ExtendCharSetGlyphMap(
		const std::set<uint32_t>& charSet,
		const std::set<uint32_t>& extensionSet,
		Image& out, GlyphMetricMap& metrics, size_t& charAdded) const noexcept;
};


_L_NS_END_MEDIA

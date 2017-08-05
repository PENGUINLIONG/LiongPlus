// File: OpenGLBrush.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "../../Array.hpp"
#include "Brush.hpp"
#include "../../Media/Color.hpp"
#include "../../Media/Image.hpp"
#include "../../Media/Font.hpp"
#include "../OpenGL/ShaderProgram.hpp"
#include "../OpenGL/Texture.hpp"
#include "../OpenGL/BufferObject.hpp"
#include "UIFactory.hpp"

_L_NS_BEG_GRAPHICS_UI

class OpenGLBrush :
	public UI::Brush
{
protected:
	OpenGL::ArrayBuffer _Vbo;
	OpenGL::UniformBuffer _Ubo;
	unsigned _Mode;

public:
	OpenGLBrush();

	void SetColor(const Vector4& color);
	void SetMode(unsigned mode);
	void SetTransform(const Matrix4x4& transform) override;
};

using IOpenGLBrush = I<OpenGLBrush>;

class SolidBrush :
	public OpenGLBrush,
	public DependencyObject<SolidBrush>
{
protected:
	void _UpdateColorOnDraw();

public:
	static const UIProperty
		ColorProperty;

	SolidBrush();

	Vector4 GetColor() const;
	void SetColor(const Vector4& color);

	void Paint(const Array<Point>& points) override;
};

template<>
struct UIFactory<SolidBrush>
{
	static std::shared_ptr<SolidBrush> Create();
	static std::shared_ptr<SolidBrush> Create(const Vector4& color);
	static std::shared_ptr<SolidBrush> Create(const Media::Color& color);
};

class ImageBrush :
	public SolidBrush
{
protected:
	OpenGL::Texture2D _Texture;

public:
	ImageBrush();

	void SetImage(const Media::Image& buffer);

	void Paint(const Array<Point>& points) override;
};

template<>
struct UIFactory<ImageBrush>
{
	static std::shared_ptr<ImageBrush> Create();
	static std::shared_ptr<ImageBrush> Create(const Media::Image& image);
};

/*
 * NEVER INSTANCIATE THIS TYPE BY YOUR SELF.
 *
 * TextBrush is specialized in rendering font glyphs. All input
 *   strings are translated Unicodes. TextBrush is not recommended to
 *   be used by controls other than [TextBlock] as foreground brush.
 *
 * Usage:
 *   Before being used by canvas, or otherwise, it's necessary to
 *     follow the following procedure to fully initialize [TextBrush].
 *     It might be cleaner to do so with [UIFactory].
 *   1. Call SetFont() and set a valid brush.
 *   2. Call BuildCache() and build up cache for all characters that
 *     might be used. The process is relatively costly so it's
 *     recommended not to rebuild frequently.
 *   3. Call SetText() and provide the text will be rendered.
 *
 *   On each draw,
 *   4. Call Paint() and render the texts. Mode property will be ignored.
 */
class TextBrush
	: public ImageBrush
{
protected:
	Media::GlyphMetricMap _Metrics;
	std::u32string _Unicodes;
	std::shared_ptr<Media::Font> _Font;
	float _MapWidth;
	float _MapHeight;

public:
	const Media::GlyphMetricMap& GetCachedMetrics() const;
	/*
	* Before calling SetText(), this method should be called to build
	*   up font cache.
	* Params:
	*   $unicodes: Unicode string that contains all the chars that
	*     will be used in the future.
	*/
	void BuildCache(const std::u32string& unicodes);

	const std::shared_ptr<Media::Font>& GetFont() const;
	void SetFont(const std::shared_ptr<Media::Font>& font);

	const std::u32string& GetText() const;
	/*
	* This method process the $str and store the texture coordinate
	*   of each char in glyph map. The texture coordinates will then
	*   be used to draw texts. Notice that the method *only* process
	*   the texture coordinates. Positioning of each character in
	*   viewport should be done when you call Rectangles.
	*/
	void SetText(const std::u32string& unicodes);
	void Paint(const Array<Point>& points);
};

template<>
struct UIFactory<TextBrush>
{
	static std::shared_ptr<TextBrush> Create();
};

_L_NS_END_GRAPHICS_OPENGL

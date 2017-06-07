// File: OpenGLBrush.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <vector>
#include "_"
#include "Brush.hpp"
#include "../../Media/Image.hpp"
#include "../../Media/Font.hpp"
#include "../OpenGL/ShaderProgram.hpp"
#include "UIFactory.hpp"

_L_NS_BEG_GRAPHICS_UI

class OpenGLBrush :
	public UI::Brush
{
protected:
	GLuint _Mode;
	GLuint _TransformUbo;
	std::shared_ptr<OpenGL::ShaderProgram> _Program;
	Rect _ClientArea;

public:
	OpenGLBrush(const std::shared_ptr<OpenGL::ShaderProgram>& prog);

	void SetMode(GLuint mode);
	void SetTransform(const Matrix4x4& transform) override;
	void SetBoundary(const Rect& clientArea) override;
};

using IOpenGLBrush = I<OpenGLBrush>;

class SolidBrush :
	public OpenGLBrush,
	public DependencyObject<SolidBrush>
{
protected:
	GLuint _Vao, _SolidSettingsUbo,
		_VertexPosLoc, _VertexPosVbo;

public:
	static const UIProperty
		ColorProperty;

	SolidBrush();

	Vector4 GetColor() const;
	void SetColor(const Vector4& color);

	void Paint(const float* points, size_t count) override;
};

template<>
struct UIFactory<SolidBrush>
{
	static std::shared_ptr<SolidBrush> Create();
	static std::shared_ptr<SolidBrush> Create(const Vector4& color);
};

class ImageBrush :
	public OpenGLBrush,
	public DependencyObject<ImageBrush>
{
protected:
	GLuint _Vao, _Vbo,
		_Texture, _TextureLoc,
		_VertexPosLoc, _TexturePosLoc;

public:
	static const UIProperty
		ImageProperty;
	ImageBrush();

	Buffer GetImage() const;
	void SetImage(const Media::Image& buffer);

	void Paint(const float* points, size_t count) override;
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
	std::vector<uint32_t> _Unicodes;
	std::shared_ptr<Media::Font> _Font;
	float _MapWidth;
	float _MapHeight;

public:
	/*
	* Before calling SetText(), this method should be called to build
	*   up font cache.
	* Params:
	*   $str: String that contains all the chars that will be used in
	*     the future.
	*/
	const Media::GlyphMetricMap& GetCachedMetrics() const;
	void BuildCache(const std::vector<uint32_t>& unicodes);

	const std::shared_ptr<Media::Font>& GetFont() const;
	void SetFont(const std::shared_ptr<Media::Font>& font);

	const std::vector<uint32_t>& GetText() const;
	/*
	* This method process the $str and store the texture coordinate
	*   of each char in glyph map. The texture coordinates will then
	*   be used to draw texts. Notice that the method *only* process
	*   the texture coordinates. Positioning of each character in
	*   viewport should be done when you call Rectangles.
	*/
	void SetText(const std::vector<uint32_t>& unicodes);
	void Paint(const float* chars, size_t count);
};

_L_NS_END_GRAPHICS_OPENGL

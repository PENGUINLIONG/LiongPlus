// File: OpenGLBrush.cpp
// Author: Rendong Liang (Liong)
#include <utility>
#include <map>
#include <memory>
#include "LiongPlus/Graphics/UI/OpenGLBrush.hpp"
#include "LiongPlus/BufferPool.hpp"
#include "LiongPlus/Media/Font.hpp"
#include "LiongPlus/Graphics/OpenGL/BufferObject.hpp"
#include "LiongPlus/Graphics/OpenGL/Texture.hpp"

_L_NS_BEG_GRAPHICS_UI

using namespace std;
using namespace OpenGL;
using namespace Media;

namespace details
{
	inline auto BrushVertexShader()
	{
		return
#include "OpenGLBrushVertexShader.glsl"
			;
	}
	inline auto BrushFragmentShader()
	{
		return
#include "OpenGLBrushFragmentShader.glsl"
			;
	}

	struct BrushSettings
	{
		float transform[16];
		float color[4];
	};

	struct BrushShaderProgramKit
	{
		ShaderProgram Program;
		VertexArray Vao;
		unsigned UboIndex;
		unsigned TexLoc;
		Texture<TextureType::Texture2D> WhiteTex;
	};

	auto buildBrushShaderProgram(ShaderProgram& prog)
	{
		Shader vert(ShaderType::Vertex),
			frag(ShaderType::Fragment);
		vert.Compile(BrushVertexShader());
		frag.Compile(BrushFragmentShader());
		prog.Attach(vert);
		prog.Attach(frag);
		prog.Link();
		prog.Detach(vert);
		prog.Detach(frag);
	}

	auto getBrushShaderProgram()
	{
		thread_local static shared_ptr<BrushShaderProgramKit> kit;
		if (kit == nullptr)
		{
			kit = make_shared<BrushShaderProgramKit>();
			auto& kitRef = *kit;
			buildBrushShaderProgram(kitRef.Program);
			// Initialize VAO.
			kitRef.Vao = kitRef.Program.InitVertexArray();
			// A white texture can help render faster.
			auto& tex = kit->WhiteTex;
			Buffer texBuf(4 * 4 * 3);
			memset(texBuf.Field(), 0xFF, texBuf.Length());
			tex.Allocate(PixelFormat::Rgb, 4, 4);
			tex.Write(texBuf.Field(), PixelFormat::Rgb);
			kitRef.TexLoc = kitRef.Program.GetUniformLocation("tex");
			kitRef.UboIndex = kitRef.Program.GetUniformBlockIndex("BufferSettings");
			kitRef.Program.SetSampler(kitRef.TexLoc, 0);
		}
		return kit;
	}

	void setBrushAttributes(VertexArray& vao, const ArrayBuffer& arrBuf)
	{
		vao.SetAttributes({
			// Vertex position.
			{ arrBuf, 0, 2, 0, 4 * sizeof(float) },
			// Texture coordinates.
			{ arrBuf, 1, 2, 2 * sizeof(float), 4 * sizeof(float) }
		});
	}

}


//
// OpenGLBrush
//

OpenGLBrush::OpenGLBrush() :
	_Vbo(),
	_Ubo(details::getBrushShaderProgram()->Program.InitUniformBlock("BrushSettings", nullptr)),
	_Mode(0)
{
	float cleanBuf[] ={ 1.f, 1.f, 1.f, 1.f };
	_Ubo.Write(cleanBuf, 4 * 4 * sizeof(float), sizeof(cleanBuf));
	SetTransform(MatrixTransform::Identity());
}

void OpenGLBrush::SetMode(unsigned mode)
{
	_Mode = mode;
}
void OpenGLBrush::SetTransform(const Matrix4x4& transform)
{
	auto mapping = _Ubo.Map();
	float* trans = mapping;
	trans[0] = transform(0, 0);
	trans[1] = transform(1, 0);
	trans[2] = transform(2, 0);
	trans[3] = transform(3, 0);
	trans[4] = transform(0, 1);
	trans[5] = transform(1, 1);
	trans[6] = transform(2, 1);
	trans[7] = transform(3, 1);
	trans[8] = transform(0, 2);
	trans[9] = transform(1, 2);
	trans[10] = transform(2, 2);
	trans[11] = transform(3, 2);
	trans[12] = transform(0, 3);
	trans[13] = transform(1, 3);
	trans[14] = transform(2, 3);
	trans[15] = transform(3, 3);
}

//
// SolidBrush
//

const UIProperty
SolidBrush::ColorProperty = UIProperty::MakeDefault<Vector4>("Color", UIActionTrigger::Render).Register<SolidBrush>();

SolidBrush::SolidBrush()
{
	SetColor({ 1.f, 1.f, 1.f, 1.f });
}

void SolidBrush::_UpdateColorOnDraw()
{
	auto color = GetColor();
	auto mapping = _Ubo.Map(4 * 4 * sizeof(float), 4 * sizeof(float));
	float* mapped = mapping;
	*(mapped++) = color.R;
	*(mapped++) = color.G;
	*(mapped++) = color.B;
	*(mapped++) = color.A;
}

Vector4 SolidBrush::GetColor() const
{
	return GetProperty<Vector4>(ColorProperty);
}
void SolidBrush::SetColor(const Vector4& color)
{
	SetProperty(ColorProperty, color);
}

void SolidBrush::Paint(const Array<Point>& points)
{
	auto& kit = *details::getBrushShaderProgram();
	// 2 for vertex position and 2 for texture coords.
	_Vbo.AllocateResizable(points.size() * (2 + 2) * sizeof(float),
		BufferAccessPattern::OneOff);
	// Translate verticies.
	{
		auto mapping = _Vbo.Map();
		float* field = mapping;
		for (auto& point : points)
		{
			*(field++) = point.X;
			*(field++) = point.Y;
			*(field++) = 0.;
			*(field++) = 0.;
		}
	}
	_UpdateColorOnDraw();
	// Draw.
	auto progAct = kit.Program.Activate();
	auto texBind = kit.WhiteTex.Bind(0);
	auto uniBind = _Ubo.BindIndex(0);
	details::setBrushAttributes(kit.Vao, _Vbo);
	kit.Vao.Draw(_Mode, 0, points.size());
}

std::shared_ptr<SolidBrush> UIFactory<SolidBrush>::Create()
{
	return std::make_shared<SolidBrush>();
}
std::shared_ptr<SolidBrush> UIFactory<SolidBrush>::Create(const Vector4& color)
{
	auto ptr = std::make_shared<SolidBrush>();
	ptr->SetColor(color);
	return ptr;
}
std::shared_ptr<SolidBrush> UIFactory<SolidBrush>::Create(const Media::Color& color)
{
	auto ptr = std::make_shared<SolidBrush>();
	ptr->SetColor(Media::ColorUtils::ToFloatColor(color));
	return ptr;
}

//
// ImageBrush
//

ImageBrush::ImageBrush() :
	_Texture()
{
}

void ImageBrush::SetImage(const Media::Image& image)
{
	_Texture.Allocate(image.Format, image.Width, image.Height);
	_Texture.Write(image.Data.Field(), image.Format);
}

void ImageBrush::Paint(const Array<Point>& points)
{
	auto& kit = *details::getBrushShaderProgram();
	// Find the boundary of incoming geometry.
	float left = 0., top = 0., right = 0., bottom = 0.;
	auto count = points.size();
	while (--count)
	{
		auto& point = points[count];
		if (left > point.X) left = point.X;
		else if (right < point.X) right = point.X;
		if (top > point.Y) top = point.Y;
		else if (bottom < point.Y) bottom = point.Y;
	}
	auto width = right - left;
	auto height = bottom - top;
	_Vbo.AllocateResizable(points.size() * (2 + 2) * sizeof(float),
		BufferAccessPattern::OneOff);
	// Normalize to Texture coordination.
	{
		auto mapping = _Vbo.Map();
		float* field = mapping;
		for (auto& point : points)
		{
			*(field++) = point.X;
			*(field++) = point.Y;
			*(field++) = (point.X - left) / width;
			*(field++) = (point.Y - top) / height;
		}
	}
	_UpdateColorOnDraw();
	auto progAct = kit.Program.Activate();
	auto texBind = _Texture.Bind(0);
	auto uniBind = _Ubo.BindIndex(0);
	details::setBrushAttributes(kit.Vao, _Vbo);
	kit.Vao.Draw(_Mode, 0, points.size());
}

shared_ptr<ImageBrush> UIFactory<ImageBrush>::Create()
{
	return make_shared<ImageBrush>();
}
shared_ptr<ImageBrush> UIFactory<ImageBrush>::Create(const Media::Image& image)
{
	auto ptr = make_shared<ImageBrush>();
	ptr->SetImage(image);
	return ptr;
}

//
// TextBrush
//

const Media::GlyphMetricMap& TextBrush::GetCachedMetrics() const
{
	return _Metrics;
}
void TextBrush::BuildCache(const std::u32string& unicodes)
{
	Media::Image glyphMap;
	auto& charSet = std::set<uint32_t>(
		unicodes.begin(), unicodes.end());
	if (!_Font->RenderCharSetGlyphMap(
		charSet, glyphMap, _Metrics).IsSuccess())
		throw std::runtime_error("Unable to render glyphs.");
	_MapWidth = glyphMap.Width;
	_MapHeight = glyphMap.Height;
	SetImage(glyphMap);
}

const std::shared_ptr<Media::Font>& TextBrush::GetFont() const
{
	return _Font;
}
void TextBrush::SetFont(const std::shared_ptr<Media::Font>& font)
{
	_Font = font;
}

const std::u32string& TextBrush::GetText() const
{
	return _Unicodes;
}
void TextBrush::SetText(const std::u32string& unicodes)
{
	_Unicodes = unicodes;
}

void TextBrush::Paint(const Array<Point>& points)
{
	auto& kit = *details::getBrushShaderProgram();
	_Vbo.AllocateResizable(points.size() * (2 + 2) * sizeof(float),
		BufferAccessPattern::OneOff);
	auto pxSize = _Font->GetPixelSize();
	// Normalize to Texture coordination.
	{
		auto mapping = _Vbo.Map();
		float* field = mapping;
		size_t i = 0;
		for (auto& point : points)
		{
			// Vertex coordinates.
			*(field++) = point.X;
			*(field++) = point.Y;
			// Glyph coordinates varies overtime.
			auto& metrics = _Metrics.at(_Unicodes.at(i / 6));
			switch (i % 6)
			{
				// Upper-right.
				case 0:
				case 5:
					*(field++) = (metrics.MapPosition.X + pxSize)
						/ _MapWidth;
					*(field++) = (_MapHeight - metrics.MapPosition.Y)
						/ _MapHeight;
					break;
				// Upper-left.
				case 1:
					*(field++) = metrics.MapPosition.X
						/ _MapWidth;
					*(field++) = (_MapHeight - metrics.MapPosition.Y)
						/ _MapHeight;
					break;
				// Lower-left.
				case 2:
				case 3:
					*(field++) = metrics.MapPosition.X
						/ _MapWidth;
					*(field++) =
						(_MapHeight - metrics.MapPosition.Y - pxSize)
						/ _MapHeight;
					break;
				// Lower-right.
				case 4:
					*(field++) = (metrics.MapPosition.X + pxSize)
						/ _MapWidth;
					*(field++) =
						(_MapHeight - metrics.MapPosition.Y - pxSize)
						/ _MapHeight;
					break;
			}
			++i;
		}
	}
	_UpdateColorOnDraw();
	auto progAct = kit.Program.Activate();
	auto texBind = _Texture.Bind(0);
	auto uniBind = _Ubo.BindIndex(0);
	details::setBrushAttributes(kit.Vao, _Vbo);
	kit.Vao.Draw(_Mode, 0, points.size());
}

shared_ptr<TextBrush> UIFactory<TextBrush>::Create()
{
	return make_shared<TextBrush>();
}

_L_NS_END_GRAPHICS_UI

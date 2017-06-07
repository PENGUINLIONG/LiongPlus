#include <utility>
#include <map>
#include <memory>
#include "OpenGLBrush.hpp"
#include "../../BufferPool.hpp"
#include "../../Media/Font.hpp"

_L_NS_BEG_GRAPHICS_UI

using namespace std;
using namespace OpenGL;

namespace details
{
	inline auto SolidVertexShader()
	{
		return "#version 330 core\n"
			"layout(std140) uniform Transform {"
			"   mat4 transform;"
			"};"
			"layout(location = 0) in vec2 vertexPos;"
			"void main()"
			"{"
			"   gl_Position = transform * vec4(vertexPos, 0., 1.);"
			"}";
	}
	inline auto SolidFragmentShader()
	{
		return "#version 330 core\n"
			"layout(std140) uniform SolidSettings {"
			"	vec4 solidColor;"
			"};"
			"out vec4 colorOut;"
			"void main()"
			"{"
			"	colorOut = solidColor;"
			"}";
	}

	inline auto ImageVertexShader()
	{
		return "#version 330 core\n"
			"layout(std140) uniform Transform {\n"
			"	mat4 transform;\n"
			"};\n"
			"layout(location = 0) in vec2 vertexPos;\n"
			"layout(location = 1) in vec2 texturePos;\n"
			"out vec2 texturePosAdj;\n"
			"void main()\n"
			"{\n"
			"	gl_Position = transform * vec4(vertexPos, 0., 1.);\n"
			"	texturePosAdj = vec2(texturePos.x, 1. - texturePos.y);\n"
			"}\n";
	}
	inline auto ImageFragmentShader()
	{
		return "#version 330 core\n"
			"uniform sampler2D tex;\n"
			"in vec2 texturePosAdj;\n"
			"out vec4 colorOut;\n"
			"void main()\n"
			"{\n"
			"	colorOut = texture(tex, texturePosAdj);\n"
			"}\n";
	}

	shared_ptr<ShaderProgram> getSolidBrushShaderProgram()
	{
		thread_local static shared_ptr<ShaderProgram> prog;
		if (prog == nullptr)
		{
			prog = make_shared<ShaderProgram>();
			Shader vert(ShaderType::Vertex),
				frag(ShaderType::Fragment);
			vert.Compile(SolidVertexShader());
			frag.Compile(SolidFragmentShader());
			prog->Attach(vert);
			prog->Attach(frag);
			prog->Link();
			prog->Detach(vert);
			prog->Detach(frag);
		}
		return prog;
	}

	shared_ptr<ShaderProgram> getImageBrushShaderProgram()
	{
		thread_local static shared_ptr<ShaderProgram> prog;
		if (prog == nullptr)
		{
			prog = make_shared<ShaderProgram>();
			Shader vert(ShaderType::Vertex),
				frag(ShaderType::Fragment);
			vert.Compile(ImageVertexShader());
			frag.Compile(ImageFragmentShader());
			prog->Attach(vert);
			prog->Attach(frag);
			prog->Link();
			prog->Detach(vert);
			prog->Detach(frag);
		}
		return prog;
	}

	inline size_t countPixelSize(Media::PixelFormat format) noexcept
	{
		using Fmt = Media::PixelFormat;
		switch (format)
		{
		case Fmt::Alpha:
		case Fmt::Red:
		case Fmt::Green:
		case Fmt::Blue:
			return 1;
		case Fmt::Rgb:
		case Fmt::Bgr:
			return 3;
		case Fmt::Rgba:
			return 4;
		}
	}
	inline GLenum toGLPixelFormat(Media::PixelFormat format) noexcept
	{
		switch (countPixelSize(format))
		{
		case 1: return GL_RED;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		}
	}

	inline void getRearrangedSwizzle(Media::PixelFormat format, int swizzle[4])
	{
		switch (format)
		{
			case LiongPlus::Media::Alpha:
				swizzle[0] = GL_ONE;
				swizzle[1] = GL_ONE;
				swizzle[2] = GL_ONE;
				swizzle[3] = GL_RED;
				break;
			case LiongPlus::Media::Red:
				swizzle[0] = GL_RED;
				swizzle[1] = GL_ZERO;
				swizzle[2] = GL_ZERO;
				swizzle[3] = GL_ONE;
				break;
			case LiongPlus::Media::Green:
				swizzle[0] = GL_ZERO;
				swizzle[1] = GL_RED;
				swizzle[2] = GL_ZERO;
				swizzle[3] = GL_ONE;
				break;
			case LiongPlus::Media::Blue:
				swizzle[0] = GL_ZERO;
				swizzle[1] = GL_ZERO;
				swizzle[2] = GL_RED;
				swizzle[3] = GL_ONE;
				break;
			case LiongPlus::Media::Rgb:
				swizzle[0] = GL_RED;
				swizzle[1] = GL_GREEN;
				swizzle[2] = GL_BLUE;
				swizzle[3] = GL_ONE;
				break;
			case LiongPlus::Media::Bgr:
				swizzle[0] = GL_BLUE;
				swizzle[1] = GL_GREEN;
				swizzle[2] = GL_RED;
				swizzle[3] = GL_ONE;
				break;
			case LiongPlus::Media::Rgba:
				swizzle[0] = GL_RED;
				swizzle[1] = GL_GREEN;
				swizzle[2] = GL_BLUE;
				swizzle[3] = GL_ALPHA;
				break;
			default:
				break;
		}
	}

	inline void rearrangePixel(GLuint texture, Media::PixelFormat format)
	{
		int swizzle[4];
		getRearrangedSwizzle(format, swizzle);
		glTextureParameteriv(texture, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
	}

	inline void rearrangePixelLegacy(GLuint target,
		Media::PixelFormat format)
	{
		int swizzle[4];
		getRearrangedSwizzle(format, swizzle);
		glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
	}
}


//
// OpenGLBrush
//

OpenGLBrush::OpenGLBrush(const shared_ptr<ShaderProgram>& program) :
	_Mode(GL_POINTS),
	_Program(program),
	_TransformUbo()
{
	_TransformUbo = _Program->InitUniformBlock("Transform", 0);
	SetTransform(Matrix4x4::Identity());
}

void OpenGLBrush::SetMode(GLuint mode)
{
	_Mode = mode;
}
void OpenGLBrush::SetTransform(const Matrix4x4& transform)
{
	float trans[16];
	for (auto i = 0; i < 16; ++i)
		trans[i] = reinterpret_cast<const Float*>(transform.Elements)[i];
	glBindBuffer(GL_UNIFORM_BUFFER, _TransformUbo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(trans), trans);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLBrush::SetBoundary(const Rect& clientArea)
{
	_ClientArea = clientArea;
}

//
// SolidBrush
//

const UIProperty
SolidBrush::ColorProperty = UIProperty::MakeDefault<Vector4>("Color", UIActionTrigger::Render).Register<SolidBrush>();

SolidBrush::SolidBrush() :
	OpenGLBrush(details::getSolidBrushShaderProgram()),
	_Vao(),
	_SolidSettingsUbo(),
	_VertexPosLoc(),
	_VertexPosVbo()
{
	_SolidSettingsUbo = _Program->InitUniformBlock("SolidSettings", 1);
	_VertexPosLoc = _Program->GetAttributeLocation("vertexPos");
	glGenVertexArrays(1, &_Vao);
	glGenBuffers(1, &_VertexPosVbo);
	glBindVertexArray(_Vao);
	glEnableVertexAttribArray(_VertexPosLoc);
	glBindVertexArray(0);
}

Vector4 SolidBrush::GetColor() const
{
	return GetProperty<Vector4>(ColorProperty);
}

void SolidBrush::SetColor(const Vector4& color)
{
	using SolidSettings = struct {
		float R, G, B, A;
	};
	SolidSettings settings ={
		float(color.X), float(color.Y), float(color.Z), float(color.W)
	};
	glBindBuffer(GL_UNIFORM_BUFFER, _SolidSettingsUbo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(settings), &settings);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	SetProperty(ColorProperty, color);
}

void SolidBrush::Paint(const float* points, size_t count)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, _TransformUbo);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, _SolidSettingsUbo);

	auto size = count * 2 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, _VertexPosVbo);
	glBufferData(GL_ARRAY_BUFFER, size, points, GL_STREAM_DRAW);

	glBindVertexArray(_Vao);
	glVertexAttribPointer(_VertexPosLoc, 2, GL_FLOAT, false,
		2 * sizeof(float), 0);

	_Program->Shade(_Mode, count);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

//
// ImageBrush
//

const UIProperty
ImageBrush::ImageProperty = UIProperty::MakeDefault<Media::Image>(
	"Image", UIActionTrigger::Render).Register<ImageBrush>();

ImageBrush::ImageBrush() :
	OpenGLBrush(details::getImageBrushShaderProgram()),
	_Vao(),
	_Vbo(),
	_Texture(),
	_TextureLoc(),
	_VertexPosLoc(),
	_TexturePosLoc()
{
	glGenTextures(1, &_Texture);

	glGenBuffers(1, &_Vbo);
	glGenVertexArrays(1, &_Vao);

	_TextureLoc = _Program->GetUniformLocation("tex");
	glUseProgram(_Program->GetHandle());
	{
		glUniform1i(_TextureLoc, _TextureLoc);
	}
	glUseProgram(0);

	glBindVertexArray(_Vao);
	{
		_VertexPosLoc = _Program->GetAttributeLocation("vertexPos");
		glEnableVertexAttribArray(_VertexPosLoc);
		_TexturePosLoc = _Program->GetAttributeLocation("texturePos");
		glEnableVertexAttribArray(_TexturePosLoc);
	}
	glBindVertexArray(0);
}

Buffer ImageBrush::GetImage() const
{
	return GetProperty<Buffer>(ImageProperty);
}
void ImageBrush::SetImage(const Media::Image& image)
{
	SetProperty(ImageProperty, image);

	glBindTexture(GL_TEXTURE_2D, _Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
		image.Width, image.Height, 0,
		details::toGLPixelFormat(image.Format), GL_UNSIGNED_BYTE,
		image.Data.Field());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	details::rearrangePixelLegacy(GL_TEXTURE_2D, image.Format);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageBrush::Paint(const float* points, size_t count)
{
	float left = 0., top = 0., right = 0., bottom = 0.;
	for (size_t i = 0; i < count; ++i)
	{
		auto x = points[i * 2];
		if (left > x) left = x;
		else if (right < x) right = x;
		auto y = points[i * 2 + 1];
		if (top > y) top = y;
		else if (bottom < y) bottom = y;
	}
	auto width = right - left;
	auto height = bottom - top;
	// Normalize to Texture coordination.
	auto size = count * 4 * sizeof(float);
	auto ref = BufferPool::Ginst().Get(size);
	auto data = (float*)ref->Field();
	for (size_t i = 0; i < count; ++i)
	{
		// Vertex coordinates.
		data[i * 4] = points[i * 2];
		data[i * 4 + 1] = points[i * 2 + 1];
		// Texture coordinates.
		data[i * 4 + 2] = (points[i * 2] - left) / width;
		data[i * 4 + 3] = (points[i * 2 + 1] - top) / height;
	}
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, _TransformUbo);

	glBindBuffer(GL_ARRAY_BUFFER, _Vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);

	glBindVertexArray(_Vao);
	glVertexAttribPointer(_VertexPosLoc, 2, GL_FLOAT, false,
		4 * sizeof(float), 0);
	glVertexAttribPointer(_TexturePosLoc, 2, GL_FLOAT, false,
		4 * sizeof(float), (void*)(2 * sizeof(float)));

	glActiveTexture(GL_TEXTURE0 + _TextureLoc);
	glBindTexture(GL_TEXTURE_2D, _Texture);

	_Program->Shade(_Mode, count);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
void TextBrush::BuildCache(const std::vector<uint32_t>& unicodes)
{
	Media::Image glyphMap;
	auto charSet = std::set<uint32_t>(
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

const std::vector<uint32_t>& TextBrush::GetText() const
{
	return _Unicodes;
}
void TextBrush::SetText(const std::vector<uint32_t>& unicodes)
{
	_Unicodes = unicodes;
}

void TextBrush::Paint(const float* points, size_t count)
{
	auto vertCount = count * 4;
	auto bufSize = vertCount * sizeof(float);
	auto ref = BufferPool::Ginst().Get(bufSize);
	auto field = (float*)ref->Field();
	auto pxSize = _Font->GetPixelSize();
	for (int i = 0; i < count; ++i)
	{
		// Vertex coordinates.
		field[i * 4] = points[i * 2];
		field[i * 4 + 1] = points[i * 2 + 1];
		// Glyph coordinates varies overtime.
		auto& metrics = _Metrics.at(_Unicodes.at(i / 6));
		switch (i % 6)
		{
			// Upper-right.
			case 0:
			case 5:
				field[i * 4 + 2] = (metrics.MapPosition.X + pxSize)
					/ _MapWidth;
				field[i * 4 + 3] = (_MapHeight - metrics.MapPosition.Y)
					/ _MapHeight;
				break;
			// Upper-left.
			case 1:
				field[i * 4 + 2] = metrics.MapPosition.X
					/ _MapWidth;
				field[i * 4 + 3] = (_MapHeight - metrics.MapPosition.Y)
					/ _MapHeight;
				break;
			// Lower-left.
			case 2:
			case 3:
				field[i * 4 + 2] = metrics.MapPosition.X
					/ _MapWidth;
				field[i * 4 + 3] = (_MapHeight - metrics.MapPosition.Y - pxSize)
					/ _MapHeight;
				break;
			// Lower-right.
			case 4:
				field[i * 4 + 2] = (metrics.MapPosition.X + pxSize)
					/ _MapWidth;
				field[i * 4 + 3] = (_MapHeight - metrics.MapPosition.Y - pxSize)
					/ _MapHeight;
				break;
		}
	}
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, _TransformUbo);

	glBindBuffer(GL_ARRAY_BUFFER, _Vbo);
	glBufferData(GL_ARRAY_BUFFER, bufSize, field, GL_STREAM_DRAW);

	glBindVertexArray(_Vao);
	glVertexAttribPointer(_VertexPosLoc, 2, GL_FLOAT, false,
		4 * sizeof(float), 0);
	glVertexAttribPointer(_TexturePosLoc, 2, GL_FLOAT, false,
		4 * sizeof(float), (void*)(2 * sizeof(float)));

	glActiveTexture(GL_TEXTURE0 + _TextureLoc);
	glBindTexture(GL_TEXTURE_2D, _Texture);

	_Program->Shade(GL_TRIANGLES, count);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

_L_NS_END_GRAPHICS_UI

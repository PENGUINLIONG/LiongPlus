// File: OpenGLCanvas.cpp
// Author: Rendong Liang (Liong)
#include <codecvt>
#include "LiongPlus/BufferPool.hpp"
#include "LiongPlus/Graphics/UI/OpenGLCanvas.hpp"
#include "LiongPlus/Graphics/UI/OpenGLBrush.hpp"
#include "LiongPlus/Graphics/UI/Window.hpp"
#include "LiongPlus/Serialization/Unicode.hpp"
#include "LiongPlus/Platform/MinOpenGL.hpp"

_L_NS_BEG_GRAPHICS_UI

using namespace LiongPlus::Platform::OpenGL;
using namespace LiongPlus::Platform::Windows;

namespace details
{
	constexpr PixelFormatDescriptor getGeneralPfd()
	{
		return
		{
			sizeof(PixelFormatDescriptor),
			1, // Always 1.
			PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER |
			PFD_SWAP_EXCHANGE,
			PFD_TYPE_RGBA, // For glAddSwapHintRectWIN extension. 
			32, // Color depth.
			0,0,0,0,0,0,
			0,0,0,0,0,0,0,
			24, // Depth buffer.
			8,0,
			PFD_MAIN_PLANE,
			0,
			0,0,0,
		};
	}

	std::vector<int> getPxFmtIntAttrList()
	{
		std::vector<int> list{
			// Hardware accelerated OpenGL.
			WGL_DRAW_TO_WINDOW_ARB, true,
			WGL_SUPPORT_OPENGL_ARB, true,
			WGL_DOUBLE_BUFFER_ARB, true,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			// Color bits.
			WGL_COLOR_BITS_ARB, 32,
			// 24-bit depth buffer is generally supported.
			WGL_DEPTH_BITS_ARB, 24,
			// Stencil buffer.
			WGL_STENCIL_BITS_ARB, 8,
			// Buffer swapping.
			WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
		};
		// MSAA multisampling.
		list.push_back(WGL_SAMPLE_BUFFERS_ARB);
		list.push_back(true);
		list.push_back(WGL_SAMPLES_ARB);
		list.push_back(8);
		list.push_back(0);
		return list;
	}
}

//
// OpenGLCanvas
//

OpenGLCanvas::OpenGLCanvas(const IMetadataHost& host) :
	_DeviceContext(std::any_cast<Handle>(host->Metadata["DeviceContext"])),
	_RenderContext(nullptr)
{
	if (!_Initialize())
		throw std::runtime_error("Failed to initialize OpenGL context.");
}
OpenGLCanvas::~OpenGLCanvas()
{
	_Finalize();
}

bool OpenGLCanvas::_Initialize()
{
	int attributes[] ={
		WGL_CONTEXT_MAJOR_VERSION_ARB, __L_OPENGL_VERSION_MAJOR,
		WGL_CONTEXT_MINOR_VERSION_ARB, __L_OPENGL_VERSION_MINOR,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 0 };

	auto wndLegacy = std::make_shared<Window>();
	auto dcLegacy = std::any_cast<Handle>(wndLegacy->Metadata["DeviceContext"]);
	Handle rc = nullptr, rcLegacy = nullptr;
	auto pfd = details::getGeneralPfd();
	unsigned int pxFmtCountAvail;
	int nPxFmt;

	bool success =
		// If OpenGL context is not created, glewInit() will fail with code 1
		//   (GLEW_ERROR_NO_GL_VERSION).
		!!(nPxFmt = ChoosePixelFormat(dcLegacy, &pfd)) &&
		!!(SetPixelFormat(dcLegacy, nPxFmt, &pfd)) &&
		!!(rcLegacy = wglCreateContext(dcLegacy)) &&
		!!wglMakeCurrent(dcLegacy, rcLegacy) &&
		(Platform::OpenGL::_InitializeMinOpenGL(), true) &&
		wglChoosePixelFormatARB(_DeviceContext, details::getPxFmtIntAttrList().data(), nullptr, 1, &nPxFmt, &pxFmtCountAvail) &&
		(SetPixelFormat(_DeviceContext, nPxFmt, &pfd)) &&
		!!(rc = wglCreateContextAttribsARB(_DeviceContext, nullptr, attributes)) &&
		!!wglMakeCurrent(nullptr, nullptr) &&
		!!wglDeleteContext(rcLegacy) &&
		!!wglMakeCurrent(_DeviceContext, rc);

	if (!success) return false;
	_RenderContext = rc;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glClearColor(0.5, 0.5, 0.5, 0.5);
	glClearColor(1., 1., 1., 1.);
	glEnable(GL_MULTISAMPLE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Clear();

	return success;
}
bool OpenGLCanvas::_Finalize()
{
	return !!wglMakeCurrent(nullptr, nullptr) &&
		!!wglDeleteContext(_RenderContext);
}

void OpenGLCanvas::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void OpenGLCanvas::Submit()
{
	SwapBuffers(_DeviceContext);
}

void OpenGLCanvas::Resize(const Rect& clientArea)
{
	// Push identity first.
	Canvas::Resize(clientArea);

	glViewport(
		GLsizei(clientArea.Left),
		GLsizei(clientArea.Top),
		GLsizei(clientArea.Width()),
		GLsizei(clientArea.Height())
	);

	_TransformStack.emplace(Matrix4x4{ {
		{ (Float)2. / clientArea.Width(), (Float)0., (Float)0., (Float)0. },
		{ (Float)0., (Float)-2. / clientArea.Height(), (Float)0., (Float)0. },
		{ (Float)0., (Float)0., 1., (Float)0. },
		{ (Float)-1., (Float)+1., (Float)0., (Float)1. }
	} });
}

void GenerateCircularPath(Point*& field, Float radius, Float fromRad, Float angleRad, const Point& center, int segmentCount)
{
	auto drad_dseg = angleRad / segmentCount;
	// Plot circumference.
	for (int seg = 0; seg <= segmentCount; ++seg)
	{
		Float rad = fromRad + drad_dseg * seg;
		field->X = center.X + std::sin(rad) * radius;
		field->Y = center.Y - std::cos(rad) * radius;
		++field;
	}
}

void OpenGLCanvas::DrawArc(Float radius, Float fromDeg, Float angleDeg, const Point& center, bool enclose)
{
	if (!_Brush.IsValid()) return;
	auto& brush = *(_Brush.Get<OpenGLBrush>());

	const auto segmentCount = (int)std::ceil(radius * angleDeg * RAD_PER_DEG * SEGMENT_PER_INCH);
	const size_t count = (segmentCount + 1) + (!_ShouldFill && enclose ? 1 : 0);
	const auto bufferSize = sizeof(Point) * count;
	auto ref = BufferPool::Ginst().Get(bufferSize);
	Point* field = reinterpret_cast<Point*>(ref->Field());

	GenerateCircularPath(field, radius,
		fromDeg * RAD_PER_DEG, angleDeg * RAD_PER_DEG,
		center, segmentCount);
	if (!_ShouldFill && enclose)
	{
		field->X = center.X +
			std::sin(fromDeg * RAD_PER_DEG) * radius;
		field->Y = center.Y -
			std::cos(fromDeg * RAD_PER_DEG) * radius;
	}

	// Draw.
	brush.SetMode(_ShouldFill ? GL_TRIANGLE_FAN : GL_LINE_STRIP);
	brush.SetTransform(_TransformStack.top());
	brush.Paint({ (Point*)ref->Field(), count, false });
}
void OpenGLCanvas::DrawBezier(const Array<Point>& points)
{
	using namespace LiongPlus::Graphics::UI;
	if (points.size() < 4)
		throw std::invalid_argument("At least 4 points required.");

	throw std::logic_error("Not implemented yet.");
}
void OpenGLCanvas::DrawCircle(Float radius, const Point& center)
{
	if (!_Brush.IsValid()) return;
	auto& brush = *(_Brush.Get<OpenGLBrush>());

	const auto segmentCount = (int)std::ceil(radius * TAU * SEGMENT_PER_INCH);
	const size_t count = (segmentCount + 1) + (_ShouldFill ? 1 : 0);
	const auto bufferSize = sizeof(Point) * count;
	auto ref = BufferPool::Ginst().Get(bufferSize);
	Point* field = reinterpret_cast<Point*>(ref->Field());
	
	if (_ShouldFill)
		*(field++) = center;
	GenerateCircularPath(field, radius, 0., TAU, center, segmentCount);

	// Draw.
	brush.SetMode(_ShouldFill ? GL_TRIANGLE_FAN : GL_LINE_STRIP);
	brush.SetTransform(_TransformStack.top());
	brush.Paint({ (Point*)ref->Field(), count, false });
}
void OpenGLCanvas::DrawFan(Float radius, Float fromDeg, Float angleDeg, const Point& center)
{
	if (!_Brush.IsValid()) return;
	auto& brush = *(_Brush.Get<OpenGLBrush>());

	const auto segmentCount = (int)std::ceil(radius * TAU * SEGMENT_PER_INCH);
	const size_t count = (segmentCount + 1) + (_ShouldFill ? 1 : 2);
	const auto bufferSize = sizeof(Point) * count;
	auto ref = BufferPool::Ginst().Get(bufferSize);
	Point* field = reinterpret_cast<Point*>(ref->Field());

	*(field++) = center;
	GenerateCircularPath(field, radius, fromDeg * RAD_PER_DEG, angleDeg * RAD_PER_DEG, center, segmentCount);
	if (!_ShouldFill)
		*(field++) = center;

	// Draw.
	brush.SetMode(_ShouldFill ? GL_TRIANGLE_FAN : GL_LINE_STRIP);
	brush.SetTransform(_TransformStack.top()); 
	brush.Paint({ (Point*)ref->Field(), count, false });
}
void OpenGLCanvas::_DrawPointSet(GLenum func, const Array<Point>& points)
{
	// If there is no valid brush, give up rendering.
	if (!_Brush.IsValid()) return;
	auto& brush = *static_cast<IOpenGLBrush>(_Brush).Get();

	// Draw.
	brush.SetMode(func);
	brush.SetTransform(_TransformStack.top());
	brush.Paint(points);
}
void OpenGLCanvas::DrawLine(const Point& from, const Point& to)
{
	DrawLines({ from, to });
}
void OpenGLCanvas::DrawLines(const Array<Point>& verticies)
{
	_DrawPointSet(GL_LINES, verticies);
}
void OpenGLCanvas::DrawPolygon(const Array<Point>& verticies)
{
	_DrawPointSet(GL_TRIANGLE_STRIP, verticies);
}
void OpenGLCanvas::DrawRect(const Rect& rect)
{
	DrawRects({ rect });
}
void OpenGLCanvas::DrawRects(const Array<Rect>& rects)
{
	auto pointCount = rects.size() * 6;
	Array<Point> points(pointCount);
	for (auto i = 0; i < rects.size(); ++i)
	{
		const Rect& rect = rects[i];
		points[i * 6 + 0] ={ rect.Right, rect.Top };
		points[i * 6 + 1] ={ rect.Left, rect.Top };
		points[i * 6 + 2] ={ rect.Left, rect.Bottom };
		points[i * 6 + 3] ={ rect.Left, rect.Bottom };
		points[i * 6 + 4] ={ rect.Right, rect.Bottom };
		points[i * 6 + 5] ={ rect.Right, rect.Top };
	}
	_DrawPointSet(GL_TRIANGLES, points);
}
void OpenGLCanvas::DrawTriangles(const Array<Point>& points)
{
	_DrawPointSet(GL_TRIANGLES, points);
}
void OpenGLCanvas::DrawRoundRect(const Rect& rect, Float radius)
{
	if (!_Brush.IsValid()) return;
	auto& brush = *(_Brush.Get<OpenGLBrush>());

	const auto segmentCount = (int)std::ceilf(radius * .25 * TAU * SEGMENT_PER_INCH);
	const size_t count = 4 * (segmentCount + 1) + (_ShouldFill ? 2 : 1);
	const auto bufferSize = sizeof(Point) * count;
	auto ref = BufferPool::Ginst().Get(bufferSize);
	Point* field = reinterpret_cast<Point*>(ref->Field());

	if (_ShouldFill)
		*(field++) ={ (Float).5 * (rect.Left + rect.Right),
		(Float).5 * (rect.Top + rect.Bottom) };

	GenerateCircularPath(field, radius, (Float)0., (Float).25 * TAU, { rect.Right - radius, rect.Top + radius }, segmentCount);
	GenerateCircularPath(field, radius, (Float).25 * TAU, (Float).25 * TAU, { rect.Right - radius, rect.Bottom - radius }, segmentCount);
	GenerateCircularPath(field, radius, (Float).5 * TAU, (Float).25 * TAU, { rect.Left + radius, rect.Bottom - radius }, segmentCount);
	GenerateCircularPath(field, radius, (Float).75 * TAU, (Float).25 * TAU, { rect.Left + radius, rect.Top + radius }, segmentCount);

	*(field++) ={ rect.Right - radius, rect.Top };

	// Draw.
	brush.SetMode(_ShouldFill ? GL_TRIANGLE_FAN : GL_LINE_STRIP);
	brush.SetTransform(_TransformStack.top());
	brush.Paint({ (Point*)field, count, false });
}
void OpenGLCanvas::DrawDirectly(const Array<Point>& points)
{
	if (!_Brush.IsValid()) return;
	auto& brush = *(_Brush.Get<OpenGLBrush>());

	brush.SetTransform(_TransformStack.top());
	brush.Paint(points);
}

std::shared_ptr<OpenGLCanvas> UIFactory<OpenGLCanvas>::Create(const IMetadataHost& host)
{
	return std::make_shared<OpenGLCanvas>(host);
}

_L_NS_END_GRAPHICS_UI

// File: OpenGLCanvas.cpp
// Author: Rendong Liang (Liong)
#include <codecvt>
#include "../../BufferPool.hpp"
#include "OpenGLCanvas.hpp"
#include "OpenGLBrush.hpp"
#include "Window.hpp"
#include "../../Serialization/Unicode.hpp"

_L_NS_BEG_GRAPHICS_UI

namespace details
{
	constexpr PIXELFORMATDESCRIPTOR getGeneralPfd()
	{
		return
		{
			sizeof(PIXELFORMATDESCRIPTOR),
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
		bool pf = wglewIsSupported("WGL_ARB_pixel_format");
		if (!pf) throw std::runtime_error("OpenGL extension \"WGL_ARB_pixel_format\" is not supported.");
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
		bool ms = wglewIsSupported("WGL_ARB_multisample");
		if (ms)
		{
			list.push_back(WGL_SAMPLE_BUFFERS_ARB);
			list.push_back(true);
			list.push_back(WGL_SAMPLES_ARB);
			list.push_back(8);
		}
		list.push_back(0);
		return list;
	}
}

//
// OpenGLCanvas
//

OpenGLCanvas::OpenGLCanvas(const IDeviceContextHost& host) :
	_DeviceContext(host->GetDeviceContext()),
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
	static bool isGlewInited = false;

	int attributes[] ={
		WGL_CONTEXT_MAJOR_VERSION_ARB, OpenGLMajor,
		WGL_CONTEXT_MINOR_VERSION_ARB, OpenGLMinor,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 0 };

	auto wndLegacy = std::make_shared<Window>();
	auto dcLegacy = wndLegacy->GetDeviceContext();
	HGLRC rc = nullptr, rcLegacy = nullptr;
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
		isGlewInited ? true : (isGlewInited = glewInit() == GLEW_OK) &&
		wglewIsSupported("WGL_ARB_pixel_format") &&
		wglChoosePixelFormatARB(_DeviceContext, details::getPxFmtIntAttrList().data(), nullptr, 1, &nPxFmt, &pxFmtCountAvail) &&
		(SetPixelFormat(_DeviceContext, nPxFmt, &pfd)) &&
		wglewIsSupported("WGL_ARB_create_context") &&
		!!(rc = wglCreateContextAttribsARB(_DeviceContext, nullptr, attributes)) &&
		!!wglMakeCurrent(nullptr, nullptr) &&
		!!wglDeleteContext(rcLegacy) &&
		!!wglMakeCurrent(_DeviceContext, rc);

	if (!success) return false;
	_RenderContext = rc;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1., 1., 1., 1.);
	glEnable(GL_MULTISAMPLE);
	
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

	glViewport(clientArea.Left, clientArea.Top, clientArea.Width(), clientArea.Height());

	Matrix4x4 baseTrans;
	_TransformStack.emplace(Matrix4x4{ {
		{ 2. / clientArea.Width(), 0, 0, 0 },
		{ 0, -2. / clientArea.Height(), 0, 0 },
		{ 0, 0, 1., 0 },
		{ -1., +1., 0, 1. }
	} });
}

void GenerateCircularPath(float*& field, double radius, double fromRad, double angleRad, const Point& center, int segmentCount)
{
	auto drad_dseg = angleRad / segmentCount;
	// Plot circumference.
	for (int seg = 0; seg <= segmentCount; ++seg)
	{
		float rad = fromRad + drad_dseg * seg;
		*(field++) = center.X + std::sin(rad) * radius;
		*(field++) = center.Y - std::cos(rad) * radius;
	}
}

void OpenGLCanvas::DrawArc(double radius, double fromDeg, double angleDeg, const Point& center, bool enclose)
{
	if (!_Brush.IsValid()) return;
	auto& brush = *(_Brush.Get<OpenGLBrush>());

	const auto segmentCount = (int)std::ceilf(radius * angleDeg * RAD_PER_DEG * SEGMENT_PER_INCH);
	const auto count = (segmentCount + 1) + (!_ShouldFill && enclose ? 1 : 0);
	const auto bufferSize = 2 * sizeof(float) * count;
	auto ref = BufferPool::Ginst().Get(bufferSize);
	float* field = reinterpret_cast<float*>(ref->Field());

	GenerateCircularPath(field, radius,
		fromDeg * RAD_PER_DEG, angleDeg * RAD_PER_DEG,
		center, segmentCount);
	if (!_ShouldFill && enclose)
	{
		*(field++) = center.X + std::sin(fromDeg * RAD_PER_DEG) * radius;
		*(field++) = center.Y - std::cos(fromDeg * RAD_PER_DEG) * radius;
	}

	// Draw.
	brush.SetMode(_ShouldFill ? GL_TRIANGLE_FAN : GL_LINE_STRIP);
	brush.SetTransform(_TransformStack.top());
	brush.Paint(reinterpret_cast<float*>(ref->Field()), count);
}
void OpenGLCanvas::DrawBezier(const Array<Point>& points)
{
	using namespace LiongPlus::Graphics::UI;
	if (points.size() < 4)
		throw std::invalid_argument("At least 4 points required.");

	const Point& previous = *points.begin(), *next;

	throw std::logic_error("Not implemented yet.");
}
void OpenGLCanvas::DrawCircle(double radius, const Point& center)
{
	if (!_Brush.IsValid()) return;
	auto& brush = *(_Brush.Get<OpenGLBrush>());

	const auto segmentCount = (int)std::ceilf(radius * TAU * SEGMENT_PER_INCH);
	const auto count = (segmentCount + 1) + (_ShouldFill ? 1 : 0);
	const auto bufferSize = 2 * sizeof(float) * count;
	auto ref = BufferPool::Ginst().Get(bufferSize);
	float* field = reinterpret_cast<float*>(ref->Field());
	
	if (_ShouldFill)
	{
		*(field++) = center.X;
		*(field++) = center.Y;
	}
	GenerateCircularPath(field, radius, 0., TAU, center, segmentCount);

	// Draw.
	brush.SetMode(_ShouldFill ? GL_TRIANGLE_FAN : GL_LINE_STRIP);
	brush.SetTransform(_TransformStack.top());
	brush.Paint(reinterpret_cast<float*>(ref->Field()), count);
}
void OpenGLCanvas::DrawFan(double radius, double fromDeg, double angleDeg, const Point& center)
{
	if (!_Brush.IsValid()) return;
	auto& brush = *(_Brush.Get<OpenGLBrush>());

	const auto segmentCount = (int)std::ceilf(radius * TAU * SEGMENT_PER_INCH);
	const auto count = (segmentCount + 1) + (_ShouldFill ? 1 : 2);
	const auto bufferSize = 2 * sizeof(float) * count;
	auto ref = BufferPool::Ginst().Get(bufferSize);
	float* field = reinterpret_cast<float*>(ref->Field());

	*(field++) = center.X;
	*(field++) = center.Y;
	GenerateCircularPath(field, radius, fromDeg * RAD_PER_DEG, angleDeg * RAD_PER_DEG, center, segmentCount);
	if (!_ShouldFill)
	{
		*(field++) = center.X;
		*(field++) = center.Y;
	}

	// Draw.
	brush.SetMode(_ShouldFill ? GL_TRIANGLE_FAN : GL_LINE_STRIP);
	brush.SetTransform(_TransformStack.top()); 
	brush.Paint(reinterpret_cast<float*>(ref->Field()), count);
}
void OpenGLCanvas::_DrawPointSet(GLenum func, const Point* points, size_t pointCount)
{
	// If there is no valid brush, give up rendering.
	if (!_Brush.IsValid()) return;
	auto& brush = *static_cast<IOpenGLBrush>(_Brush).Get();

	const auto bufSize = (2 * pointCount) * sizeof(float);
	auto ref = BufferPool::Ginst().Get(bufSize);
	auto field = reinterpret_cast<float*>(ref->Field());
	
	for (auto pointIndex = 0; pointIndex < pointCount; ++pointIndex)
	{
		*(field++) = points[pointIndex].X;
		*(field++) = points[pointIndex].Y;
	}

	// Draw.
	brush.SetMode(func);
	brush.SetTransform(_TransformStack.top());
	brush.Paint(reinterpret_cast<float*>(ref->Field()), pointCount);
}
void OpenGLCanvas::DrawLine(const Point& from, const Point& to)
{
	DrawLines({ from, to });
}
void OpenGLCanvas::DrawLines(const Array<Point>& verticies)
{
	_DrawPointSet(GL_LINES, verticies.data(), verticies.size() - (verticies.size() % 2));
}
void OpenGLCanvas::DrawPolygon(const Array<Point>& verticies)
{
	_DrawPointSet(GL_TRIANGLE_STRIP, verticies.data(), verticies.size());
}
void OpenGLCanvas::DrawRect(const Rect& rect)
{
	DrawRects({ rect });
}
void OpenGLCanvas::DrawRects(const Array<Rect>& rects)
{
	auto pointCount = rects.size() * 6;
	BufferPool::Ref ref = BufferPool::Ginst().Get(pointCount * sizeof(Point));
	auto verticies = reinterpret_cast<Point*>(ref->Field());
	
	for (auto i = 0; i < rects.size(); ++i)
	{
		const Rect& rect = rects[i];
		verticies[i * 6 + 0] ={ rect.Right, rect.Top };
		verticies[i * 6 + 1] ={ rect.Left, rect.Top };
		verticies[i * 6 + 2] ={ rect.Left, rect.Bottom };
		verticies[i * 6 + 3] ={ rect.Left, rect.Bottom };
		verticies[i * 6 + 4] ={ rect.Right, rect.Bottom };
		verticies[i * 6 + 5] ={ rect.Right, rect.Top };
	}
	_DrawPointSet(GL_TRIANGLES, verticies, pointCount);
}
void OpenGLCanvas::DrawTriangles(const Array<Point>& verticies)
{
	_DrawPointSet(GL_TRIANGLES, verticies.data(), verticies.size() - (verticies.size() % 3));
}
void OpenGLCanvas::DrawRoundRect(const Rect& rect, Float radius)
{
	if (!_Brush.IsValid()) return;
	auto& brush = *(_Brush.Get<OpenGLBrush>());

	const auto segmentCount = (int)std::ceilf(radius * .25 * TAU * SEGMENT_PER_INCH);
	const auto count = 4 * (segmentCount + 1) + (_ShouldFill ? 2 : 1);
	const auto bufferSize = 2 * sizeof(float) * count;
	auto ref = BufferPool::Ginst().Get(bufferSize);
	float* field = reinterpret_cast<float*>(ref->Field());

	if (_ShouldFill)
	{
		*(field++) = .5 * (rect.Left + rect.Right);
		*(field++) = .5 * (rect.Top + rect.Bottom);
	}

	GenerateCircularPath(field, radius, 0., .25 * TAU, { rect.Right - radius, rect.Top + radius }, segmentCount);
	GenerateCircularPath(field, radius, .25 * TAU, .25 * TAU, { rect.Right - radius, rect.Bottom - radius }, segmentCount);
	GenerateCircularPath(field, radius, .5 * TAU, .25 * TAU, { rect.Left + radius, rect.Bottom - radius }, segmentCount);
	GenerateCircularPath(field, radius, .75 * TAU, .25 * TAU, { rect.Left + radius, rect.Top + radius }, segmentCount);

	*(field++) = rect.Right - radius;
	*(field++) = rect.Top;

	// Draw.
	brush.SetMode(_ShouldFill ? GL_TRIANGLE_FAN : GL_LINE_STRIP);
	brush.SetTransform(_TransformStack.top());
	brush.Paint(reinterpret_cast<float*>(ref->Field()), count);
}
void OpenGLCanvas::DrawDirectly(const float* points, size_t count)
{
	if (!_Brush.IsValid()) return;
	auto& brush = *(_Brush.Get<OpenGLBrush>());

	brush.SetTransform(_TransformStack.top());
	brush.Paint(points, count);
}

std::shared_ptr<OpenGLCanvas> UIFactory<OpenGLCanvas>::Create(const IDeviceContextHost& host)
{
	return std::make_shared<OpenGLCanvas>(host);
}

_L_NS_END_GRAPHICS_UI

// File: OpenGLCanvas.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "Canvas.hpp"
#include "UIFactory.hpp"
#include "DependencyInjection.hpp"
#include "DeviceContextHost.hpp"

_L_NS_BEG_GRAPHICS_UI

class OpenGLCanvas : public Canvas
{
private:
	DeviceContext _DeviceContext;
	HGLRC _RenderContext;

	void _DrawPointSet(GLenum func, const Point* points, size_t pointCount);

	bool _Initialize();
	bool _Finalize();

public:
	OpenGLCanvas(const IDeviceContextHost& device);
	~OpenGLCanvas();

	void Clear() override;
	void Submit() override;

	void Resize(const Rect& clientArea) override;

	void DrawArc(double radius, double fromDeg, double angleDeg, const Point& center, bool enclose) override;
	void DrawBezier(const Array<Point>& points) override;
	void DrawCircle(double radius, const Point& center) override;
	void DrawFan(double radius, double fromDeg, double angleDeg, const Point& center) override;
	void DrawLine(const Point& from, const Point& to) override;
	void DrawLines(const Array<Point>& verticies) override;
	void DrawPolygon(const Array<Point>& verticies) override;
	void DrawRect(const Rect& rect) override;
	void DrawRects(const Array<Rect>& rects) override;
	void DrawRoundRect(const Rect& rect, Float radius) override;
	void DrawTriangles(const Array<Point>& verticies) override;

	void DrawDirectly(const float* points, size_t count) override;
};

template<>
struct UIFactory<OpenGLCanvas>
{
	static std::shared_ptr<OpenGLCanvas> Create(const IDeviceContextHost& host);
};

_L_NS_END_GRAPHICS_UI

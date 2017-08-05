// File: OpenGLCanvas.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "Canvas.hpp"
#include "UIFactory.hpp"
#include "LiongPlus/MetadataHost.hpp"

_L_NS_BEG_GRAPHICS_UI

class OpenGLCanvas : public Canvas
{
private:
#ifdef __L_WINDOWS
	void* _DeviceContext;
	void* _RenderContext;
#endif

	void _DrawPointSet(unsigned func, const Array<Point>& points);

	bool _Initialize();
	bool _Finalize();

public:
	OpenGLCanvas(const IMetadataHost& device);
	~OpenGLCanvas();

	void Clear() override;
	void Submit() override;

	void Resize(const Rect& clientArea) override;

	void DrawArc(Float radius, Float fromDeg, Float angleDeg, const Point& center, bool enclose) override;
	void DrawBezier(const Array<Point>& points) override;
	void DrawCircle(Float radius, const Point& center) override;
	void DrawFan(Float radius, Float fromDeg, Float angleDeg, const Point& center) override;
	void DrawLine(const Point& from, const Point& to) override;
	void DrawLines(const Array<Point>& verticies) override;
	void DrawPolygon(const Array<Point>& verticies) override;
	void DrawRect(const Rect& rect) override;
	void DrawRects(const Array<Rect>& rects) override;
	void DrawRoundRect(const Rect& rect, Float radius) override;
	void DrawTriangles(const Array<Point>& verticies) override;

	void DrawDirectly(const Array<Point>& points) override;
};

template<>
struct UIFactory<OpenGLCanvas>
{
	static std::shared_ptr<OpenGLCanvas> Create(const IMetadataHost& host);
};

_L_NS_END_GRAPHICS_UI

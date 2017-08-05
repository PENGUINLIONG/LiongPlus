// File: Canvas.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <stack>
#include "_"
#include "../../Array.hpp"
#include "Brush.hpp"
#include "../../Event.hpp"
#include "../../Media/Font.hpp"
#include "../../Interface.hpp"
#include "../../Matrix4x4.hpp"

_L_NS_BEG_GRAPHICS_UI

enum TextAlignment
{
	Left, Center, Right
};

class Canvas
{
protected:
	bool _ShouldFill;
	IBrush _Brush;
	std::stack<Matrix4x4> _TransformStack;

public:
	Canvas();
	Canvas(const Canvas&) = delete;
	Canvas(Canvas&&) = delete;

	virtual void SetBrush(const IBrush& brush);
	virtual void SetFillMode(bool shouldFill);

	virtual void PushTransform(const Matrix4x4& transform);
	virtual void PopTransform();

	virtual void Resize(const Rect& clientArea);

	virtual void Clear() = 0;
	virtual void Submit() = 0;

	virtual void DrawArc(Float radius, Float fromDeg, Float angleDegree, const Point& center, bool enclose) = 0;
	virtual void DrawBezier(const Array<Point>& points) = 0;
	virtual void DrawCircle(Float radius, const Point& center) = 0;
	virtual void DrawFan(Float radius, Float fromDeg, Float angleDegree, const Point& center) = 0;
	virtual void DrawLine(const Point& from, const Point& to) = 0;
	virtual void DrawLines(const Array<Point>& verticies) = 0;
	virtual void DrawPolygon(const Array<Point>& verticies) = 0;
	virtual void DrawRect(const Rect& rect) = 0;
	virtual void DrawRects(const Array<Rect>& rects) = 0;
	virtual void DrawRoundRect(const Rect& rect, Float radius) = 0;
	virtual void DrawTriangles(const Array<Point>& verticies) = 0;

	/*
	 * This method allow the users to manage their own drawing
	 *   routine. But it should be carefully used.
	 *
	 * Params:
	 *   $points: base pointer of float array that contains all the
	 *     1x2 verticies to be drawn.
	 *   $count: Number of POINTS to be drawn, NOT THE NUMBER OF
	 *     FLOATS.
	 */
	virtual void DrawDirectly(const Array<Point>& points) = 0;
};

using ICanvas = I<Canvas>;

_L_NS_END_GRAPHICS_UI

// File: Canvas.cpp
// Author: Rendong Liang (Liong)
#include <cmath>
#include <stdexcept>
#include "LiongPlus/Graphics/UI/Canvas.hpp"

_L_NS_BEG_GRAPHICS_UI

//
// Canvas
//

Canvas::Canvas() :
	_ShouldFill(false),
	_Brush(nullptr),
	_TransformStack()
{
	_TransformStack.push(MatrixTransform::Identity());
}

void Canvas::SetBrush(const IBrush& brush)
{
	_Brush = brush;
	_Brush->SetTransform(_TransformStack.top());
}
void Canvas::SetFillMode(bool shouldFill)
{
	_ShouldFill = shouldFill;
}

void Canvas::PushTransform(const Matrix4x4& transform)
{
	_TransformStack.push(transform.Transform(_TransformStack.top()));
}
void Canvas::PopTransform()
{
	_TransformStack.pop();
}

void Canvas::Resize(const Rect& clientArea)
{
	_TransformStack = std::stack<Matrix4x4>();
	_TransformStack.emplace(MatrixTransform::Identity());
}

_L_NS_END_GRAPHICS_UI
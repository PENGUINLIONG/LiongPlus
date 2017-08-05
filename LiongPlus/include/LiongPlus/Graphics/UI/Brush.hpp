// File: Brush.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "../../Interface.hpp"
#include "../../Matrix4x4.hpp"
#include "DependencyInjection.hpp"

_L_NS_BEG_GRAPHICS_UI

/*
 * class Brush
 * ------
 * Enabling customization of UI element painting.
 */
class Brush :
	public DependencyHost
{
public:
	/*
	 * Do what ever necessary for a point.
	 * 
	 * Params:
	 *   $position: Ratio position in rectangular drawable area.
	 *     e.g. Drawing point (10, 3) in area [20, 30, 40, 50], the position will be
	 *     (10 / (40 - 20), 3 / (50-30)).
	 * Note:
	 *   Not only color but other related properties, like texture UV, can be specified.
	 */
	virtual void Paint(const Array<Point>& points) = 0;
	virtual void SetTransform(const Matrix4x4& transform) = 0;
};

using IBrush = I<Brush>;

_L_NS_END_GRAPHICS_UI

// File: Doodle.cpp
// Author: Rendong Liang (Liong)
#include "Doodle.hpp"

_L_NS_BEG_GRAPHICS_UI_CONTROLS

void Doodle::DoRender(const ICanvas& canvas)
{
	RenderEventArgs args;
	args.Canvas = canvas;
	Render(shared_from_this(), args);
}

_L_NS_END_GRAPHICS_UI_CONTROLS

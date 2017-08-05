// File: Doodle.cpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/Graphics/UI/Controls/Doodle.hpp"

_L_NS_BEG_GRAPHICS_UI_CONTROLS

void Doodle::DoRender(const ICanvas& canvas)
{
	RenderEventArgs args;
	args.Canvas = canvas;
	Render(shared_from_this(), args);
}

_L_NS_END_GRAPHICS_UI_CONTROLS

_L_NS_BEG_GRAPHICS_UI

std::shared_ptr<Controls::Doodle> UIFactory<Controls::Doodle>::Create()
{
	return std::make_shared<Controls::Doodle>();
}

_L_NS_END_GRAPHICS_UI

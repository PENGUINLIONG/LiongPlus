// File: Control.cpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/Graphics/UI/Controls/Button.hpp"

_L_NS_BEG_GRAPHICS_UI_CONTROLS

void Button::OnMouseDown(MouseButtonEventArgs& args)
{
	ContentControl::OnMouseDown(args);
	_MouseDown = true;
}

void Button::OnMouseUp(MouseButtonEventArgs& args)
{
	ContentControl::OnMouseUp(args);
	if (_MouseDown) OnClick(args);
	_MouseDown = false;
}

void Button::OnClick(MouseEventArgs& args)
{
	Click(shared_from_this(), args);
}

_L_NS_END_GRAPHICS_UI_CONTROLS

_L_NS_BEG_GRAPHICS_UI

std::shared_ptr<Controls::Button> UIFactory<Controls::Button>::Create()
{
	return std::make_shared<Controls::Button>();
}

_L_NS_END_GRAPHICS_UI

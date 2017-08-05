// File: Control.cpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/Graphics/UI/Control.hpp"

_L_NS_BEG_GRAPHICS_UI

const UIProperty
Control::ForegroundProperty = UIProperty::MakeDefault<IBrush>(
	"Foreground", UIActionTrigger::Render).Register<Control>();
const UIProperty
Control::BackgroundProperty = UIProperty::MakeDefault<IBrush>(
	"Background", UIActionTrigger::Render).Register<Control>();
const UIProperty
Control::BorderBrushProperty = UIProperty::MakeDefault<IBrush>(
	"BorderBrush", UIActionTrigger::Render).Register<Control>();
const UIProperty
Control::BorderProperty = UIProperty::MakeDefault<Thickness>(
	"Border", UIActionTrigger::Render).Register<Control>();
const UIProperty
Control::PaddingProperty = UIProperty::MakeDefault<Thickness>(
	"Padding", UIActionTrigger::Render).Register<Control>();

IBrush Control::GetBackGround()
{
	return GetProperty<IBrush>(BackgroundProperty);
}
void Control::SetBackGround(const IBrush& bg)
{
	SetProperty(BackgroundProperty, bg);
}
IBrush Control::GetForeground()
{
	return GetProperty<IBrush>(ForegroundProperty);
}
void Control::GetForeground(const IBrush& fg)
{
	SetProperty(ForegroundProperty, fg);
}
Thickness Control::GetPadding()
{
	return GetProperty<Thickness>(PaddingProperty);
}
void Control::SetPadding(const Thickness& pad)
{
	SetProperty(PaddingProperty, pad);
}


void Control::DoRender(const ICanvas& canvas)
{
	auto bg = GetBackGround();
	if (!(bg == nullptr))
	{
		canvas->SetFillMode(true);
		canvas->SetBrush(bg);
		auto size = GetSize();
		canvas->DrawRect({ 0, 0, size.Width, size.Height });
	}
}

_L_NS_END_GRAPHICS_UI

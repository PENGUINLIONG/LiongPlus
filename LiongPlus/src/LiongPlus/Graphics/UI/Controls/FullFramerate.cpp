// File: FullFramerate.cpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/Graphics/UI/Controls/FullFramerate.hpp"

_L_NS_BEG_GRAPHICS_UI_CONTROLS

const UIProperty FullFramerate::RenderTriggerProperty = UIProperty::MakeDefault<bool>("RenderTrigger", UIActionTrigger::Render).Register<FullFramerate>();

void FullFramerate::PostRender(const ICanvas& canvas)
{
	SetProperty(RenderTriggerProperty, true);
}

_L_NS_END_GRAPHICS_UI_CONTROLS

_L_NS_BEG_GRAPHICS_UI

std::shared_ptr<Controls::FullFramerate> UIFactory<Controls::FullFramerate>::Create()
{
	return std::make_shared<Controls::FullFramerate>();
}

_L_NS_END_GRAPHICS_UI

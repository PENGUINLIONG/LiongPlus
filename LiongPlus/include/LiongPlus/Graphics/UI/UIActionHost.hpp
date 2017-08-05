// File: UIActionHost.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"

_L_NS_BEG_GRAPHICS_UI

class UIActionHost
{
public:
	virtual void TriggerRender() = 0;
	virtual void TriggerLayout() = 0;
	virtual void TriggerLogicTree() = 0;
};

_L_NS_END_GRAPHICS_UI

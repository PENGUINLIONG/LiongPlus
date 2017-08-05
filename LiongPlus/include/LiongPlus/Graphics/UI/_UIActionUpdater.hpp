// File: [INTERNAL] _UIActionUpdater.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "../../Interface.hpp"
#include "UIActionHost.hpp"

_L_NS_BEG_GRAPHICS_UI

// Internal use. DO NOT CALL.
namespace UIUpdater
{
	void _Register(const I<UIActionHost>& target);
	void _Unregister(const I<UIActionHost>& target);

	void _RequestRender();
	void _RequestLayout();
	void _RequestLogicTree();
};

_L_NS_END_GRAPHICS_UI

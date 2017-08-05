// File: [INTERNAL] _UIAnimationUpdater.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "Animation.hpp"

_L_NS_BEG_GRAPHICS_UI

namespace UIUpdater
{
	bool _ApplyAnimation(const Animation& animation, const UIProperty& prop, const IUIElement& target, bool force);
	void _CancelAnimation(const UIProperty& prop, const IUIElement& target);
}

_L_NS_END_GRAPHICS_UI

// File: Control.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "../../Interface.hpp"
#include "../../Optional.hpp"
#include "UIElement.hpp"
#include "UIEvents.hpp"

_L_NS_BEG_GRAPHICS_UI

class Brush;

class Control :
	public UIElement,
	public DependencyObject<Control>
{
public:
	static const UIProperty
		ForegroundProperty,
		BackgroundProperty,
		BorderBrushProperty,
		BorderProperty,
		PaddingProperty;

	IBrush GetBackGround();
	void SetBackGround(const IBrush& bg);
	IBrush GetForeground();
	void GetForeground(const IBrush& fg);
	Thickness GetPadding();
	void SetPadding(const Thickness& pad);

	void DoRender(const ICanvas& canvas) override;
};

_L_NS_END_GRAPHICS_UI

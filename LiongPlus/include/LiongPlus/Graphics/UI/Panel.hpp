// File: Panel.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "Control.hpp"
#include "DependencyInjection.hpp"
#include "UIEvents.hpp"

_L_NS_BEG_GRAPHICS_UI

class Panel :
	public Control,
	public DependencyObject<Panel>
{
public:
	static const UIProperty ChildrenProperty;

	UIElementCollection GetChildren() const;
	void SetChildren(const UIElementCollection& children);


	// Override tunnels.

	void OnPreviewMouseDown(MouseButtonEventArgs& args) override;
	void OnPreviewMouseMove(MouseEventArgs& args) override;
	void OnPreviewMouseUp(MouseButtonEventArgs& args) override;
	void OnPreviewMouseWheel(MouseWheelEventArgs& args) override;

	void OnPreviewTouchDown(TouchEventArgs& args) override;
	void OnPreviewTouchMove(TouchEventArgs& args) override;
	void OnPreviewTouchUp(TouchEventArgs& args) override;

	void OnPreviewKeyUp(KeyboardEventArgs& args) override;
	void OnPreviewKeyDown(KeyboardEventArgs& args) override;





	void DoRender(const ICanvas& canvas) override;
	void LogicTree() override;
};

_L_NS_END_GRAPHICS_UI

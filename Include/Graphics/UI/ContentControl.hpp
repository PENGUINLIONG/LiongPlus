
#pragma once
#include "_"
#include "Control.hpp"

_L_NS_BEG_GRAPHICS_UI

class ContentControl :
	public Control,
	public DependencyObject<ContentControl>
{
public:

	// Dependency Properties.
	
	static const UIProperty ContentProperty;

	// Property accessors.

	IUIElement GetContent() const;
	void SetContent(const IUIElement& content);

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

	// Override UI reactors.

	void DoRender(const ICanvas& canvas) override;
	Size Layout(const Size& containerSize) override;
	void LogicTree() override;
};

_L_NS_END_GRAPHICS_UI

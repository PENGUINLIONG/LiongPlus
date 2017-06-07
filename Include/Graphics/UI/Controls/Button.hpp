// File: Button.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "../UIFactory.hpp"
#include "../ContentControl.hpp"
#include "../UIEvents.hpp"

_L_NS_BEG_GRAPHICS_UI_CONTROLS

class Button : public ContentControl
{
private:
	bool _MouseDown = false;
public:
	Event<I<Button>, MouseEventArgs> Click;

	Button() = default;

	void OnMouseDown(MouseButtonEventArgs& args) override;
	void OnMouseUp(MouseButtonEventArgs& args) override;
	virtual void OnClick(MouseEventArgs& args);
};

_L_NS_END_GRAPHICS_UI_CONTROLS

_L_NS_BEG_GRAPHICS_UI

template<>
struct UIFactory<Controls::Button>
{
	static std::shared_ptr<Controls::Button> Create();
};

_L_NS_END_GRAPHICS_UI

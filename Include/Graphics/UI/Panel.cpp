// File: Panel.cpp
// Author: Rendong Liang (Liong)
#include <vector>
#include "Panel.hpp"

_L_NS_BEG_GRAPHICS_UI

const UIProperty
Panel::ChildrenProperty =
	UIProperty::MakeDefault<UIElementCollection>
	("Children", UIActionTrigger::LogicTree_Layout_Render)
	.Register<Panel>();

UIElementCollection Panel::GetChildren() const
{
	return GetProperty<UIElementCollection>(ChildrenProperty);
}
void Panel::SetChildren(const UIElementCollection& children)
{
	SetProperty(ChildrenProperty, children);
}

void Panel::DoRender(const ICanvas& canvas)
{
	Control::DoRender(canvas);
	for (const auto& child : GetChildren())
		child->Render(canvas);
}

#pragma region Override tunnel event routers.

#define _L_OVERRIDE_TUNNEL_EVENT_ROUTER(name, argsType) \
void Panel::OnPreview##name(argsType& args) \
{ \
	Preview##name(shared_from_this(), args); \
	if (args.IsHandled) return;\
	auto children = GetChildren(); \
	for (auto& ctrl : children) \
	{ \
		if (ctrl.IsValid()) \
		{ \
			args.Source = shared_from_this(); \
			ctrl->OnPreview##name(args); \
			if (args.IsHandled) return; \
		} \
	} \
	args.Source = shared_from_this(); \
	On##name(args); \
}
#define _L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(name, argsType) \
void Panel::OnPreview##name(argsType& args) \
{ \
	if (!CursorHitTest(args.Position)) return; \
	auto rawPos = args.Position; \
	auto margin = GetMargin(); \
	Preview##name(shared_from_this(), args); \
	if (args.IsHandled) return;\
	auto children = GetChildren(); \
	args.Position ={ \
		args.Position.X - margin.Left, \
		args.Position.Y - margin.Top \
	}; \
	for (auto& ctrl : children) \
	{ \
		if (ctrl.IsValid()) \
		{ \
			args.Source = shared_from_this(); \
			ctrl->OnPreview##name(args); \
			if (args.IsHandled) return; \
		} \
	} \
	args.Source = shared_from_this(); \
	On##name(args); \
	args.Position = rawPos; \
}

_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(MouseDown, MouseButtonEventArgs)
_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(MouseMove, MouseEventArgs)
_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(MouseUp, MouseButtonEventArgs)
_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(MouseWheel, MouseWheelEventArgs)

_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(TouchDown, TouchEventArgs)
_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(TouchMove, TouchEventArgs)
_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(TouchUp, TouchEventArgs)

_L_OVERRIDE_TUNNEL_EVENT_ROUTER(KeyUp, KeyboardEventArgs)
_L_OVERRIDE_TUNNEL_EVENT_ROUTER(KeyDown, KeyboardEventArgs)

#undef _L_IMPL_TUNNEL_EVENT_ROUTER
#pragma endregion

void Panel::LogicTree()
{
	Control::LogicTree();
	for (const auto& child : GetChildren())
	{
		child->SetProperty(UIElement::ParentProperty, std::weak_ptr<UIElement>(shared_from_this()));
		child->LogicTree();
	}
}

_L_NS_END_GRAPHICS_UI

// File: ContentControl.cpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/Graphics/UI/ContentControl.hpp"

_L_NS_BEG_GRAPHICS_UI

const UIProperty
ContentControl::ContentProperty = UIProperty::MakeDefault<IUIElement>(
	"Content", UIActionTrigger::LogicTree_Layout_Render)
	.Register<ContentControl>();

IUIElement ContentControl::GetContent() const
{
	return GetProperty<IUIElement>(ContentProperty);
}
void ContentControl::SetContent(const IUIElement& content)
{
	SetProperty(ContentProperty, content);
}

#pragma region Override tunnel event routers.
#define _L_OVERRIDE_TUNNEL_EVENT_ROUTER(name, argsType) \
void ContentControl::OnPreview##name(argsType& args) \
{ \
	Preview##name(shared_from_this(), args); \
	if (args.IsHandled) return; \
	auto content = GetContent(); \
	if (content.IsValid()) \
	{ \
		args.Source = shared_from_this(); \
		content->OnPreview##name(args); \
		if (args.IsHandled) return; \
	} \
	args.Source = shared_from_this(); \
	On##name(args); \
};
#define _L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(name, argsType) \
void ContentControl::OnPreview##name(argsType& args) \
{ \
	if (!CursorHitTest(args.Position)) return; \
	auto rawPos = args.Position; \
	auto margin = GetMargin(); \
	Preview##name(shared_from_this(), args); \
	if (args.IsHandled) return; \
	auto content = GetContent(); \
	args.Position ={ \
		args.Position.X - margin.Left, \
		args.Position.Y - margin.Top \
	}; \
	if (content.IsValid()) \
	{ \
		args.Source = shared_from_this(); \
		content->OnPreview##name(args); \
		if (args.IsHandled) return; \
	} \
	args.Source = shared_from_this(); \
	On##name(args); \
	args.Position = rawPos; \
};

_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(MouseDown, MouseButtonEventArgs)
_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(MouseMove, MouseEventArgs)
_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(MouseUp, MouseButtonEventArgs)
_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(MouseWheel, MouseWheelEventArgs)

_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(TouchDown, TouchEventArgs)
_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(TouchMove, TouchEventArgs)
_L_OVERRIDE_CURSOR_TUNNEL_EVENT_ROUTER(TouchUp, TouchEventArgs)

_L_OVERRIDE_TUNNEL_EVENT_ROUTER(KeyDown, KeyboardEventArgs)
_L_OVERRIDE_TUNNEL_EVENT_ROUTER(KeyUp, KeyboardEventArgs)

#undef _L_OVERRIDE_TUNNEL_EVENT_ROUTER
#pragma endregion

void ContentControl::DoRender(const ICanvas& canvas)
{
	Control::DoRender(canvas);
	auto& content = GetProperty<IUIElement>(ContentProperty);
	if (content.IsValid())
		content->Render(canvas);
}

Size ContentControl::Layout(const Size& containerSize)
{
	auto rv = Control::Layout(containerSize);
	auto content = GetProperty<IUIElement>(ContentProperty);
	auto padding = GetProperty<Thickness>(PaddingProperty);
	auto size = GetProperty<Size>(SizeProperty);
	if (!(content == nullptr))
	{
		content->Layout({
			size.Width - padding.Left - padding.Right,
			size.Height - padding.Top - padding.Bottom
		});
	}
	return rv;
}

void ContentControl::LogicTree()
{
	Control::LogicTree();
	const auto& content = GetProperty<IUIElement>(ContentProperty);
	if (!content.IsValid()) return;
	content->SetProperty(UIElement::ParentProperty, std::weak_ptr<UIElement>(shared_from_this()));
	content->LogicTree();
}

_L_NS_END_GRAPHICS_UI

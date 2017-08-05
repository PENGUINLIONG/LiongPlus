// File: UIElement.cpp
// Author: Rendong Liang (Liong)
#include <cmath>
#include "LiongPlus/Graphics/UI/UIElement.hpp"
#include "LiongPlus/Graphics/UI/UIActionHost.hpp"

_L_NS_BEG_GRAPHICS_UI

const UIProperty
UIElement::NameProperty = UIProperty::MakeDefault<std::string>(
	"Name", UIActionTrigger::None,
	[](const std::string& str) { return !str.empty(); }).Register<UIElement>();
const UIProperty
UIElement::IsEnabledProperty = UIProperty::MakeDefault<bool>(
	"IsEnabled", UIActionTrigger::Render).Register<UIElement>();
const UIProperty
UIElement::SizeProperty = UIProperty::MakeDefault<Size>(
	"Size", UIActionTrigger::Layout_Render).Register<UIElement>();
const UIProperty
UIElement::VisibilityProperty = UIProperty::MakeDefault<Visibility>(
	"Visibility", UIActionTrigger::Layout_Render).Register<UIElement>();
const UIProperty
UIElement::AnchorProperty = UIProperty::MakeDefault<Point>(
	"Anchor", UIActionTrigger::Render).Register<UIElement>();
const UIProperty
UIElement::HorizontalAlignmentProperty = UIProperty::MakeDefault<HorizontalAlignment>(
	"HorizontalAlignment", UIActionTrigger::Layout_Render).Register<UIElement>();
const UIProperty
UIElement::VerticalAlignmentProperty = UIProperty::MakeDefault<VerticalAlignment>(
	"VerticalAlignment", UIActionTrigger::Layout_Render).Register<UIElement>();
const UIProperty
UIElement::TransformProperty = UIProperty::Make<Matrix4x4>(
	"Transform", UIActionTrigger::Render,
	[](const Matrix4x4&) { return true; },
	[]() { Matrix4x4* m = new Matrix4x4; *m = MatrixTransform::Identity(); return m; },
	[](void* m) { delete reinterpret_cast<Matrix4x4*>(m); }).Register<UIElement>();
const UIProperty
UIElement::MarginProperty = UIProperty::MakeDefault<Thickness>(
	"Margin", UIActionTrigger::Layout_Render).Register<UIElement>();
const UIProperty
UIElement::ParentProperty = UIProperty::MakeDefault<std::weak_ptr<UIElement>>(
	"Parent", UIActionTrigger::LogicTree_Layout_Render).Register<UIElement>();
const UIProperty
UIElement::DispatcherProperty = UIProperty::Make<std::shared_ptr<Threading::Dispatcher>>(
	"Dispatcher", UIActionTrigger::None,
	[](const std::shared_ptr<Threading::Dispatcher>& dispatcher) { return true; },
	[]() { return new std::shared_ptr<Threading::Dispatcher>(Threading::Dispatcher::Current()); },
	[](void* d) { delete reinterpret_cast<std::shared_ptr<Threading::Dispatcher>*>(d); }).Register<UIElement>();
const UIProperty
UIElement::UIActionHostProperty =
UIProperty::MakeDefault<UIActionHost*>("UIActionHost", UIActionTrigger::Render).Register<UIElement>();

namespace details
{
	/*
	 * Test if $value is in range [$from, $from + $size].
	 */
	bool isInRange(Float value, Float from, Float size)
	{
		value -= from;
		return value >= 0 && value <= size;
	}
}

bool UIElement::CursorHitTest(const Point& pos)
{
	using namespace details;

	const auto& margin = GetMargin();
	const auto& size = GetSize();

	return pos.X >= margin.Left &&
		pos.X <= margin.Left + size.Width &&
		pos.Y >= margin.Top &&
		pos.Y <= margin.Top + size.Height;
}

std::string UIElement::GetName() const
{
	return GetProperty<std::string>(NameProperty);
}
bool UIElement::IsEnabled() const
{
	return GetProperty<bool>(IsEnabledProperty);
}
Size UIElement::GetSize() const
{
	return GetProperty<Size>(SizeProperty);
}
Visibility UIElement::GetVisibility() const
{
	return GetProperty<Visibility>(VisibilityProperty);
}
Point UIElement::GetAnchor() const
{
	return GetProperty<Point>(AnchorProperty);
}
HorizontalAlignment UIElement::GetHorizontalAlignment() const
{
	return GetProperty<HorizontalAlignment>(HorizontalAlignmentProperty);
}
VerticalAlignment UIElement::GetVerticalAlignment() const
{
	return GetProperty<VerticalAlignment>(VerticalAlignmentProperty);
}
Matrix4x4 UIElement::GetTransform() const
{
	return GetProperty<Matrix4x4>(TransformProperty);
}
Thickness UIElement::GetMargin() const
{
	return GetProperty<Thickness>(MarginProperty);
}
IUIElement UIElement::GetParent() const
{
	return GetProperty<std::weak_ptr<UIElement>>(ParentProperty);
}
const std::shared_ptr<Threading::Dispatcher>& UIElement::GetDispatcher() const
{
	return GetProperty<std::shared_ptr<Threading::Dispatcher>>(DispatcherProperty);
}
const UIActionHost* UIElement::GetUIActionHost() const
{
	return GetProperty<UIActionHost*>(UIActionHostProperty);
}

void UIElement::SetName(const std::string& name)
{
	SetProperty(NameProperty, name);
}
void UIElement::Enable()
{
	SetProperty(IsEnabledProperty, true);
}
void UIElement::Disable()
{
	SetProperty(IsEnabledProperty, false);
}
void UIElement::SetSize(const Size& size)
{
	SetProperty(SizeProperty, size);
}
void UIElement::SetVisibility(Visibility visibility)
{
	SetProperty(VisibilityProperty, visibility);
}
void UIElement::SetAnchor(const Point& anchor)
{
	SetProperty(AnchorProperty, anchor);
}
void UIElement::SetHorizontalAlignment(HorizontalAlignment align)
{
	SetProperty(HorizontalAlignmentProperty, align);
}
void UIElement::SetVerticalAlignment(VerticalAlignment align)
{
	SetProperty(VerticalAlignmentProperty, align);
}
void UIElement::SetTransform(const Matrix4x4& mat)
{
	SetProperty(TransformProperty, mat);
}
void UIElement::SetMargin(const Thickness& margin)
{
	SetProperty(MarginProperty, margin);
}

void UIElement::OnMouseEnter(EventArgs& args)
{
	MouseEnter(shared_from_this(), args);
}
void UIElement::OnMouseLeave(EventArgs& args)
{
	MouseLeave(shared_from_this(), args);
}
void UIElement::OnSizeChanged(EventArgs& args)
{
	SizeChanged(shared_from_this(), args);
}
void UIElement::OnCharInput(CharInputEventArgs& args)
{
	CharInput(shared_from_this(), args);
}

#pragma region Default event routers.
#define _L_IMPL_DEFAULT_EVENT_ROUTER(name, argsType) \
void UIElement::On##name(argsType& args) \
{ \
	name(shared_from_this(), args); \
	if (args.IsHandled) return; \
} \
void UIElement::OnPreview##name(argsType& args) \
{ \
	Preview##name(shared_from_this(), args); \
	if (args.IsHandled) return; \
	On##name(args); \
}

#define _L_IMPL_DEFAULT_CURSOR_EVENT_ROUTER(name, argsType) \
void UIElement::On##name(argsType& args) \
{ \
	name(shared_from_this(), args); \
	if (args.IsHandled) return; \
} \
void UIElement::OnPreview##name(argsType& args) \
{ \
	if (!CursorHitTest(args.Position)) return; \
	Preview##name(shared_from_this(), args); \
	if (args.IsHandled) return; \
	On##name(args); \
}


_L_IMPL_DEFAULT_CURSOR_EVENT_ROUTER(MouseDown, MouseButtonEventArgs)
_L_IMPL_DEFAULT_CURSOR_EVENT_ROUTER(MouseMove, MouseEventArgs)
_L_IMPL_DEFAULT_CURSOR_EVENT_ROUTER(MouseUp, MouseButtonEventArgs)
_L_IMPL_DEFAULT_CURSOR_EVENT_ROUTER(MouseWheel, MouseWheelEventArgs)

_L_IMPL_DEFAULT_CURSOR_EVENT_ROUTER(TouchDown, TouchEventArgs)
_L_IMPL_DEFAULT_CURSOR_EVENT_ROUTER(TouchMove, TouchEventArgs)
_L_IMPL_DEFAULT_CURSOR_EVENT_ROUTER(TouchUp, TouchEventArgs)

_L_IMPL_DEFAULT_EVENT_ROUTER(KeyDown, KeyboardEventArgs)
_L_IMPL_DEFAULT_EVENT_ROUTER(KeyUp, KeyboardEventArgs)

#undef _L_IMPL_DEFAULT_EVENT_ROUTER
#pragma endregion

void UIElement::Render(const ICanvas& canvas)
{
	PreRender(canvas);
	DoRender(canvas);
	PostRender(canvas);
}

void UIElement::PreRender(const ICanvas& canvas)
{
	auto margin = GetMargin();
	canvas->PushTransform(MatrixTransform::Translate(margin.Left, margin.Top));
	canvas->PushTransform(GetTransform());
}
void UIElement::DoRender(const ICanvas& canvas)
{
}
void UIElement::PostRender(const ICanvas& canvas)
{
	canvas->PopTransform();
	canvas->PopTransform();
}

Size UIElement::Layout(const Size& containerSize)
{
	using namespace std;
	auto size = GetSize();
	auto margin = GetMargin();
	Size minSize ={};

	switch (GetHorizontalAlignment())
	{
	case HorizontalAlignment::Left:
		minSize.Width = margin.Right = margin.Left + size.Width;
		break;
	case HorizontalAlignment::Middle:
		margin.Left = (containerSize.Width - size.Width) / 2.;
		margin.Right = (containerSize.Width + size.Width) / 2.;
		minSize.Width = size.Width;
		break;
	case HorizontalAlignment::Right:
		margin.Left = containerSize.Width - margin.Right - size.Width;
		minSize.Width = margin.Right + size.Width;
		break;
	case HorizontalAlignment::Stretch:
		size.Width = containerSize.Width - margin.Left - margin.Right;
		minSize.Width = containerSize.Width;
		break;
	}
	switch (GetVerticalAlignment())
	{
	case VerticalAlignment::Top:
		minSize.Height = margin.Bottom = margin.Top + size.Height;
		break;
	case VerticalAlignment::Center:
		margin.Top = (containerSize.Height - size.Height) / 2;
		margin.Bottom = (containerSize.Height + size.Height) / 2;
		minSize.Height = size.Height;
		break;
	case VerticalAlignment::Bottom:
		margin.Top = containerSize.Height - margin.Bottom - size.Height;
		minSize.Height = margin.Bottom + size.Height;
		break;
	case VerticalAlignment::Stretch:
		size.Height = containerSize.Height - margin.Top - margin.Bottom;
		minSize.Height = containerSize.Height;
		break;
	}

	minSize.Width = min(minSize.Width, containerSize.Width);
	minSize.Height = min(minSize.Height, containerSize.Height);

	SetSize(size);
	SetMargin(margin);

	return minSize;
}

void UIElement::LogicTree()
{
}

_L_NS_END_GRAPHICS_UI

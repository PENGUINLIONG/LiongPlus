// File: UIElement.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "_"
#include "Canvas.hpp"
#include "DependencyInjection.hpp"
#include "../../Threading/Dispatcher.hpp"
#include "../../Interface.hpp"
#include "../../Matrix4x4.hpp"
#include "UIEvents.hpp"

_L_NS_BEG_GRAPHICS_UI

enum class Visibility
{
	Visible,
	Collapsed,
	Hidden,
};

enum class HorizontalAlignment
{
	Left,
	Middle,
	Right,
	Stretch
};

enum class VerticalAlignment
{
	Top,
	Center,
	Bottom,
	Stretch
};

class UIElement;

using IUIElement = Interface<UIElement>;

/*
 * About Layout:
 *   The following properties are involved in layout process:
 *   * [Size]
 *   * [Margin]
 *   * [HorizontalAlignment]
 *   * [VerticalAlignment]
 *   On layouting, either [Size] or [Margin] will be given a new value.
 *   When an alignment is set to [Stretch], the [Size] will be re-calculated.
 *   Otherwise, the opposite side's, or both sides', [Margin] will be re-calculated.
 *   If the size is too small for a control, the control might exceed the boundary and overlap other controls.
 */
class UIElement :
	public std::enable_shared_from_this<UIElement>,
	public DependencyHost,
	public DependencyObject<UIElement>
{
protected:
	bool CursorHitTest(const Point& pos);

public:
	
	// Dependency properties.

	static const UIProperty
		NameProperty,
		IsEnabledProperty,
		SizeProperty,
		VisibilityProperty,
		AnchorProperty,
		HorizontalAlignmentProperty,
		VerticalAlignmentProperty,
		TransformProperty,
		MarginProperty,
		ParentProperty,
		DispatcherProperty,
		UIActionHostProperty;

	// Property accessors.

	std::string GetName() const;
	bool IsEnabled() const;
	Size GetSize() const;
	Visibility GetVisibility() const;
	Point GetAnchor() const;
	HorizontalAlignment GetHorizontalAlignment() const;
	VerticalAlignment GetVerticalAlignment() const;
	Matrix4x4 GetTransform() const;
	Thickness GetMargin() const;
	IUIElement GetParent() const;
	const std::shared_ptr<Threading::Dispatcher>& GetDispatcher() const;
	const UIActionHost* GetUIActionHost() const;

	void SetName(const std::string& name);
	void Enable();
	void Disable();
	void SetSize(const Size& size);
	void SetVisibility(Visibility visibility);
	void SetAnchor(const Point& anchor);
	void SetHorizontalAlignment(HorizontalAlignment align);
	void SetVerticalAlignment(VerticalAlignment align);
	void SetTransform(const Matrix4x4& mat);
	void SetMargin(const Thickness& margin);

	// Events

	template<typename TEventArgs>
	using UIEvent = Event<IUIElement, TEventArgs>;
	
	UIEvent<EventArgs>
		MouseEnter, MouseLeave, SizeChanged;
	UIEvent<MouseEventArgs>
		PreviewMouseMove, MouseMove;
	UIEvent<MouseButtonEventArgs>
		PreviewMouseDown, PreviewMouseUp, MouseDown, MouseUp;
	UIEvent<MouseWheelEventArgs>
		PreviewMouseWheel, MouseWheel;
	UIEvent<TouchEventArgs>
		PreviewTouchDown, PreviewTouchMove, PreviewTouchUp,
		TouchDown, TouchMove, TouchUp;
	UIEvent<KeyboardEventArgs>
		PreviewKeyDown, PreviewKeyUp, KeyDown, KeyUp;
	UIEvent<CharInputEventArgs>
		CharInput;

	// Simple events.

	virtual void OnMouseEnter(EventArgs& args);
	virtual void OnMouseLeave(EventArgs& args);
	virtual void OnSizeChanged(EventArgs& args);
	virtual void OnCharInput(CharInputEventArgs& args);

	// Bubble events raiser.

	virtual void OnMouseDown(MouseButtonEventArgs& args);
	virtual void OnMouseMove(MouseEventArgs& args);
	virtual void OnMouseUp(MouseButtonEventArgs& args);
	virtual void OnMouseWheel(MouseWheelEventArgs& args);

	virtual void OnTouchDown(TouchEventArgs& args);
	virtual void OnTouchMove(TouchEventArgs& args);
	virtual void OnTouchUp(TouchEventArgs& args);

	virtual void OnKeyUp(KeyboardEventArgs& args);
	virtual void OnKeyDown(KeyboardEventArgs& args);

	// Tunnel events raiser.

	virtual void OnPreviewMouseDown(MouseButtonEventArgs& args);
	virtual void OnPreviewMouseMove(MouseEventArgs& args);
	virtual void OnPreviewMouseUp(MouseButtonEventArgs& args);
	virtual void OnPreviewMouseWheel(MouseWheelEventArgs& args);

	virtual void OnPreviewTouchDown(TouchEventArgs& args);
	virtual void OnPreviewTouchMove(TouchEventArgs& args);
	virtual void OnPreviewTouchUp(TouchEventArgs& args);

	virtual void OnPreviewKeyUp(KeyboardEventArgs& args);
	virtual void OnPreviewKeyDown(KeyboardEventArgs& args);

	// UI reactors.

	void Render(const ICanvas& canvas);
	/*
	 * Preparation works for rendering.
	 * e.g. push transform matricies in stack.
	 */
	virtual void PreRender(const ICanvas& canvas);
	/*
	 * Render the UI element. The drawable area is not clipped.
	 */
	virtual void DoRender(const ICanvas& canvas);
	/*
	 * Finish rendering, discard modifications done before in PreRender().
	 * e.g. Pop transforms out.
	 */
	virtual void PostRender(const ICanvas& canvas);
	/*
	 * Recalculate the measures of UI element.
	 * Params:
	 *   The maximum size the UI element can extend to.
	 * Returns:
	 *   The minimum size taken up by the UI element, including fixed-side margins.
	 *   (Fixed-side margin: Assume an element horizontally aligned to the left, the fixed-side margin
	 *     will be [Margin.Left] as its value must be preserved)
	 */
	virtual Size Layout(const Size& containerSize);
	/*
	 * Rearrange logic tree and attach properties to content/children.
	 */
	virtual void LogicTree();
};

using UIElementCollection = std::vector<IUIElement>;

_L_NS_END_GRAPHICS_UI

// File: UIEvents.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "../../Event.hpp"
#include "../../Interface.hpp"

_L_NS_BEG_GRAPHICS_UI

class UIElement;

struct RoutedEventArgs : public EventArgs
{
public:
	I<UIElement> OriginalSource;
	I<UIElement> Source;
	bool IsHandled;

	RoutedEventArgs(const I<UIElement>& src) :
		EventArgs(),
		OriginalSource(src),
		Source(src),
		IsHandled(false)
	{
	}
};

struct InputEventArgs : public RoutedEventArgs
{
public:
	TimePoint TimePoint;

	InputEventArgs(const I<UIElement>& src) :
		RoutedEventArgs(src),
		TimePoint(std::chrono::steady_clock::now())
	{
	}
};

enum class MouseButton
{
	None       = 0x00000000,
	AuxControl = 0x00000001,
	AuxAlt     = 0x00000010,
	AuxShift   = 0x00000100,
	Left       = 0x00001000,
	Right      = 0x00010000,
	Wheel      = 0x00100000,
	X1         = 0x01000000,
	X2         = 0x10000000,
};
constexpr MouseButton operator|(MouseButton a, MouseButton b)
{
	return static_cast<MouseButton>(static_cast<int>(a) | static_cast<int>(b));
}
constexpr MouseButton operator&(MouseButton a, MouseButton b)
{
	return static_cast<MouseButton>(static_cast<int>(a) & static_cast<int>(b));
}

enum class MouseButtonState
{
	Pressed, Released
};

struct MouseEventArgs : public InputEventArgs
{
public:
	Point Position;
	MouseButton DownButtons;

	MouseEventArgs(const I<UIElement>& src, const Point& pos, MouseButton downBtn) :
		InputEventArgs(src),
		Position(pos),
		DownButtons(downBtn)
	{
	}
};

struct MouseButtonEventArgs : public MouseEventArgs
{
public:
	MouseButton Button;

	MouseButtonEventArgs(const I<UIElement>& src, const Point& pos, MouseButton downBtn, MouseButton btn) :
		MouseEventArgs(src, pos, downBtn),
		Button(btn)
	{
	}
};

struct MouseWheelEventArgs : public InputEventArgs
{
public:
	// Here, on Windows, delta is defined as 1/120 unit of wheel rotation. Positive value for forward rotation, away from user, and vice versa.
	Point Position;
	int Delta;

	MouseWheelEventArgs(const I<UIElement>& src, const Point& pos, int delta) :
		InputEventArgs(src),
		Position(pos),
		Delta(delta)
		{
		}
};

enum class KeyboardKeyState
{
	Pressed, Released
};

struct KeyboardEventArgs : public InputEventArgs
{
	unsigned char VirtualKey;
	int RepeatCount;
	KeyboardKeyState PreviousState;
	KeyboardKeyState State;

	KeyboardEventArgs(I<UIElement> src, unsigned char vk, int repeat, KeyboardKeyState prevState, KeyboardKeyState state) :
		InputEventArgs(src),
		VirtualKey(vk),
		RepeatCount(repeat),
		PreviousState(prevState),
		State(state)
	{
	}
};

struct CharInputEventArgs : public EventArgs
{
	char32_t Char;
};

enum class Gesture
{
	Begin,
	End,
	Zoom,
	Pan,
	Rotate,
	TwoFingerTap,
	PressAndTap,
};

struct TouchEventArgs : public InputEventArgs
{
public:
	Point Position;
	Gesture Gesture;

	TouchEventArgs(const I<UIElement>& src, const Point& pos, enum class Gesture ges) :
		InputEventArgs(src),
		Position(pos),
		Gesture(ges)
	{
	}
};

_L_NS_END_GRAPHICS_UI

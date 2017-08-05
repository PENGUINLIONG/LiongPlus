// File: Window.cpp
// Author: Rendong Liang (Liong)
#include <chrono>
#include <utility>
#include <random>
#include "LiongPlus/Graphics/UI/Window.hpp"
#include "LiongPlus/Graphics/UI/_UIActionUpdater.hpp"
#include "LiongPlus/Serialization/Unicode.hpp"

using namespace LiongPlus::Platform::Windows;

_L_NS_BEG_GRAPHICS_UI

const UIProperty
Window::MouseInsideProperty = UIProperty::MakeDefault<bool>(
	"MouseInside").Register<Window>();
const UIProperty
Window::CanvasProperty = UIProperty::MakeDefault<ICanvas>(
	"Canvas", UIActionTrigger::Render).Register<Window>();

namespace details
{
	using namespace std;

	thread_local static std::map<Handle, IWindow> _ManagedWindows;

	void onKeyDown(IWindow window, WParam w, LParam l)
	{
		KeyboardEventArgs args{
			window, (unsigned char)w, l & 0xFFFF, ((l & 0x40000000) == 0) ?
			KeyboardKeyState::Released : KeyboardKeyState::Pressed,
			KeyboardKeyState::Pressed
		};
		window->OnPreviewKeyDown(args);
	}
	void onKeyUp(IWindow& window, WParam w, LParam l)
	{
		KeyboardEventArgs args{
			window, (unsigned char)w, l & 0xFFFF, ((l & 0x40000000) == 0) ?
			KeyboardKeyState::Released : KeyboardKeyState::Pressed,
			KeyboardKeyState::Released
		};
		window->OnPreviewKeyUp(args);
	}
	
	MouseButton remapMouseButton(WParam w)
	{
		return
			(w & MK_CONTROL ? MouseButton::AuxControl : MouseButton::None) |
			(w & MK_SHIFT ? MouseButton::AuxShift : MouseButton::None) |
			(w & MK_LBUTTON ? MouseButton::Left : MouseButton::None) |
			(w & MK_RBUTTON ? MouseButton::Right : MouseButton::None) |
			(w & MK_MBUTTON ? MouseButton::Wheel : MouseButton::None) |
			(w & MK_XBUTTON1 ? MouseButton::X1 : MouseButton::None) |
			(w & MK_XBUTTON2 ? MouseButton::X2 : MouseButton::None);
	}

	Point getCursorPosition(LParam l)
	{
		return {
			static_cast<Float>(GetLowWord(l)),
			static_cast<Float>(GetHighWord(l))
		};
	}

	void onMouseMove(IWindow& window, Handle hWindow, WParam w, LParam l)
	{
		auto& ref = window->GetProperty<bool>(Window::MouseInsideProperty);
		if (!ref)
		{
			window->SetProperty(Window::MouseInsideProperty, true);
			TrackMouseEventArgs trackMouseEvent{ sizeof(TrackMouseEventArgs),
				TME_LEAVE, hWindow, (DWord)HOVER_DEFAULT };
			TrackMouseEvent(&trackMouseEvent);
			EventArgs args;
			window->OnMouseEnter(args);
		}
		MouseEventArgs args{
			window, getCursorPosition(l), remapMouseButton(w)
		};
		window->OnPreviewMouseMove(args);
	}
	void onMouseLeave(IWindow& window)
	{
		auto& ref = window->GetProperty<bool>(Window::MouseInsideProperty);
		if (ref)
		{
			window->SetProperty(Window::MouseInsideProperty, false);
			EventArgs args;
			window->OnMouseLeave(args);
		}
	}
	void onMouseClick(const IWindow& window, decltype(&Window::OnPreviewMouseUp) call, MouseButton button, WParam w, LParam l)
	{
		MouseButtonEventArgs args{
			window, getCursorPosition(l),  remapMouseButton(w), button
		};

		(window.Get().get()->*call)(args);
	}
	void onMouseWheel(const IWindow& window, WParam w, LParam l)
	{
		MouseWheelEventArgs args{
			window, getCursorPosition(l), int(GetHighWord(w))
		};
		window->OnMouseWheel(args);
	}

	void onCharInput(const IWindow& window, WParam w, LParam l)
	{
		CharInputEventArgs args;
		args.Char = char32_t(w);
		
		window->OnCharInput(args);
	}

	void onSizeChange(const IWindow& window, LParam l)
	{
		auto border = window->GetProperty<Thickness>(Window::BorderProperty);
		window->SetSize({
			static_cast<Float>(GetLowWord(l)),
			static_cast<Float>(GetHighWord(l))
		});
		EventArgs args;
		window->OnSizeChanged(args);
	}
}

_L_NS_END_GRAPHICS_UI

LResult __stdcall LiongPlus_Graphics_Window_WndProc(
	LiongPlus::Platform::Windows::Handle hWnd,
	unsigned msg,
	LiongPlus::Platform::Windows::WParam w,
	LiongPlus::Platform::Windows::LParam l)
{
	using namespace LiongPlus::Platform;
	using namespace LiongPlus::Graphics::UI;
	using namespace LiongPlus::Graphics::UI::details;
	
	auto& it = _ManagedWindows.find(hWnd);
	if (it == _ManagedWindows.end())
		return DefWindowProcW(hWnd, msg, w, l);;
	IWindow window = it->second;
	if (!window.IsValid())
	{
		_ManagedWindows.erase(hWnd);
		return DefWindowProcW(hWnd, msg, w, l);
	}

	switch (msg)
	{
		case WM_KEYUP: return onKeyUp(window, w, l), 0;
		case WM_KEYDOWN: return onKeyDown(window, w, l), 0;
		case WM_IME_CHAR: return onCharInput(window, w, l), 0;
		case WM_MOUSEMOVE: return onMouseMove(window, hWnd, w, l), 0;
		case WM_MOUSEWHEEL: return onMouseWheel(window, w, l), 0;
		case WM_MOUSELEAVE: return onMouseLeave(window), 0;
		case WM_LBUTTONUP: return onMouseClick(window, &Window::OnPreviewMouseUp, MouseButton::Left, w, l), true;
		case WM_MBUTTONUP: return onMouseClick(window, &Window::OnPreviewMouseUp, MouseButton::Wheel, w, l), true;
		case WM_RBUTTONUP: return onMouseClick(window, &Window::OnPreviewMouseUp, MouseButton::Right, w, l), true;
		case WM_XBUTTONUP: return onMouseClick(window, &Window::OnPreviewMouseUp,
			GetHighWord(w) == 0 ? MouseButton::X1 : MouseButton::X2,
			GetLowWord(w), l), true;
		case WM_LBUTTONDOWN: return onMouseClick(window, &Window::OnPreviewMouseDown, MouseButton::Left, w, l), true;
		case WM_MBUTTONDOWN: return onMouseClick(window, &Window::OnPreviewMouseDown, MouseButton::Wheel, w, l), true;
		case WM_RBUTTONDOWN: return onMouseClick(window, &Window::OnPreviewMouseDown, MouseButton::Right, w, l), true;
		case WM_XBUTTONDOWN: return onMouseClick(window, &Window::OnPreviewMouseDown,
			GetHighWord(w) == 0 ? MouseButton::X1 : MouseButton::X2,
				GetLowWord(w), l), true;
		case WM_SIZE:
			return onSizeChange(window, l), true;
		case WM_DPICHANGED:
			SetWindowPos(hWnd, 0,
				((Windows::Rect*)l)->Left, ((Windows::Rect*)l)->Top,
				((Windows::Rect*)l)->Right - ((Windows::Rect*)l)->Left, ((Windows::Rect*)l)->Bottom - ((Windows::Rect*)l)->Top,
				SWP_NOZORDER | SWP_NOACTIVATE);
			break;
		case WM_QUIT:
			_ManagedWindows.erase(hWnd);
		default: break;
	}
	return DefWindowProcW(hWnd, msg, w, l);
}

_L_NS_BEG_GRAPHICS_UI

//
// Window
//

bool Window::_Initialize()
{
	using namespace std;
	using namespace Platform;
	static default_random_engine generator;
	static uniform_int_distribution<int> distribution(0, 999'999'999);

	wstring className = L"WindowClass";
	wstring windowName = L"Window";
	className += to_wstring(distribution(generator));
	windowName += to_wstring(distribution(generator));

	DWord style = WS_OVERLAPPEDWINDOW;
	DWord exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	Handle inst;

	SetProcessDpiAwareness(ProcessDpiAwareness::PROCESS_PER_MONITOR_DPI_AWARE);

	Windows::Rect rect ={ 0, 0, __DefaultWidth, __DefaultHeight };
	AdjustWindowRectEx(&rect, style, false, exStyle);
	SetProperty(BorderProperty, Thickness{
		(Float)0. - (Float)rect.Left,
		(Float)0. - (Float)rect.Top,
		(Float)rect.Right - (Float)__DefaultWidth,
		(Float)rect.Bottom - (Float)__DefaultHeight });

	if (!(inst = GetModuleHandleW(nullptr))) return false;
	WindowClass clazz
	{
		CS_HREDRAW | CS_VREDRAW | CS_OWNDC, &::LiongPlus_Graphics_Window_WndProc,
		0, 0, inst, nullptr, nullptr, nullptr, nullptr, className.c_str()
	};
	Handle window;
	bool success = !!RegisterClassW(&clazz) &&
		!!(window = CreateWindowExW(exStyle,
			className.c_str(), windowName.c_str(), style,
			__DefaultX, __DefaultY,
			rect.Right - rect.Left, rect.Bottom - rect.Top,
			nullptr, nullptr, inst, nullptr));
	Metadata["WindowContext"].emplace<Handle>(window);
	Metadata["DeviceContext"].emplace<Handle>(GetDC(window));
	UpdateWindow(window);

	SetProperty(Window::SizeProperty, Size{ __DefaultWidth, __DefaultHeight });

	return success;
}

bool Window::_Finalize()
{
	auto handle = std::any_cast<Handle>(Metadata["WindowContext"]);
	if (handle == 0)
		return true;
	else
	{
		wchar_t className[256];
		return GetClassNameW(handle, className, 255) &&
			DestroyWindow(handle) &&
			UnregisterClassW(className, GetModuleHandleW(nullptr));
	}
}

Window::Window()
{
	if (!_Initialize())
		throw std::runtime_error("Unable to create window.");
}
Window::~Window()
{
	_Finalize();
}

void Window::EnterMessageLoop()
{
	using namespace std::chrono;

	Message msg{};

	while (msg.message != WM_QUIT)
	{
		Threading::Dispatcher::DoDispatchedTasks();
		while (PeekMessageA(&msg, 0, 0, 0, 1))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		std::this_thread::sleep_for(10us);
	}
}

void Window::SetTitle(const std::string& title)
{
	using namespace Serialization::Unicode;
	std::u32string utf32;
	if (TryFromUtf8(title.begin(), title.end(), utf32).IsError())
		return;
	std::u16string utf16;
	if (TryToUtf16(utf32.begin(), utf32.end(), utf16).IsError())
		return;
	SetWindowTextW(std::any_cast<Handle>(Metadata["WindowContext"]),
		(const wchar_t*)utf16.c_str());
}

ICanvas Window::GetCanvas()
{
	return GetProperty<ICanvas>(CanvasProperty);
}
void Window::SetCanvas(const ICanvas& canvas)
{
	SetProperty<ICanvas>(CanvasProperty, canvas);
	auto size = GetSize();
	canvas->Resize({ 0, 0, size.Width, size.Height });
}

void Window::Show() const
{
	ShowWindow(std::any_cast<Handle>(Metadata.at("WindowContext")), SW_NORMAL);
}

void Window::Hide() const
{
	ShowWindow(std::any_cast<Handle>(Metadata.at("WindowContext")), SW_HIDE);
}


void Window::PreRender(const ICanvas& canvas)
{
}
void Window::PostRender(const ICanvas& canvas)
{
}

void Window::OnSizeChanged(EventArgs& args)
{
	auto size = GetProperty<Size>(Window::SizeProperty);
	if (GetCanvas().IsValid())
		GetCanvas()->Resize({ 0, 0, size.Width, size.Height });
}

void Window::TriggerRender()
{
	auto canvas = GetCanvas();
	if (!canvas.IsValid()) return;
	canvas->Clear();
	Render(canvas);
	canvas->Submit();
}

void Window::TriggerLayout()
{
	Layout(GetProperty<Size>(SizeProperty));
}

void Window::TriggerLogicTree()
{
	LogicTree();
}

std::shared_ptr<Window> UIFactory<Window>::Create()
{
	auto rv = std::shared_ptr<Window>(new Window(), [](Window* ptr) {
		details::_ManagedWindows.erase(std::any_cast<Handle>(ptr->Metadata["WindowContext"]));
		UIUpdater::_Unregister(IWindow(ptr->shared_from_this()));
		return ptr;
	});
	// Register as window event receiver.
	UIUpdater::_Register(rv);
	details::_ManagedWindows[std::any_cast<Handle>(rv->Metadata["WindowContext"])] = rv;
	return rv;
}

_L_NS_END_GRAPHICS_UI

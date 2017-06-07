// File: Window.cpp
// Author: Rendong Liang (Liong)
#include <chrono>
#include <utility>
#include <random>
#include "Window.hpp"
#include "UIUpdater.UIAction.hpp"

_L_NS_BEG_GRAPHICS_UI

const UIProperty
Window::MouseInsideProperty = UIProperty::MakeDefault<bool>(
	"MouseInside").Register<Window>();
const UIProperty
Window::WindowContextProperty = UIProperty::MakeDefault<HWND>(
	"WindowContext").Register<Window>();
const UIProperty
Window::CanvasProperty = UIProperty::MakeDefault<ICanvas>(
	"Canvas", UIActionTrigger::Render).Register<Window>();


namespace details
{
	using namespace std;

	thread_local static std::map<HWND, IWindow> _ManagedWindows;

	void onKeyDown(IWindow window, WPARAM w, LPARAM l)
	{
		KeyboardEventArgs args{
			window, (unsigned char)w, l & 0xFFFF, ((l & 0x40000000) == 0) ?
			KeyboardKeyState::Released : KeyboardKeyState::Pressed,
			KeyboardKeyState::Pressed
		};
		window->OnPreviewKeyDown(args);
	}
	void onKeyUp(IWindow& window, WPARAM w, LPARAM l)
	{
		KeyboardEventArgs args{
			window, (unsigned char)w, l & 0xFFFF, ((l & 0x40000000) == 0) ?
			KeyboardKeyState::Released : KeyboardKeyState::Pressed,
			KeyboardKeyState::Released
		};
		window->OnPreviewKeyUp(args);
	}
	
	MouseButton remapMouseButton(WPARAM w)
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

	void onMouseMove(IWindow& window, HWND hWindow, WPARAM w, LPARAM l)
	{
		auto& ref = window->GetProperty<bool>(Window::MouseInsideProperty);
		if (!ref)
		{
			window->SetProperty(Window::MouseInsideProperty, true);
			TRACKMOUSEEVENT trackMouseEvent{ sizeof(TRACKMOUSEEVENT), TME_LEAVE, hWindow, HOVER_DEFAULT };
			TrackMouseEvent(&trackMouseEvent);
			EventArgs args;
			window->OnMouseEnter(args);
		}
		MouseEventArgs args{
			window,{
				static_cast<Float>(GET_X_LPARAM(l)),
				static_cast<Float>(GET_Y_LPARAM(l))
			}, remapMouseButton(w)
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
	void onMouseClick(const IWindow& window, decltype(&Window::OnPreviewMouseUp) call, MouseButton button, WPARAM w, LPARAM l)
	{
		MouseButtonEventArgs args{
			window,{
				static_cast<Float>(GET_X_LPARAM(l)),
				static_cast<Float>(GET_Y_LPARAM(l))
			},  remapMouseButton(w), button
		};

		(window.Get().get()->*call)(args);
	}
	void onMouseWheel(const IWindow& window, WPARAM w, LPARAM l)
	{
		MouseWheelEventArgs args{
			window,{
				static_cast<Float>(GET_X_LPARAM(l)),
				static_cast<Float>(GET_Y_LPARAM(l))
			}, GET_WHEEL_DELTA_WPARAM(w)
		};
		window->OnMouseWheel(args);
	}

	void onSizeChange(const IWindow& window, LPARAM l)
	{
		auto border = window->GetProperty<Thickness>(Window::BorderProperty);
		window->SetSize(Size{ (float)LOWORD(l), (float)HIWORD(l) });
		EventArgs args;
		window->OnSizeChanged(args);
	}

	void startReceivingWindowMessage(const std::shared_ptr<Window>& window)
	{
		// Register as window event receiver.
		details::_ManagedWindows[window->GetProperty<HWND>(Window::WindowContextProperty)] = window;
		UIUpdater::_Register(window);
	}

	void stopReceivingWindowMessage(Window* window)
	{
		auto hwnd = window->GetProperty<HWND>(Window::WindowContextProperty);
		if (!!hwnd)
		{
			UIUpdater::_Unregister(IWindow(window->shared_from_this()));
			details::_ManagedWindows.erase(hwnd);
		}
	}
}

_L_NS_END_GRAPHICS_UI

LRESULT __stdcall LiongPlus_Graphics_Window_WndProc
(HWND hWnd, UINT msg, WPARAM w, LPARAM l)
{
	using namespace LiongPlus::Graphics::UI;
	using namespace LiongPlus::Graphics::UI::details;
	
	auto& it = _ManagedWindows.find(hWnd);
	if (it == _ManagedWindows.end())
		return DefWindowProc(hWnd, msg, w, l);;
	IWindow window = it->second;
	if (!window.IsValid())
	{
		_ManagedWindows.erase(hWnd);
		return DefWindowProc(hWnd, msg, w, l);
	}

	switch (msg)
	{
		case WM_KEYUP: return onKeyUp(window, w, l), 0;
		case WM_KEYDOWN: return onKeyDown(window, w, l), 0;
		case WM_MOUSEMOVE: return onMouseMove(window, hWnd, w, l), 0;
		case WM_MOUSEWHEEL: return onMouseWheel(window, w, l), 0;
		case WM_MOUSELEAVE: return onMouseLeave(window), 0;
		case WM_LBUTTONUP: return onMouseClick(window, &Window::OnPreviewMouseUp, MouseButton::Left, w, l), true;
		case WM_MBUTTONUP: return onMouseClick(window, &Window::OnPreviewMouseUp, MouseButton::Wheel, w, l), true;
		case WM_RBUTTONUP: return onMouseClick(window, &Window::OnPreviewMouseUp, MouseButton::Right, w, l), true;
		case WM_XBUTTONUP: return onMouseClick(window, &Window::OnPreviewMouseUp,
				GET_XBUTTON_WPARAM(w) == 0 ? MouseButton::X1 : MouseButton::X2,
				GET_KEYSTATE_WPARAM(w), l), true;
		case WM_LBUTTONDOWN: return onMouseClick(window, &Window::OnPreviewMouseDown, MouseButton::Left, w, l), true;
		case WM_MBUTTONDOWN: return onMouseClick(window, &Window::OnPreviewMouseDown, MouseButton::Wheel, w, l), true;
		case WM_RBUTTONDOWN: return onMouseClick(window, &Window::OnPreviewMouseDown, MouseButton::Right, w, l), true;
		case WM_XBUTTONDOWN: return onMouseClick(window, &Window::OnPreviewMouseDown,
				GET_XBUTTON_WPARAM(w) == 0 ? MouseButton::X1 : MouseButton::X2,
				GET_KEYSTATE_WPARAM(w), l), true;
		case WM_SIZE:
			return onSizeChange(window, l), true;
		case WM_DPICHANGED:
			SetWindowPos(hWnd, NULL,
				((RECT*)l)->left, ((RECT*)l)->top,
				((RECT*)l)->right - ((RECT*)l)->left, ((RECT*)l)->bottom - ((RECT*)l)->top,
				SWP_NOZORDER | SWP_NOACTIVATE);
			break;
		case WM_QUIT:
			_ManagedWindows.erase(hWnd);
		default: break;
	}
	return DefWindowProc(hWnd, msg, w, l);
}

_L_NS_BEG_GRAPHICS_UI

//
// Window
//

bool Window::_Initialize()
{
	using namespace std;
	static default_random_engine generator;
	static uniform_int_distribution<int> distribution(0, 999'999'999);

#ifdef _UNICODE
	wstring className = L"WindowClass";
	wstring windowName = L"Window";
	className += to_wstring(distribution(generator));
	windowName += to_wstring(distribution(generator));
#else
	string className = "WindowClass";
	string windowName = "Window";
	className += to_string(distribution(generator));
	windowName += to_string(distribution(generator));
#endif

	DWORD style = WS_OVERLAPPEDWINDOW,
		exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	HINSTANCE inst;

	SetProcessDpiAwareness(PROCESS_DPI_AWARENESS::PROCESS_PER_MONITOR_DPI_AWARE);

	RECT rect ={ 0, 0, __DefaultWidth, __DefaultHeight };
	AdjustWindowRectEx(&rect, style, false, exStyle);
	SetProperty(BorderProperty, Thickness{
		0.0 - rect.left,
		0.0 - rect.top,
		(Float)rect.right - __DefaultWidth,
		(Float)rect.bottom - __DefaultHeight });

	if (!(inst = GetModuleHandle(nullptr))) return false;
	WNDCLASS clazz
	{
		CS_HREDRAW | CS_VREDRAW | CS_OWNDC, &LiongPlus_Graphics_Window_WndProc,
		0, 0, inst, LoadIcon(nullptr, IDI_APPLICATION), LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, className.c_str()
	};
	HWND window;
	bool success = !!RegisterClass(&clazz) &&
		!!(window = CreateWindowEx(exStyle, className.c_str(), windowName.c_str(), style,
			__DefaultX, __DefaultY,
			rect.right - rect.left, rect.bottom - rect.top,
			nullptr, nullptr, inst, nullptr));
	SetProperty(WindowContextProperty, window);
	UpdateWindow(window);

	SetProperty(Window::SizeProperty, Size{ __DefaultWidth, __DefaultHeight });

	return success;
}

bool Window::_Finalize()
{
	HWND window = GetProperty<HWND>(WindowContextProperty);
#ifdef _UNICODE
	wchar_t className[256];
#else
	char className[256];
#endif
	return window == 0 ? true : !!DestroyWindow(window) &&
		!!GetClassName(window, className, 255) &&
		!!UnregisterClass(className, GetModuleHandle(nullptr));
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

	MSG msg{};

	while (msg.message != WM_QUIT)
	{
		Dispatcher::DoDispatchedTasks();
		while (PeekMessage(&msg, 0, 0, 0, 1))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		std::this_thread::sleep_for(1ms);
	}
}

void Window::SetTitle(const std::string& title)
{
	SetWindowTextA(GetProperty<HWND>(Window::WindowContextProperty), title.c_str());
}

ICanvas Window::GetCanvas()
{
	return GetProperty<ICanvas>(CanvasProperty);
}
void Window::SetCanvas(const ICanvas& canvas)
{
	SetProperty<ICanvas>(CanvasProperty, canvas);
	auto size = GetProperty<Size>(SizeProperty);
	canvas->Resize({ 0, 0, size.Width, size.Height });
}

void Window::Show()
{
	ShowWindow(GetProperty<HWND>(WindowContextProperty), SW_NORMAL);
}

void Window::Hide()
{
	ShowWindow(GetProperty<HWND>(WindowContextProperty), SW_HIDE);
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

DeviceContext Window::GetDeviceContext()
{
	return GetDC(GetProperty<HWND>(WindowContextProperty));
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
	std::shared_ptr<Window> ptr(new Window(), [](Window* ptr) { details::stopReceivingWindowMessage(ptr); delete ptr; });
	details::startReceivingWindowMessage(ptr);
	return ptr;
}

_L_NS_END_GRAPHICS_UI

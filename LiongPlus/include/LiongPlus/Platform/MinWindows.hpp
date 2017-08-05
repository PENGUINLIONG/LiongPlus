// File: MinWindows.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <cstdint>
#include "_"

/*
 * This file provided the minimum number of Windows APIs that allows
 *   LiongPlus to work properly. It's too dirty to use 'windows.h'.
 *   It's not recommended to use the entire platform specific
 *   namespace because of the mess.
 * All string-related functions use the Unicode variant to avoid
 *   performance hit.
 */
_L_NS_BEG_PLATFORM

#ifdef __L_WINDOWS
namespace Windows
{
	// Type definitions.

#if defined(_WIN64)
	using UintPtr = uint64_t;
	using IntPtr = int64_t;
#else
	using UintPtr = uint32_t;
	using IntPtr = int32_t;
#endif
	using WParam = UintPtr;
	using LParam = IntPtr;
	using LResult = IntPtr;
	using Handle = void*;

	using WndProc = LResult (__stdcall *)(Handle, unsigned, WParam, LParam);
	using Proc = int(__stdcall *)();

	using Bool = int32_t;
	using Word = uint16_t;
	using DWord = uint32_t;

	using Atom = Word;

	struct Rect
	{
		int32_t Left, Top, Right, Bottom;
	};
	struct Point
	{
		int32_t X, Y;
	};

	enum
	{
		WM_KEYDOWN = 0x0100,
		WM_KEYUP = 0x0101,

		WM_IME_CHAR = 0x0286,

		WM_MOUSEMOVE = 0x0200,
		WM_LBUTTONDOWN = 0x0201,
		WM_LBUTTONUP = 0x0202,
		WM_RBUTTONDOWN = 0x0204,
		WM_RBUTTONUP = 0x0205,
		WM_MBUTTONDOWN = 0x0207,
		WM_MBUTTONUP = 0x0208,
		WM_XBUTTONDOWN = 0x020B,
		WM_XBUTTONUP = 0x020C,

		WM_MOUSEWHEEL = 0x020A,
		WM_MOUSELEAVE = 0x02A3,

		WM_SIZE = 0x0005,
		WM_DPICHANGED = 0x02E0,

		WM_QUIT = 0x0012,
	};

	enum
	{
		MK_LBUTTON = 0x0001,
		MK_RBUTTON = 0x0002,
		MK_SHIFT = 0x0004,
		MK_CONTROL = 0x0008,
		MK_MBUTTON = 0x0010,
		MK_XBUTTON1 = 0x0020,
		MK_XBUTTON2 = 0x0040,
	};


	// APIs used by LiongPlus.


	// winuser.h

	extern "C" __declspec(dllimport)
		LResult __stdcall DefWindowProcW(
			Handle window, unsigned message,
			WParam wParam, LParam lParam);

	enum
	{
		SWP_NOZORDER = 0x0004,
		SWP_NOACTIVATE = 0x0010,
	};
	extern "C" __declspec(dllimport)
		Bool __stdcall SetWindowPos(Handle window,
			Handle windowInsertAfter,
			int x, int y, int width, int height, unsigned flags);

	enum
	{
		HOVER_DEFAULT = 0xFFFFFFFF,

		TME_LEAVE = 0x00000002,
	};
	struct TrackMouseEventArgs
	{
		DWord structSize;
		DWord flags;
		Handle  target;
		DWord hoverTime;
	};
	extern "C" __declspec(dllimport)
		Bool __stdcall TrackMouseEvent(TrackMouseEventArgs* args);

	extern "C" __declspec(dllimport)
		Bool __stdcall AdjustWindowRectEx(Rect* rect,
			DWord dwStyle, int menu, DWord exStyle);

	extern "C" __declspec(dllimport)
		Handle __stdcall GetModuleHandleW(const wchar_t* name);

	enum
	{
		CS_VREDRAW = 0x0001,
		CS_HREDRAW = 0x0002,
		CS_OWNDC = 0x0003,
	};
	struct WindowClass {
		unsigned Style;
		WndProc Procedure;
		int ClassExtra;
		int WindowExtra;
		Handle Instance;
		Handle Icon;
		Handle Cursor;
		Handle Background;
		const wchar_t* MenuName;
		const wchar_t* ClassName;
	};
	extern "C" __declspec(dllimport)
		Atom __stdcall RegisterClassW(const WindowClass* windowClass);
	extern "C" __declspec(dllimport)
		Bool __stdcall UnregisterClassW(const wchar_t* className, Handle instance);
	extern "C" __declspec(dllimport)
		int __stdcall GetClassNameW(Handle hWnd, wchar_t* className, int maxCount);

	enum
	{
		WS_OVERLAPPEDWINDOW = (0x00000000L | 0x00C00000L | 0x00080000L | 0x00040000L | 0x00020000L | 0x00010000L),
	};
	enum
	{
		WS_EX_APPWINDOW = 0x00040000L,
		WS_EX_WINDOWEDGE = 0x00000100L,
	};
	extern "C" __declspec(dllimport)
		Handle __stdcall CreateWindowExW(
			DWord exStyle, const wchar_t* className,
			const wchar_t* windowName,
			DWord dwStyle, int x, int y, int width, int height,
			Handle parent, Handle menu, Handle instance,
			void* param);
	extern "C" __declspec(dllimport)
		Bool __stdcall UpdateWindow(Handle window);
	extern "C" __declspec(dllimport)
		Bool __stdcall DestroyWindow(Handle window);
	enum
	{
		SW_NORMAL = 1,
		SW_HIDE = 0,
	};
	extern "C" __declspec(dllimport)
		Bool __stdcall ShowWindow(Handle window, int cmdShow);

	struct Message
	{
		Handle hwnd;
		unsigned message;
		WParam wParam;
		LParam lParam;
		DWord time;
		Point pt;
	};
	extern "C" __declspec(dllimport)
		Bool __stdcall PeekMessageA(
			Message* msg, Handle window,
			unsigned msgFilterMin,
			unsigned msgFilterMax,
			unsigned removeMsg);
	extern "C" __declspec(dllimport)
		Bool __stdcall TranslateMessage(const Message* msg);
	extern "C" __declspec(dllimport)
		LResult __stdcall DispatchMessageW(const Message* msg);

	extern "C" __declspec(dllimport)
		Bool __stdcall SetWindowTextW(
			Handle window, const wchar_t* string);

	extern "C" __declspec(dllimport)
		Bool  __stdcall SwapBuffers(Handle dc);

	// Utility.

	template<typename T>
	constexpr T GetHighWord(T val)
	{
		return (val >> 16) & 0xFFFF;
	}
	template<typename T>
	constexpr T GetLowWord(T val)
	{
		return val & 0xFFFF;
	}

#ifdef __L_OPENGL_ACC
	struct PixelFormatDescriptor
	{
		Word StructSize;
		Word Version;
		DWord Flags;
		Byte PixelType;
		Byte ColorBits;
		Byte RedBits;
		Byte RedShift;
		Byte GreenBits;
		Byte GreenShift;
		Byte BlueBits;
		Byte BlueShift;
		Byte AlphaBits;
		Byte AlphaShift;
		Byte AccumBits;
		Byte AccumRedBits;
		Byte AccumGreenBits;
		Byte AccumBlueBits;
		Byte AccumAlphaBits;
		Byte DepthBits;
		Byte StencilBits;
		Byte AuxBuffers;
		Byte LayerType;
		Byte Reserved;
		DWord LayerMask;
		DWord VisibleMask;
		DWord DamageMask;
	};
	enum
	{
		PFD_TYPE_RGBA = 0,
		PFD_TYPE_COLORINDEX = 1,
	};
	enum
	{
		PFD_MAIN_PLANE = 0,
	};
	enum
	{
		PFD_DOUBLEBUFFER = 0x00000001,
		PFD_DRAW_TO_WINDOW = 0x00000004,
		PFD_SWAP_EXCHANGE = 0x00000200,

		PFD_SUPPORT_OPENGL = 0x00000020,
		PFD_SUPPORT_DIRECTDRAW = 0x00002000,
		
		PFD_DIRECT3D_ACCELERATED = 0x00004000,
	};
	extern "C" __declspec(dllimport)
		int __stdcall ChoosePixelFormat(Handle dc, const PixelFormatDescriptor* pfd);
	extern "C" __declspec(dllimport)
		Bool __stdcall SetPixelFormat(Handle hdc,
			int format, const PixelFormatDescriptor* pfd);
#endif

	extern "C" __declspec(dllimport)
		Handle __stdcall GetModuleHandleW(const wchar_t* moduleName);

	extern "C" __declspec(dllimport)
		Handle __stdcall GetDC(Handle window);

	// ShellScalingApi.h
	enum ProcessDpiAwareness {
		PROCESS_DPI_UNAWARE = 0,
		PROCESS_SYSTEM_DPI_AWARE = 1,
		PROCESS_PER_MONITOR_DPI_AWARE = 2
	};
	extern "C" __declspec(dllimport)
		Handle __stdcall SetProcessDpiAwareness(ProcessDpiAwareness value);
}

#endif // __L_WINDOWS

_L_NS_END_PLATFORM

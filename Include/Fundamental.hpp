// File: Fundamental.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Fundamental
#define _L_Fundamental
/* __________________________________________________________ */
/*|                                                          |*/
/*|                                                          |*/
/*|                       Configuration                      |*/
/*|                                                          |*/
/*|__________________________________________________________|*/
/*                                                            */
/* __________________________________________________________ */
/*|                                                          |*/
/*|                          Compiler                        |*/
/*|__________________________________________________________|*/
/*|                                                          |*/
/*|*/                     #define _L_MSVC                  /*|*/
/*|                       #define _L_GCC                   /*|*/
/*|__________________________________________________________|*/
/*                                       */
/* __________________________________________________________ */
/*|                                                          |*/
/*|                 Platform & Runtime System                |*/
/*|__________________________________________________________|*/
/*|                       #define _L_ASM                   /*|*/
/*|                       #define _L_X64                   /*|*/
/*|*/                   #define _L_WINDOWS                 /*|*/
/*|                      #define _L_MAC_OS                 /*|*/
/*|                      #define _L_LINUX                  /*|*/
/*|__________________________________________________________|*/
/*                                                            */
/* __________________________________________________________ */
/*|                                                          |*/
/*|                       Mode & Features                    |*/
/*|__________________________________________________________|*/
/*|*/                    #define _L_DEBUG                  /*|*/
/*|*/                   #define _L_RELEASE                 /*|*/
/*|*/                #define _L_EFFICIENT_IO               /*|*/
/*|*/                    #define _L_LINQ                   /*|*/
/*|*/               #define _L_STABILITY_CHECK             /*|*/
/*|*/                   #define _L_SYNTAX                  /*|*/
/*|*/                  #define _L_GRAPHICS                 /*|*/
/*|*/                    #define _L_NET                    /*|*/
/*|__________________________________________________________|*/
/*                                                            */
/* __________________________________________________________ */
/*|                                                          |*/
/*|                         W T F ?                          |*/
/*|__________________________________________________________|*/
/*|                   #define protected public             /*|*/
/*|                    #define private public              /*|*/
/*|__________________________________________________________|*/

#ifdef _L_WINDOWS
#include <Windows.h>
#include <windowsx.h>
#include <WinUser.h>

#ifdef _L_GRAPHICS
#include <gl\GL.h>
#include <gl\GLU.h>
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#endif // _L_GRAPHICS

#define _LT(x) L##x
#define _L_Char wchar_t

#define _L_putchar(c) putwchar(c)
#define _L_getchar() reinterpret_cast<wchar_t>(getwchar())
#define _L_puts(str) _putws(str)
#define _L_fputs(str, file) fputws(str, file)
#else // _L_WINDOWS // !_L_WINDOWS
#define _LT(x)
#define _L_Char char

#define _L_putchar(c) putchar(c)
#define _L_getchar(c) getchar(c)
#define _L_puts(str) puts(str)
#define _L_fputs(str, file) fputs(str, file)
#endif // !_L_WINDOWS

#ifndef _L_DEBUG
#define NDEBUG
#endif

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cwchar>
#include <cassert>
#include <initializer_list>

#ifdef _L_MSVC
#pragma warning(disable: 4584)
#endif

#ifdef _L_SYNTAX

#define delegate(x) using x = Delegate
#define foreach for
#define in :
#define var auto

#endif

namespace LiongPlus
{
	typedef char Byte;

	struct Size
	{
		int Width, Height;
	};

	struct Point
	{
		int X, Y;
	};

	struct Rect
	{
		int Left, Top, Right, Bottom;
	};

	template<typename T>
	struct RemoveRef
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveRef<T&>
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveRef<T&&>
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveConst
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveConst<const T>
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveVolatile
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveVolatile<volatile T>
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveDebuff
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveDebuff<T&>
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveDebuff<T&&>
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveDebuff<const T>
	{
		typedef T Type;
	};

	template<typename T>
	struct RemoveDebuff<volatile T>
	{
		typedef T Type;
	};

	template<typename T>
	typename RemoveRef<T>::Type&& Move(T&& obj)
	{
		return (typename RemoveRef<T>::Type&&)obj;
	}

	template<typename T>
	typename T&& Forward(typename RemoveRef<T>::Type& obj)
	{
		return (T&&)obj;
	}

	template<typename T>
	typename T&& Forward(typename RemoveRef<T>::Type&& obj)
	{
		return (T&&)obj;
	}

	template<typename T>
	void Swap(T& x, T& y)
	{
		T temp = x;
		x = y;
		y = temp;
	}

////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Base type of all int fundamental classes.
	/// </summary>
	class Object
	{
	public:
		Object()
		{
		}
		virtual ~Object() = 0;
	};

	/// <summary>
	/// Disable copy constructor and operator= as it is depended on another object.
	/// </summary>
	class DependentObject
		: public Object
	{
	public:
		DependentObject(const DependentObject&) = delete;
		DependentObject(DependentObject&&) = delete;
		DependentObject& operator=(const DependentObject&) = delete;
		DependentObject& operator=(DependentObject&&) = delete;
		DependentObject()
		{
		}
	};

	/// <summary>
	/// Base type of all int fundamental interfaces.
	/// </summary>
	class Interface
		: protected DependentObject
	{
	protected:
		virtual ~Interface()
		{
		}
	};
}
#endif
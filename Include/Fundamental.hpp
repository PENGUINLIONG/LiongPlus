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
/*|*/                    #define _L_LINQ                   /*|*/
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

#pragma execution_character_set("utf-8")

#ifdef _L_WINDOWS
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <WinDNS.h>
#include <Windows.h>
#include <windowsx.h>
#include <WinUser.h>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Dnsapi.lib")

#ifdef _L_GRAPHICS
#include <gl\GL.h>
#include <gl\GLU.h>
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#endif // _L_GRAPHICS

#else // _L_WINDOWS // !_L_WINDOWS
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif // !_L_WINDOWS

#ifndef _L_DEBUG
#define NDEBUG
#endif

#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cwchar>
#include <cassert>
#include <codecvt>
#include <exception>
#include <functional>
#include <future>
#include <set>
#include <initializer_list>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

#ifdef _L_MSVC
#pragma warning(disable: 4002) // Too many actual params for 'assert'.
#pragma warning(disable: 4244) // Loss of data.
#pragma warning(disable: 4267) // Loss of data.
#pragma warning(disable: 4584)
#endif

#ifdef _L_SYNTAX

#define delegate(x) using x = std::function
#define foreach for
#define in :
#define var auto

#endif

#define _L_ABSTRACT

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

////////////////////////////////////////////////////////////////////////////////////////
	
// This is an example of delegate announcement with special syntax of LiongPlus.
#ifdef _L_SYNTAX
	template<typename ... Ts>
	delegate(Action) < void(Ts ...) >;
#else
	template<typename ... Ts>
	using Action = std::function<void(Ts ...)>;
#endif
	template<typename TResult, typename ... TArgs>
	using Func = std::function<TResult(TArgs ...)>;

	template<typename T>
	using Predicate = std::function<bool(T)>;

	template<typename TInput, typename TOutput>
	using Converter = std::function<TOutput(const TInput)>;

	template<typename T>
	using Comparison = std::function<int(T, T)>;
}
#endif
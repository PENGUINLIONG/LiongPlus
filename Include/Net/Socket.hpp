// File: Socket.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Socket
#define _L_Socket
#include "../Fundamental.hpp"
#ifdef _L_NET
#include "../Exception.hpp"
#include "../IDisposable.hpp"
#include <mutex>

#ifdef _L_WINDOWS
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#include <WinSock2.h>
#include <WS2tcpip.h>
#else // !_L_WINDOWS
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

namespace LiongPlus
{
	namespace Net
	{
		class SocketException
			: public Exception
		{
		public:
			SocketException(const char* description = "");
		};

		/// <summary>
		/// A quick access to initialize WinSock library.
		/// </summary>
		/// <remarks>
		/// It's suggested that to quote this when you want to use socket-associated functionalities. Use this class as following code.
		/// <c>
		/// int main()
		/// {
		///		SocketInitialization init;
		///		// Use of socket...
		/// }
		/// </c>
		/// </remarks>
		class SocketInitialization
		{
		public:
			SocketInitialization();
			~SocketInitialization();
		};

		enum class AddressFamily
		{
			InterNetwork,
			InterNetworkV6,
			Unspecified,
			Unknown
		};

		enum class ProtocolType
		{
			IP,
			IPv4,
			IPv6,
			Tcp,
			Udp,
			Unspecified,
			Unknown
		};

		class Socket
			: public IDisposable
		{
		public:
			Socket();
			~Socket();

			// IDisposable

			void Dispose();
		};
	}
}
#endif
#endif
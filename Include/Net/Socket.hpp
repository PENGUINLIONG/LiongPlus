// File: Socket.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Socket
#define _L_Socket
#include "../Fundamental.hpp"
#ifdef _L_NET
#include "../Exception.hpp"
#include "../IDisposable.hpp"
#include <mutex>

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
		/// long main()
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
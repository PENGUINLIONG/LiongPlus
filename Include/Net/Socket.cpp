#include "Socket.hpp"

namespace LiongPlus
{
	namespace Net
	{
		SocketException::SocketException(const char* description)
			: Exception(description)
		{
		}

		SocketInitialization::SocketInitialization()
		{
#ifdef _L_WINDOWS
			WSADATA wsa_data;
			if (WSAStartup(MAKEWORD(1, 1), &wsa_data) != 0)
				throw SocketException("WinSock Library initialization failed.");
#endif
		}

		SocketInitialization::~SocketInitialization()
		{
			WSACleanup();
		}
	}
}
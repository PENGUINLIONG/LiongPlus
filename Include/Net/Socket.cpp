// File: Socket.cpp
// Author: Rendong Liang (Liong)
#include "Socket.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using std::swap;

		StartUpNetModule::StartUpNetModule()
		{
#ifdef _L_WINDOWS
			if (WSAStartup(MAKEWORD(2, 2), &_Data) != 0)
				throw std::runtime_error("Failed in starting up WinSock.");
#endif
		}
		StartUpNetModule::~StartUpNetModule()
		{
#ifdef _L_WINDOWS
			if (WSACleanup() != 0)
				throw std::runtime_error("Failed in cleaning up WinSock.");
#endif
		}



		Socket::Socket()
			: _HSocket(-1)
		{
		}
		Socket::Socket(HSocket hSocket)
			: _HSocket(hSocket)
		{
		}
		Socket::Socket(int addressFamily, int type, int protocal)
			: _HSocket(-1)
		{
			_HSocket = socket(addressFamily, type, protocal);
		}
		Socket::Socket(Socket&& instance)
			: Socket()
		{
			swap(_HSocket, instance._HSocket);
		}
		Socket::~Socket()
		{
#ifdef _L_WINDOWS
			if (_HSocket != INVALID_SOCKET)
#else
			if (_HSocket >= 0)
#endif
				Close();
		}

		Socket& Socket::operator=(Socket&& instance)
		{
			swap(_HSocket, instance._HSocket);
			return *this;
		}

		Socket Socket::Accept(SocketAddress& addr)
		{
			int len = addr.Length();
			return accept(_HSocket, (sockaddr*)addr.Field(), &len);
		}

		bool Socket::Bind(const SocketAddress& addr)
		{
			return !IsErrorOccured(::bind(_HSocket, (const sockaddr*)addr.Field(), addr.Length()));
		}

		bool Socket::Close()
		{
#ifdef _L_WINDOWS
			if (_HSocket != INVALID_SOCKET )
			{
				if (IsErrorOccured(closesocket(_HSocket)))
#else
			if (_HSocket >= 0 )
			{
				if (IsErrorOccured(close(_HSocket)))
#endif
					return false;
				_HSocket = -1;
			}
			return true;
		}

		bool Socket::Connect(const SocketAddress& addr)
		{
			return !IsErrorOccured(connect(_HSocket, (const sockaddr*)addr.Field(), addr.Length()));
		}

		bool Socket::Listen(int backlog)
		{
			return !IsErrorOccured(listen(_HSocket, backlog));
		}

		bool Socket::Send(const Buffer& buffer, size_t offset)
		{
			return send(_HSocket, buffer.Field() + offset, buffer.Length() - offset, 0) >= 0;
		}
		bool Socket::Send(const Buffer& buffer, size_t offset, int flags)
		{
			return send(_HSocket, buffer.Field() + offset, buffer.Length() - offset, flags) >= 0;
		}

		bool Socket::SetSendTimeOut(uint32_t ms)
		{
#ifdef _L_WINDOWS
			return SetOption(SO_RCVTIMEO, ms);
#else
			Buffer buffer(sizeof(timeval));
			*(reinterpret_cast<timeval*>(buffer.Field())) = { 0, ms };
			return SetOption(SO_RCVTIMEO, buffer);
#endif
		}

		bool Socket::SetOption(int flags, uint32_t value)
		{
			return setsockopt(_HSocket, SOL_SOCKET, flags, (const char *)&value, sizeof(uint32_t)) >= 0;
		}
		bool Socket::SetOption(int flags, Buffer& value)
		{
			return setsockopt(_HSocket, SOL_SOCKET, flags, value.Field(), value.Length()) >= 0;
		}

		bool Socket::SetReceiveTimeOut(uint32_t ms)
		{
#ifdef _L_WINDOWS
			return SetOption(SO_SNDTIMEO, ms);
#else
			Buffer buffer(sizeof(timeval));
			*(reinterpret_cast<timeval*>(buffer.Field())) = { 0, ms };
			return SetOption(SO_SNDTIMEO, buffer);
#endif
		}

		bool Socket::Receive(Buffer& buffer, size_t offset)
		{
			return recv(_HSocket, buffer.Field() + offset, buffer.Length() - offset, 0) >= 0;
		}
		bool Socket::Receive(Buffer& buffer, size_t offset, int flags)
		{
			return recv(_HSocket, buffer.Field() + offset, buffer.Length() - offset, flags) >= 0;
		}

		bool Socket::SendTo(Buffer& buffer, size_t offset, const SocketAddress& addr)
		{
			return sendto(_HSocket, buffer.Field() + offset, buffer.Length() - offset, 0, (const sockaddr*)addr.Field(), addr.Length()) >= 0;
		}
		bool Socket::SendTo(Buffer& buffer, size_t offset, const SocketAddress& addr, int flags)
		{
			return sendto(_HSocket, buffer.Field() + offset, buffer.Length() - offset, flags, (const sockaddr*)addr.Field(), addr.Length()) >= 0;
		}

		bool Socket::ReceiveFrom(Buffer& buffer, size_t offset, SocketAddress& addr)
		{
			int len = addr.Length();
			return recvfrom(_HSocket, buffer.Field() + offset, buffer.Length() - offset, 0, (sockaddr*)addr.Field(), &len) >= 0;
		}
		bool Socket::ReceiveFrom(Buffer& buffer, size_t offset, SocketAddress& addr, int flags)
		{
			int len = addr.Length();
			return recvfrom(_HSocket, buffer.Field() + offset, buffer.Length() - offset, flags, (sockaddr*)addr.Field(), &len) >= 0;
		}

		bool Socket::IsValid() const
		{
#ifdef _L_WINDOWS
			return _HSocket != INVALID_SOCKET;
#else
			return _HSocket >= 0;
#endif
		}

		// Private

		bool Socket::IsErrorOccured(int code)
		{
#ifdef _L_WINDOWS
			return code != 0;
#else
			return code < 0;
#endif
		}
	}
}
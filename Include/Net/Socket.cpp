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
#ifdef _L_WINDOWS
			if (_HSocket == INVALID_SOCKET)
#else
			if (_HSocket < 0)
#endif
				throw std::runtime_error("Failed in creating socket.");
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
			HSocket code = accept(_HSocket, (sockaddr*)addr.Field(), &len);
#ifdef _L_WINDOWS
			if (code == INVALID_SOCKET)
#else
			if (code < 0)
#endif
				throw std::runtime_error("Failed in accepting incoming connection.");
			else return Socket(code);
		}

		void Socket::Bind(const SocketAddress& addr)
		{
			if (IsErrorOccured(bind(_HSocket, (const sockaddr*)addr.Field(), addr.Length())))
				throw std::runtime_error("Failed in binding with a certain address.");
		}

		void Socket::Close()
		{
			if (_HSocket >= 0)
			{
#ifdef _L_WINDOWS
				if (IsErrorOccured(closesocket(_HSocket)))
#else
				if (IsErrorOccured(close(_HSocket)))
#endif
					throw std::runtime_error("Failed in closing socket.");
				_HSocket = -1;
			}
			else throw
				std::logic_error("Socket already closed");
		}

		void Socket::Connect(const SocketAddress& addr)
		{
			if (IsErrorOccured(connect(_HSocket, (const sockaddr*)addr.Field(), addr.Length())))
				throw std::runtime_error("Failed in connectiong to a certain address.");
		}

		void Socket::Listen(int backlog)
		{
			if (IsErrorOccured(listen(_HSocket, backlog)))
				throw std::runtime_error("Failed in listening.");
		}

		void Socket::Send(const Buffer& buffer)
		{
			if (send(_HSocket, buffer.Field(), buffer.Length(), 0) < 0)
				throw std::runtime_error("Failed in sending data.");
		}
		void Socket::Send(const Buffer& buffer, int flags)
		{
			if (send(_HSocket, buffer.Field(), buffer.Length(), flags) < 0)
				throw std::runtime_error("Failed in sending data.");
		}

		void Socket::SetOption(int flags, uint32_t value)
		{
			uint32_t temp = value;
			setsockopt(_HSocket, SOL_SOCKET, flags, (const char *)&temp, sizeof(uint32_t));
		}
		void Socket::SetOption(int flags, Buffer value)
		{
			setsockopt(_HSocket, SOL_SOCKET, flags, value.Field(), value.Length());
		}

		void Socket::Receive(Buffer& buffer)
		{

			if (recv(_HSocket, buffer.Field(), buffer.Length(), 0) < 0)
				throw std::runtime_error("Failed in receiving data.");
		}
		void Socket::Receive(Buffer& buffer, int flags)
		{
			if (recv(_HSocket, buffer.Field(), buffer.Length(), flags) < 0)
				throw std::runtime_error("Failed in receiving data.");
		}

		void Socket::SendTo(Buffer& buffer, const SocketAddress& addr)
		{
			if (sendto(_HSocket, buffer.Field(), buffer.Length(), 0, (const sockaddr*)addr.Field(), addr.Length()) < 0)
				throw std::runtime_error("Failed in sending data to a certain address");
		}
		void Socket::SendTo(Buffer& buffer, const SocketAddress& addr, int flags)
		{
			if (sendto(_HSocket, buffer.Field(), buffer.Length(), flags, (const sockaddr*)addr.Field(), addr.Length()) < 0)
				throw std::runtime_error("Failed in sending data to a certain address");
		}

		void Socket::ReceiveFrom(Buffer& buffer, SocketAddress& addr)
		{
			int len = addr.Length();
			if (recvfrom(_HSocket, buffer.Field(), buffer.Length(), 0, (sockaddr*)addr.Field(), &len) < 0)
				throw std::runtime_error("Failed in receiving data from a certain address.");
		}
		void Socket::ReceiveFrom(Buffer& buffer, SocketAddress& addr, int flags)
		{
			int len = addr.Length();
			if (recvfrom(_HSocket, buffer.Field(), buffer.Length(), flags, (sockaddr*)addr.Field(), &len) < 0)
				throw std::runtime_error("Failed in receiving data from a certain address.");
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
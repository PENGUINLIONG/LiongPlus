// File: Socket.cpp
// Author: Rendong Liang (Liong)
#include "Socket.hpp"

_L_NS_BEG_NET

using std::swap;

StartUpNetModule::StartUpNetModule()
{
#ifdef _WIN32
	if (WSAStartup(MAKEWORD(2, 2), &Data) != 0)
		throw std::runtime_error("Failed in starting up WinSock.");
#endif
}
StartUpNetModule::~StartUpNetModule()
{
#ifdef _WIN32
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
#ifdef _WIN32
	if (_HSocket != INVALID_SOCKET)
#elif __linux__
	if (_HSocket >= 0)
#else
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
#ifdef _WIN32
	if (_HSocket != INVALID_SOCKET )
	{
		if (IsErrorOccured(closesocket(_HSocket)))
#elif defined __linux__
	if (_HSocket >= 0 )
	{
		if (IsErrorOccured(close(_HSocket)))
#else
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

SocketResult Socket::Send(const Buffer& buffer, size_t offset)
{
	int amount = send(_HSocket, (char*)buffer.Field() + offset, buffer.Length() - offset, 0);
	SocketResult rv;
	rv.IsErrorOccurred = amount < 0;
	rv.Amount = rv.IsErrorOccurred ? 0 : amount;
	return rv;
}
SocketResult Socket::Send(const Buffer& buffer, size_t offset, int flags)
{
	int amount = send(_HSocket, (char*)buffer.Field() + offset, buffer.Length() - offset, flags);
	SocketResult rv;
	rv.IsErrorOccurred = amount < 0;
	rv.Amount = rv.IsErrorOccurred ? 0 : amount;
	return rv;
}

bool Socket::SetSendTimeOut(long ms)
{
#ifdef _WIN32
	return SetOption(SO_RCVTIMEO, ms);
#elif defined __linux__
	Buffer buffer(sizeof(timeval));
	*(reinterpret_cast<timeval*>(buffer.Field())) = { 0, ms };
	return SetOption(SO_RCVTIMEO, buffer);
#else
#endif
}

bool Socket::SetOption(int flags, const long value)
{
	return setsockopt(_HSocket, SOL_SOCKET, flags, (const char *)&value, sizeof(uint32_t)) >= 0;
}
bool Socket::SetOption(int flags, const Buffer& value)
{
	return setsockopt(_HSocket, SOL_SOCKET, flags, (char*)value.Field(), value.Length()) >= 0;
}

bool Socket::SetReceiveTimeOut(long ms)
{
#ifdef _WIN32
	return SetOption(SO_SNDTIMEO, ms);
#elif defined __linux__
	Buffer buffer(sizeof(timeval));
	*(reinterpret_cast<timeval*>(buffer.Field())) = { 0, ms };
	return SetOption(SO_SNDTIMEO, buffer);
#else
#endif
}

SocketResult Socket::Receive(Buffer& buffer, size_t offset)
{
	int amount = recv(_HSocket, (char*)buffer.Field() + offset, buffer.Length() - offset, 0);
	SocketResult rv;
	rv.IsErrorOccurred = amount < 0;
	rv.Amount = rv.IsErrorOccurred ? 0 : amount;
	return rv;
}
SocketResult Socket::Receive(Buffer& buffer, size_t offset, int flags)
{
	int amount = recv(_HSocket, (char*)buffer.Field() + offset, buffer.Length() - offset, flags);
	SocketResult rv;
	rv.IsErrorOccurred = amount < 0;
	rv.Amount = rv.IsErrorOccurred ? 0 : amount;
	return rv;
}

SocketResult Socket::SendTo(const Buffer& buffer, size_t offset, const SocketAddress& addr)
{
	int amount = sendto(_HSocket, (char*)buffer.Field() + offset, buffer.Length() - offset, 0, (const sockaddr*)addr.Field(), addr.Length());
	SocketResult rv;
	rv.IsErrorOccurred = amount < 0;
	rv.Amount = rv.IsErrorOccurred ? 0 : amount;
	return rv;
}
SocketResult Socket::SendTo(const Buffer& buffer, size_t offset, const SocketAddress& addr, int flags)
{
	int amount = sendto(_HSocket, (char*)buffer.Field() + offset, buffer.Length() - offset, flags, (const sockaddr*)addr.Field(), addr.Length());
	SocketResult rv;
	rv.IsErrorOccurred = amount < 0;
	rv.Amount = rv.IsErrorOccurred ? 0 : amount;
	return rv;
}

SocketResult Socket::ReceiveFrom(Buffer& buffer, size_t offset, SocketAddress& addr)
{
	int len = addr.Length();

	int amount = recvfrom(_HSocket, (char*)buffer.Field() + offset, buffer.Length() - offset, 0, (sockaddr*)addr.Field(), &len);
	SocketResult rv;
	rv.IsErrorOccurred = amount < 0;
	rv.Amount = rv.IsErrorOccurred ? 0 : amount;
	return rv;
}
SocketResult Socket::ReceiveFrom(Buffer& buffer, size_t offset, SocketAddress& addr, int flags)
{
	int len = addr.Length();
	int amount = recvfrom(_HSocket, (char*)buffer.Field() + offset, buffer.Length() - offset, flags, (sockaddr*)addr.Field(), &len);
	SocketResult rv;
	rv.IsErrorOccurred = amount < 0;
	rv.Amount = rv.IsErrorOccurred ? 0 : amount;
	return rv;
}

bool Socket::IsValid() const
{
#ifdef _WIN32
	return _HSocket != INVALID_SOCKET;
#elif defined __linux__
	return _HSocket >= 0;
#else
#endif
}

// Private

bool Socket::IsErrorOccured(int code)
{
#ifdef _WIN32
	return code != 0;
#elif defined __linux__
	return code < 0;
#else
#endif
}

_L_NS_END_NET

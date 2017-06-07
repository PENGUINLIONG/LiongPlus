// File: Socket.hpp
// Author: Rendong Liang (Liong)

#pragma once
#include <cstdint>
#include "_"
#include "../Buffer.hpp"
#include "SocketAddress.hpp"

_L_NS_BEG_NET
class StartUpNetModule
{
private:
#ifdef _WIN32
	WSADATA Data;
#else
	bool _Trash;
#endif
public:
	StartUpNetModule();
	StartUpNetModule(const StartUpNetModule&) = delete;
	StartUpNetModule(StartUpNetModule&&) = delete;
	~StartUpNetModule();
};

struct SocketResult
{
	bool IsErrorOccurred = false;
	size_t Amount = 0;
};

class Socket
{
private:
#ifdef _WIN32
	typedef SOCKET HSocket;
#elif defined __linux__
	typedef int HSocket;
#else
#endif
	HSocket _HSocket;

	Socket(HSocket hSocket);

	bool IsErrorOccured(int code);
public:
	Socket();
	Socket(int addressFamily, int type, int protocal);
	Socket(const Socket&) = delete;
	Socket(Socket&& instance);
	~Socket();

	Socket& operator=(Socket&& instance);
			
	Socket Accept(SocketAddress& addr);
	bool Bind(const SocketAddress& addr);
	bool Close();
	bool Connect(const SocketAddress& addr);
	bool Listen(int backlog);
	SocketResult Send(const Buffer& buffer, size_t offset = 0);
	SocketResult Send(const Buffer& buffer, size_t offset, int flags);
	bool SetSendTimeOut(long ms);
	bool SetOption(int flags, long value);
	bool SetOption(int flags, const Buffer& value);
	bool SetReceiveTimeOut(long ms);
	SocketResult Receive(Buffer& buffer, size_t offset = 0);
	SocketResult Receive(Buffer& buffer, size_t offset, int flags);
	SocketResult SendTo(const Buffer& buffer, size_t offset, const SocketAddress& addr);
	SocketResult SendTo(const Buffer& buffer, size_t offset, const SocketAddress& addr, int flags);
	SocketResult ReceiveFrom(Buffer& buffer, size_t offset, SocketAddress& addr);
	SocketResult ReceiveFrom(Buffer& buffer, size_t offset, SocketAddress& addr, int flags);

	bool IsValid() const;
};
_L_NS_END_NET

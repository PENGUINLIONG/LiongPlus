// File: Socket.hpp
// Author: Rendong Liang (Liong)

#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

template<int TAddressFamily, int TType, int TProtpocal>
class Socket
{
private:
	int _HSocket = -1;
public:
	Socket()
	{
	}
	~Socket()
	{
		if (_HSocket >= 0)
			shutdown(_HSocket, SHUT_RDWR);
	}
	
	void Bind()
	{
		bind(_HSocket);
	}
	void Create()
	{
		_HSocket = socket(TAddressFamily, TType, TProtpocal);
	}
	Listen()
	{
		
	}
	Send();
	Receive();
	Disconnect();
}

template<int TAddressFamily, int TProtpocal>
class Socket<TAddressFamily, SOCK_DGRAM, TProtpocal>
{
private:
	int _HSocket = -1;
public:
	SendTo();
	ReceiveFrom();
}

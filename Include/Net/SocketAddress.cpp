// File: SocketAddress.cpp
// Author: Rendong Liang (Liong)

#include "SocketAddress.hpp"

namespace LiongPlus
{
	namespace Net
	{
		SocketAddress::SocketAddress(size_t length)
			: _Addr(length)
		{
			_Addr.Wipe();
		}
		SocketAddress::SocketAddress(SocketAddress&& instance)
			: _Addr()
		{
			swap(_Addr, instance._Addr);
		}
		SocketAddress::~SocketAddress()
		{
		}

		SocketAddress& SocketAddress::operator=(SocketAddress&& instance)
		{
			swap(_Addr, instance._Addr);
			return *this;
		}
		Byte& SocketAddress::operator[](size_t index)
		{
			return _Addr[index];
		}

		size_t SocketAddress::Length() const
		{
			return _Addr.Length();
		}

		Byte* SocketAddress::Field()
		{
			return _Addr.Field();
		}
		const Byte* SocketAddress::Field() const
		{
			return _Addr.Field();
		}

		unsigned short& SocketAddress::AddressFamily()
		{
			return *((unsigned short*)_Addr.Field());
		}
		const unsigned short SocketAddress::AddressFamily() const
		{
			return *((unsigned short*)_Addr.Field());
		}
		
		SocketAddress SocketAddress::ParseIPv4(const std::string name, unsigned short port)
		{
			SocketAddress addr(sizeof(sockaddr_in));
			if (inet_pton(AF_INET, name.c_str(), addr.Field() + 4) != 1)
				throw std::runtime_error("Failed in interpreting IPv4 address.");
			*((unsigned short*)(addr.Field() + 2)) = htons(port);
			*((unsigned short*)addr.Field()) = AF_INET;
			return addr;
		}
		SocketAddress SocketAddress::ParseIPv6(const std::string name, unsigned short port)
		{
			SocketAddress addr(sizeof(sockaddr_in6));
			if (inet_pton(AF_INET6, name.c_str(), addr.Field() + 8) != 1)
				throw std::runtime_error("Failed in interpreting IPv6 address.");
			*((unsigned short*)(addr.Field() + 2)) = htons(port);
			*((unsigned short*)addr.Field()) = AF_INET6;
			return addr;
		}
	}
}
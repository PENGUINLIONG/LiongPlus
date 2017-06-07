// File: SocketAddress.cpp
// Author: Rendong Liang (Liong)

#include "SocketAddress.hpp"

_L_NS_BEG_NET

SocketAddress::SocketAddress(const size_t length)
	: _Addr(length)
{
	_Addr.Wipe();
}
SocketAddress::SocketAddress(const SocketAddress& instance)
	: _Addr(instance._Addr)
{
}
SocketAddress::SocketAddress(SocketAddress&& instance)
	: _Addr()
{
	swap(_Addr, instance._Addr);
}
SocketAddress::~SocketAddress()
{
}

SocketAddress& SocketAddress::operator=(const SocketAddress& instance)
{
	_Addr = instance._Addr;
	return *this;
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

uint16_t& SocketAddress::AddressFamily()
{
	return *((uint16_t*)_Addr.Field());
}
const uint16_t SocketAddress::AddressFamily() const
{
	return *((uint16_t*)_Addr.Field());
}

std::string SocketAddress::ToString() const
{
	return "";
}



IPv4EndPoint::IPv4EndPoint(const IPv4EndPoint& instance)
	: SocketAddress(instance)
{
}
IPv4EndPoint::IPv4EndPoint(IPv4EndPoint&& instance)
	: SocketAddress(std::forward<IPv4EndPoint>(instance))
{
}
IPv4EndPoint::IPv4EndPoint(const std::string name, uint16_t port)
	: SocketAddress(sizeof(sockaddr_in))
{
	if (inet_pton(AF_INET, name.c_str(), _Addr.Field() + 4) != 1)
		throw std::runtime_error("Failed in interpreting IPv4 address.");
	*((uint16_t*)(_Addr.Field() + 2)) = htons(port);
	*((uint16_t*)_Addr.Field()) = AF_INET;
}
IPv4EndPoint::IPv4EndPoint(const uint8_t* addr_be, uint16_t port)
	:SocketAddress(sizeof(sockaddr_in))
{
	*((uint32_t*)(_Addr.Field() + 4)) = *((uint32_t*)addr_be);
	*((uint16_t*)(_Addr.Field() + 2)) = htons(port);
	*((uint16_t*)_Addr.Field()) = AF_INET;
}
IPv4EndPoint::IPv4EndPoint(uint32_t addr_le, uint16_t port)
	: SocketAddress(sizeof(sockaddr_in))
{
	*((uint32_t*)(_Addr.Field() + 4)) = htonl(addr_le);
	*((uint16_t*)(_Addr.Field() + 2)) = htons(port);
	*((uint16_t*)_Addr.Field()) = AF_INET;
}

uint16_t& IPv4EndPoint::Port()
{
	return *((uint16_t*)(_Addr.Field() + 2));
}
const uint16_t IPv4EndPoint::Port() const
{
	return *((uint16_t*)(_Addr.Field() + 2));
}

std::string IPv4EndPoint::ToString() const
{
	auto field = (uint8_t*)_Addr.Field();
	auto rv = std::to_string(field[4]) + '.' + std::to_string(field[5]) + '.' + std::to_string(field[6]) + '.' + std::to_string(field[7]);
	if (Port() != 0)
		rv += ':' + Port();
	return rv;
}


IPv6EndPoint::IPv6EndPoint(const IPv6EndPoint& instance)
	: SocketAddress(instance)
{
}
IPv6EndPoint::IPv6EndPoint(IPv6EndPoint&& instance)
	: SocketAddress(std::forward<IPv6EndPoint>(instance))
{
}
IPv6EndPoint::IPv6EndPoint(const std::string name, uint16_t port)
	: SocketAddress(sizeof(sockaddr_in6))
{
	if (inet_pton(AF_INET6, name.c_str(), _Addr.Field() + 8) != 1)
		throw std::runtime_error("Failed in interpreting IPv6 address.");
	*((uint16_t*)(_Addr.Field() + 2)) = htons(port);
	*((uint16_t*)_Addr.Field()) = AF_INET6;
}
IPv6EndPoint::IPv6EndPoint(const uint8_t* addr_be, uint16_t port)
	: SocketAddress(sizeof(sockaddr_in6))
{
	std::copy(addr_be, addr_be + 16, _Addr.Field());
	*((uint16_t*)(_Addr.Field() + 2)) = htons(port);
	*((uint16_t*)_Addr.Field()) = AF_INET6;
}
IPv6EndPoint::IPv6EndPoint(const uint16_t* addr_le, uint16_t port)
	: SocketAddress(sizeof(sockaddr_in6))
{
	size_t pos = 8;
	while (pos-- > 0)
		((uint16_t*)_Addr.Field())[4 + pos] = htons(addr_le[pos]);
	*((uint16_t*)(_Addr.Field() + 2)) = htons(port);
	*((uint16_t*)_Addr.Field()) = AF_INET6;
}

uint16_t& IPv6EndPoint::Port()
{
	return *((uint16_t*)(_Addr.Field() + 2));
}
uint16_t IPv6EndPoint::Port() const
{
	return *((uint16_t*)(_Addr.Field() + 2));
}

uint32_t& IPv6EndPoint::FlowInfo()
{
	return *((uint32_t*)(_Addr.Field() + 4));
}
uint32_t IPv6EndPoint::FlowInfo() const
{
	return *((uint32_t*)(_Addr.Field() + 4));
}

uint32_t& IPv6EndPoint::ScopeId()
{
	return *((uint32_t*)(_Addr.Field() + 24));
}
uint32_t IPv6EndPoint::ScopeId() const
{
	return *((uint32_t*)(_Addr.Field() + 24));
}

_L_NS_END_NET

// File: SocketAddress.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include <cstdint>
#include <string>
#include "_"
#include "../Buffer.hpp"

_L_NS_BEG_NET

class SocketAddress
{
protected:
	Buffer _Addr;
public:
	SocketAddress() = default;
	SocketAddress(const size_t length);
	SocketAddress(const SocketAddress&);
	SocketAddress(SocketAddress&&);
	~SocketAddress();

	SocketAddress& operator=(const SocketAddress&);
	SocketAddress& operator=(SocketAddress&&);
	Byte& operator[](size_t index);
			
	size_t Length() const;
	Byte* Field();
	const Byte* Field() const;
	uint16_t& AddressFamily();
	const uint16_t AddressFamily() const;
			
	virtual std::string ToString() const;
};

class IPv4EndPoint
	: public SocketAddress
{
public:
	IPv4EndPoint() = delete;
	IPv4EndPoint(const IPv4EndPoint&);
	IPv4EndPoint(IPv4EndPoint&&);
	IPv4EndPoint(const std::string name, const uint16_t port);
	IPv4EndPoint(const uint8_t* addr_be, const uint16_t port);
	/* Little endian! */
	IPv4EndPoint(const uint32_t addr_le, const uint16_t port);
			
	uint16_t& Port();
	const uint16_t Port() const;

	std::string ToString() const override;
};

class IPv6EndPoint
	: public SocketAddress
{
public:
	IPv6EndPoint() = delete;
	IPv6EndPoint(const IPv6EndPoint&);
	IPv6EndPoint(IPv6EndPoint&&);
	IPv6EndPoint(const std::string name, uint16_t port);
	IPv6EndPoint(const uint8_t* addr_be, uint16_t port);
	IPv6EndPoint(const uint16_t* addr_le, uint16_t port);
			
	uint16_t& Port();
	uint16_t Port() const;
	uint32_t& FlowInfo();
	uint32_t FlowInfo() const;
	uint32_t& ScopeId();
	uint32_t ScopeId() const;
};

_L_NS_END_NET

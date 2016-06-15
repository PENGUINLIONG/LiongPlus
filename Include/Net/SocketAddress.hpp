// File: SocketAddress.hpp
// Author: Rendong Liang (Liong)

#pragma once
#include "../Fundamental.hpp"
#include "../Buffer.hpp"

namespace LiongPlus
{
	namespace Net
	{
		class SocketAddress
		{
		protected:
			Buffer _Addr;
		public:
			SocketAddress() = delete;
			SocketAddress(size_t length);
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
			uint16_t AddressFamily() const;
		};

		class IPv4EndPoint
			: public SocketAddress
		{
		public:
			IPv4EndPoint() = delete;
			IPv4EndPoint(const IPv4EndPoint&);
			IPv4EndPoint(IPv4EndPoint&&);
			IPv4EndPoint(const std::string name, uint16_t port);
			IPv4EndPoint(const uint8_t* addr_be, uint16_t port);
			/* Little endian! */
			IPv4EndPoint(uint32_t addr_le, uint16_t port);
			
			uint16_t& Port();
			uint16_t Port() const;

			std::string ToString()
			{
				std::stringstream ss;
				auto field = (uint8_t*)_Addr.Field();
				ss << std::to_string(field[4]) << '.' << std::to_string(field[5]) << '.' << std::to_string(field[6]) << '.' << std::to_string(field[7]);
				if (Port() != 0)
					ss << ':' << Port();
				return ss.str();
			}
		};

		class IPv6EndPoint
			: public SocketAddress
		{
		public:
			IPv6EndPoint() = delete;
			IPv6EndPoint(const IPv6EndPoint&);
			IPv6EndPoint(IPv6EndPoint&&);
			IPv6EndPoint(const std::string name, unsigned short port);
			IPv6EndPoint(const uint8_t* addr_be, uint16_t port);
			IPv6EndPoint(const uint16_t* addr_le, uint16_t port);
			
			uint16_t& Port();
			uint16_t Port() const;
			uint32_t& FlowInfo();
			uint32_t FlowInfo() const;
			uint32_t& ScopeId();
			uint32_t ScopeId() const;
		};
	}
}
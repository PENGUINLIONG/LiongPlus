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
		private:
			Buffer _Addr;
		public:
			SocketAddress() = delete;
			SocketAddress(size_t length);
			SocketAddress(const SocketAddress&) = delete;
			SocketAddress(SocketAddress&&);
			~SocketAddress();

			SocketAddress& operator=(SocketAddress&&);
			Byte& operator[](size_t index);
			
			size_t Length() const;
			Byte* Field();
			const Byte* Field() const;
			unsigned short& AddressFamily();
			const unsigned short AddressFamily() const;

			static SocketAddress ParseIPv4(const std::string name, unsigned short port);
			static SocketAddress ParseIPv6(const std::string name, unsigned short port);
		};
	}
}
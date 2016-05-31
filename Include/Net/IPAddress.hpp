// File: IPAddress.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <string>

namespace LiongPlus
{
	namespace Net
	{
		class IPAddress
		{
		private:
			char* _Addr = nullptr;
			size_t _Size = 0;
			short _Port = 0;
			
			IPAddress();
		public:
			typename<size_t TSize>
			IPAddress(char addr[TSize], short port)
				: _Addr(new char[TSize])
				, _Size(TSize)
				, _Port(port)
			{
				std::memcpy(_Addr, addr, TSize);
			}
			IPAddress(char* addr, size_t length, short port);
			~IPAddress();
			
			// Static
			
			static IPAddress Parse;
		}
	}
}
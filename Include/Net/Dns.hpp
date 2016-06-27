// File: Dns.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "../Fundamental.hpp"
#include "SocketAddress.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;
		class Dns
		{
		public:
			static bool GetHostIPv4(const string name, vector<IPv4EndPoint>& result)
			{
#ifdef _L_WINDOWS
				DNS_RECORD* recordList;
				if (DnsQuery_UTF8(name.c_str(), DNS_TYPE_A, DNS_QUERY_STANDARD, NULL, &recordList, NULL) != 0)
					return false;				
				auto record = recordList;
				while (record != nullptr)
				{
					result.push_back(IPv4EndPoint(reinterpret_cast<uint8_t*>(&(record->Data.A)), 0));
					record = record->pNext;
				}
				DnsRecordListFree(recordList, DnsRecordList);
#else
				addrinfo hints = { 0 };
				hints.ai_family = AF_INET;
				addrinfo* addrList = nullptr;

				if (getaddrinfo(name.c_str(), nullptr, &hints, &addrList) != 0)
					return false;
				auto addr = addrList;
				while (addr != nullptr)
				{
					result.push_back(IPv4EndPoint(reinterpret_cast<uint8_t*>(addr->ai_addr->sa_data) + 2, 0));
					addr = addr->ai_next;
				}
				freeaddrinfo(addr);
#endif
				return true;
			}
		};
	}
}
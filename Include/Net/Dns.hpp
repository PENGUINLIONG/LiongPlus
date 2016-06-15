// File: Dns.hpp
// Author: Rendong Liang (Liong)
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
			static vector<IPv4EndPoint> GetHostIPv4(const string name)
			{
				vector<IPv4EndPoint> addrs;
				DNS_RECORD* recordList;
				if (DnsQuery_UTF8(name.c_str(), DNS_TYPE_A, DNS_QUERY_STANDARD, NULL, &recordList, NULL) != 0)
					throw runtime_error("Failed in getting host address in IPv4.");
				
				auto record = recordList;
				while (record != nullptr)
				{
					addrs.push_back(IPv4EndPoint(reinterpret_cast<uint8_t*>(&(record->Data.A)), 0));
					record = record->pNext;
				}
				DnsRecordListFree(record, DnsRecordList);
				return addrs;
			}
		};
	}
}
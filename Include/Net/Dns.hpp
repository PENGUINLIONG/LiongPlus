// File: Dns.hpp
// Author: Rendong Liang (Liong)
#include "../Fundamental.hpp"
#include "SocketAddress.hpp"

namespace LiongPlus
{
	namespace Net
	{
		class Dns
		{
		public:
			SocketAddress GetHostAddress(const std::string name)
			{
				DNS_RECORD* records;
				DnsQuery_UTF8(name.c_str(), DNS_TYPE_A, DNS_QUERY_STANDARD, NULL, &records, NULL);
				DnsFree(records, );
			}
		};
	}
}
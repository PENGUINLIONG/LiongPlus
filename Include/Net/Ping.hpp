// File: Ping.hpp
// Author: Rendong Liang (Liong)
#include "../Fundamental.hpp"
#include "Socket.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;
		class Ping
		{
			static atomic_bool shouldExit;
		public:
			static tuple<double, long> MakePing(SocketAddress& addr, long count, long timeout)
			{
				std::future<bool> rslt = std::async(std::launch::sync, addr, timeout);
				rslt.wait_for(std::chrono::duration<long, std::chrono::milliseconds>(timeout));
				
			}

			static bool MakePingImpl(SocketAddress& addr)
			{
				Socket socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
				Buffer buffer(8);
				buffer.Wipe();
				auto addr = IPv4EndPoint("127.0.0.1", 0);

				socket.SendTo(buffer, addr);
				socket.ReceiveFrom(buffer, addr);
				if (buffer[0] == 8)
					return true;
				else return false;
			}
		};
	}
}

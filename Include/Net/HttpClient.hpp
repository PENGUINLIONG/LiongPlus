// File: HttpClient.hpp
// Author: Rendong Liang (Liong)
#include "../Fundamental.hpp"
#include "Socket.hpp"
#include "Dns.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;
		using std::swap;

		class HttpClient
		{
			Socket _Socket;
			string _BaseAddr;
			bool _IsConnected;

			HttpClient(string host, uint16_t port = 80)
				: _Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
				, _BaseAddr(host)
			{
			}
			HttpClient(const HttpClient&) = delete;
			HttpClient(HttpClient&& instance)
				: _Socket()
				, _BaseAddr()
			{
				swap(_Socket, instance._Socket);
				swap(_BaseAddr, instance._BaseAddr);
			}

			string& BaseAddress()
			{
				return _BaseAddr;
			}
			void SendRequest()
			{

				_Socket.Send();
			}
			void SendRequestAsync()
			{
				async(launch::async, );
			}
		};
	}
}
/*// File: HttpClient.hpp
// Author: Rendong Liang (Liong)
#include "../Fundamental.hpp"
#include "Socket.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;
		using std::swap;

		class HttpClient
		{
			Socket _Socket;
			SocketAddress _Addr;
			bool _IsConnected;

			HttpClient(SocketAddress& addr)
				: _Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
				, _Addr(addr)
				, _IsConnected(false)
			{
			}
			HttpClient(const HttpClient&) = delete;
			HttpClient(HttpClient&& instance)
				: _Socket()
				, _Addr()
				, _IsConnected(false)
			{
				swap(_Socket, instance._Socket);
				swap(_Addr, instance._Addr);
				swap(_IsConnected, instance._IsConnected);
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
}*/
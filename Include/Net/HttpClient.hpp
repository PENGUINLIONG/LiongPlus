// File: HttpClient.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "../Fundamental.hpp"
#include "Dns.hpp"
#include "HttpMessage.hpp"
#include "HttpBufferPool.hpp"
#include "Socket.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;
		using std::swap;

		class HttpClient
		{
			friend void swap(HttpClient& a, HttpClient& b)
			{
				swap(a._Socket, b._Socket);
				swap(a._Addr, b._Addr);
				swap(a._IsConnected, b._IsConnected);
				swap(a._HostName, b._HostName);
			}
		private:
			Socket _Socket;
			SocketAddress _Addr;
			bool _IsConnected;
			string _HostName;

			static string _UserAgentCode;
			static string _AcceptContentType;
			
			static HttpResponse DoSend(HttpClient* this_ptr, HttpRequestLine& line, HttpHeader& header, Buffer& content);
			static HttpResponse DoSendRequest(HttpClient* this_ptr, HttpRequest& request);

		public:
			HttpClient();
			HttpClient(const HttpClient&) = delete;
			HttpClient(HttpClient&&);
			HttpClient(SocketAddress& addr);
			HttpClient(const string hostname);

			string& HostName();

			// TODO: Get set hostname ! ///////////////////////////////

			future<HttpResponse> GetAsync(string path);
			future<HttpResponse> PostAsync(string path, Buffer& content);
			future<HttpResponse> PutAsync(string path, Buffer& content);

			future<HttpResponse> SendAsync(HttpRequest& request);

			bool IsConnected() const;
		};
	}
}
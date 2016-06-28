// File: HttpClient.cpp
// Author: Rendong Liang (Liong)
#include "HttpClient.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;

		string HttpClient::_UserAgentCode = "LiongPlus.Net/0.1";
		string HttpClient::_AcceptContentType = "*/*";

		HttpClient::HttpClient()
			: _Socket()
			, _Addr()
			, _IsConnected(false)
			, _HostName()
		{
		}
		HttpClient::HttpClient(HttpClient&& instance)
			: _Socket()
			, _Addr()
			, _IsConnected(false)
			, _HostName()
		{
			swap(_Socket, instance._Socket);
			swap(_Addr, instance._Addr);
			swap(_IsConnected, instance._IsConnected);
			swap(_HostName, instance._HostName);
		}
		HttpClient::HttpClient(SocketAddress& addr)
			: _Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
			, _Addr(addr)
			, _IsConnected(false)
			, _HostName(addr.ToString())
		{
		}
		HttpClient::HttpClient(const string hostname)
			: _Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
			, _Addr()
			, _IsConnected(false)
			, _HostName(hostname)
		{
			vector<IPv4EndPoint> ips;
			if (!Dns::GetHostIPv4(hostname, ips))
				throw runtime_error("Dns information not found or an internal error has occured.");
			ips[0].Port() = htons(80);
			_Addr = ips[0];
		}

		string& HttpClient::HostName()
		{
			return _HostName;
		}

		future<HttpResponse> HttpClient::GetAsync(string path)
		{
			HttpRequestLine line(1, 1, HttpMethod::Get, path);
			HttpHeader header
			{
				HttpHeader::Request::Host, _HostName,
				HttpHeader::Request::Accept, _AcceptContentType,
				HttpHeader::Request::UserAgent, _UserAgentCode
			};
			Buffer empty;
			
			auto rv = async(launch::async, HttpClient::DoSend, this, line, header, empty);
			return rv;
		}
		future<HttpResponse> HttpClient::PostAsync(string path, Buffer& content)
		{
			HttpRequestLine line(1, 1, HttpMethod::Post, path);
			HttpHeader header
			{
				HttpHeader::Request::Host, _HostName,
				HttpHeader::Request::Accept, _AcceptContentType,
				HttpHeader::Request::UserAgent, _UserAgentCode
			};

			auto rv = async(launch::async, HttpClient::DoSend, this, line, header, content);
			return rv;
		}
		future<HttpResponse> HttpClient::PutAsync(string path, Buffer& content)
		{
			HttpRequestLine line(1, 1, HttpMethod::Put, path);
			HttpHeader header
			{
				HttpHeader::Request::Host, _HostName,
				HttpHeader::Request::Accept, _AcceptContentType,
				HttpHeader::Request::UserAgent, _UserAgentCode
			};

			auto rv = async(launch::async, HttpClient::DoSend, this, line, header, content);
			return rv;
		}
		future<HttpResponse> HttpClient::SendAsync(HttpRequest& request)
		{
			auto rv = async(launch::async, HttpClient::DoSendRequest, this, request);
			return rv;
		}

		bool HttpClient::IsConnected() const
		{
			return _IsConnected;
		}

		// Private
		
		// Static

		HttpResponse HttpClient::DoSend(HttpClient* this_ptr, HttpRequestLine& line, HttpHeader& header, Buffer& content)
		{
			return forward<HttpResponse>(DoSendRequest(this_ptr, HttpRequest{ line, header, content }));
		}
		HttpResponse HttpClient::DoSendRequest(HttpClient* this_ptr, HttpRequest& request)
		{
			bool triedConnecting = false;

			Socket& socket = this_ptr->_Socket;

			// If not connected,
			if (!this_ptr->_IsConnected)
			{
				// Try connecting.
			tryConnecting:
				if (socket.Connect(this_ptr->_Addr))
					this_ptr->_IsConnected = true;
				else
					return{}; // Failed, return an empty response.
				triedConnecting = true;
			}
			
			auto sendInfo = socket.Send(request.ToBuffer());
			
			// If failed in sending request,
			if (sendInfo.IsErrorOccurred)
			{
				// Consider if the connection is out dated.
				// But if we have tried connecting previously, re-connection will obviously fail.
				if (triedConnecting)
					return{};
				else
					goto tryConnecting;
			}

			HttpBufferToken token = HttpBufferPool::Apply();
			Buffer& buffer = HttpBufferPool::Fetch(token);

			auto recvInfo = socket.Receive(buffer);
			if (recvInfo.Amount == 0)
				return{};

			HttpResponse res;
			size_t headerLength = res.FromBuffer(buffer, 0);
			if (res.Header.Contains(HttpHeader::Entity::ContentLength))
			{
				size_t contentLength = stoull(res.Header[HttpHeader::Entity::ContentLength]);

				size_t contentHaveRead = recvInfo.Amount - headerLength;

				while (contentLength > contentHaveRead)
				{
					auto complementInfo = socket.Receive(res.Content, contentHaveRead);
					if (complementInfo.Amount == 0)
						return{};
					else
						contentHaveRead += complementInfo.Amount;
				}
			}
			HttpBufferPool::Release(token);

			return res;
		}

	}
}

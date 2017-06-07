// File: HttpClient.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include <future>
#include <mutex>
#include "_"
#include "../Dns.hpp"
#include "HttpMessage.hpp"
#include "../Socket.hpp"

_L_NS_BEG_NET_HTTP
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
	static const long _MinBufferSize = 4 * 1024 * 1024;
	static const std::string _UserAgentCode;
	static const std::string _AcceptContentType;

	Socket _Socket;
	SocketAddress _Addr;
	bool _IsConnected;
	std::string _HostName;
	std::mutex _HostNameLock;

	HttpResponse ReceiveResponse();

	static HttpResponse DoSend(HttpClient& this_ptr, HttpRequestLine&& line, HttpHeader&& header, Buffer&& content);
	static HttpResponse DoSendRequest(HttpClient& this_ptr, HttpRequest&& request);

public:
	HttpClient();
	HttpClient(const HttpClient&) = delete;
	HttpClient(HttpClient&&);
	HttpClient(const SocketAddress& addr);
	HttpClient(const std::string& hostname, bool shouldWait = true);

	std::string GetHostName() const;
	std::future<bool> SetHostName(const std::string&);

	HttpResponse Get(const std::string& path);
	std::future<HttpResponse> GetAsync(const std::string& path);
	HttpResponse Post(const std::string& path, Buffer&& content);
	std::future<HttpResponse> PostAsync(const std::string& path, Buffer&& content);
	HttpResponse Put(const std::string& path, Buffer&& content);
	std::future<HttpResponse> PutAsync(const std::string& path, Buffer&& content);

	HttpResponse Send(HttpRequest&& request);
	future<HttpResponse> SendAsync(HttpRequest&& request);

	bool IsConnected() const;
	/*
	* Make sure the connectivity between local machine and remote host.
	* Returns:
	*   True if it has successfully connected to the remote host. False, otherwise.
	*/
	bool EnsureConnectivity();
};

_L_NS_END_NET_HTTP

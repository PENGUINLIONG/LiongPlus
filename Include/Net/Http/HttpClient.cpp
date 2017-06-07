// File: HttpClient.cpp
// Author: Rendong Liang (Liong)
#include "HttpClient.hpp"
#include "../../BufferPool.hpp"
#include <future>

_L_NS_BEG_NET_HTTP

using namespace std;

const long _MinBufferSize = 4 * 1024 * 1024;
const string HttpClient::_UserAgentCode = "LiongPlus.Net/0.1";
const string HttpClient::_AcceptContentType = "*/*";

HttpClient::HttpClient()
	: _Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
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
HttpClient::HttpClient(const SocketAddress& addr)
	: _Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
	, _Addr(addr)
	, _IsConnected(false)
	, _HostName(addr.ToString())
{
}
HttpClient::HttpClient(const string& hostname, bool shouldWait)
	: _Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
	, _Addr()
	, _IsConnected(false)
	, _HostName()
{
	if (shouldWait)
		SetHostName(hostname).wait();
	else
		SetHostName(hostname);
}

string HttpClient::GetHostName() const
{
	return _HostName;
}
future<bool> HttpClient::SetHostName(const string& hostname)
{
	auto handle = async(launch::async, [this](const string& hostn) -> bool
	{
		lock_guard<mutex> lock(_HostNameLock);

		vector<IPv4EndPoint> ips;
		if (!Dns::GetHostIPv4(hostn, ips))
			return false;
		ips[0].Port() = htons(80);
		_Addr = ips[0];
		_HostName = move(hostn);
		_IsConnected = false;
		_Socket = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		return true;
	}, hostname);
	return handle;
}

HttpResponse HttpClient::Get(const string& path)
{
	HttpRequestLine line(1, 1, HttpMethods::Get, path);
	HttpHeader header
	{
		RequestHeaders::Host, _HostName,
		RequestHeaders::Accept, _AcceptContentType,
		RequestHeaders::UserAgent, _UserAgentCode
	};
	Buffer empty;

	return HttpClient::DoSend(*this, move(line), move(header), forward<Buffer>(empty));
}
future<HttpResponse> HttpClient::GetAsync(const string& path)
{
	return async(launch::async, [this](const string& p) { return Get(p); }, path);
}
HttpResponse HttpClient::Post(const string& path, Buffer&& content)
{
	HttpRequestLine line(1, 1, HttpMethods::Post, path);
	HttpHeader header
	{
		RequestHeaders::Host, _HostName,
		RequestHeaders::Accept, _AcceptContentType,
		RequestHeaders::UserAgent, _UserAgentCode
	};

	return HttpClient::DoSend(*this, move(line), move(header), forward<Buffer>(content));
}
future<HttpResponse> HttpClient::PostAsync(const string& path, Buffer&& content)
{
	return async(launch::async, [this, &path, &content]() { return Post(path, forward<Buffer>(content)); });
}
HttpResponse HttpClient::Put(const string& path, Buffer&& content)
{
	HttpRequestLine line(1, 1, HttpMethods::Put, path);
	HttpHeader header
	{
		RequestHeaders::Host, _HostName,
		RequestHeaders::Accept, _AcceptContentType,
		RequestHeaders::UserAgent, _UserAgentCode
	};

	return HttpClient::DoSend(*this, move(line), move(header), forward<Buffer>(content));
}
future<HttpResponse> HttpClient::PutAsync(const string& path, Buffer&& content)
{
	return async(launch::async, [this, &path, &content]() { return Put(path, forward<Buffer>(content)); });
}

HttpResponse HttpClient::Send(HttpRequest&& request)
{
	return HttpClient::DoSendRequest(*this, forward<HttpRequest>(request));
}
future<HttpResponse> HttpClient::SendAsync(HttpRequest&& request)
{
	return async(launch::async, [this](HttpRequest req) { return Send(forward<HttpRequest>(req)); }, request);
}

bool HttpClient::IsConnected() const
{
	return _IsConnected;
}

// Private

// Static

HttpResponse HttpClient::DoSend(HttpClient& this_ptr, HttpRequestLine&& line, HttpHeader&& header, Buffer&& content)
{
	return forward<HttpResponse>(DoSendRequest(this_ptr, HttpRequest{ forward<HttpRequestLine>(line), forward<HttpHeader>(header), forward<Buffer>(content) }));
}


bool HttpClient::EnsureConnectivity()
{
	lock_guard<mutex> lock(_HostNameLock);

	// If not connected,
	if (_IsConnected)
		return true;
	else
	{
		// Try connecting to the remote host.
		if (_Socket.Connect(_Addr)) return _IsConnected = true;
		else return false;
	}
}

HttpResponse HttpClient::ReceiveResponse()
{
	// Retrieve buffer to carry response content.
	auto buffer = *BufferPool::Ginst().Get(_MinBufferSize);
	auto recvInfo = _Socket.Receive(buffer);
	if (recvInfo.Amount == 0) return{};

	HttpResponse res;
	size_t headerLength = res.FromBuffer(buffer, 0);
	// Check header fields.
	if (res.Header.Contains(EntityHeaders::ContentLength))
	{
		size_t contentLength = stoull(res.Header[EntityHeaders::ContentLength]);
		size_t contentHaveRead = recvInfo.Amount - headerLength;

		while (contentLength > contentHaveRead)
		{
			auto complementInfo = _Socket.Receive(res.Content, contentHaveRead);
			if (complementInfo.Amount == 0) return{};
			else contentHaveRead += complementInfo.Amount;
		}
	}
	if (res.Header.Contains(GeneralHeaders::Connection))
	{
		if (res.Header[GeneralHeaders::Connection] == "close")
			_IsConnected = false;
	}
}

HttpResponse HttpClient::DoSendRequest(HttpClient& this_ptr, HttpRequest&& request)
{
	bool haveTriedResend = false;
tryResend:
	// Make sure the connectivity, but it is possible that the connection has been closed implicitly.
	if (!this_ptr.EnsureConnectivity()) return{};

	auto sendInfo = this_ptr._Socket.Send(request.ToBuffer());
	// If failed in sending request,
	if (sendInfo.IsErrorOccurred)
	{
		// If have tried resend message, the connection can hardly be established. Give up and return a empty response.
		if (haveTriedResend)
			return{};
		else
		{
			haveTriedResend = true;
			goto tryResend;
		}
	}
	
	return forward<HttpResponse>(this_ptr.ReceiveResponse());
}

_L_NS_END_NET_HTTP


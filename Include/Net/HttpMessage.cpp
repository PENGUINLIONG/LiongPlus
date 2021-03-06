// File: HttpMessage.cpp
// Author: Rendong Liang (Liong)
#include "HttpMessage.hpp"
#include "../IO/MemoryStream.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;
		using std::swap;


		//
		// HttpMethod
		//

		const char
			*HttpMethod::Get = "GET",
			*HttpMethod::Head = "HEAD",
			*HttpMethod::Post = "POST",
			*HttpMethod::Put = "PUT",
			*HttpMethod::Delete = "DELETE",
			*HttpMethod::Trace = "TRACE",
			*HttpMethod::Options = "OPTIONS",
			*HttpMethod::Connect = "CONNECT",
			*HttpMethod::Patch = "PATCH";

		//
		// HttpResponseLine
		//

		HttpRequestLine::HttpRequestLine(HttpRequestLine&& instance)
			: HttpRequestLine()
		{
			swap(MajorVersion, instance.MajorVersion);
			swap(MinorVersion, instance.MinorVersion);
			swap(Method, instance.Method);
			swap(Path, instance.Path);
		}
		HttpRequestLine::HttpRequestLine(long major, long minor, string method, string path)
			: MajorVersion(major)
			, MinorVersion(minor)
			, Method(method)
			, Path(path)
		{
		}

		HttpRequestLine& HttpRequestLine::operator=(const HttpRequestLine& instance)
		{
			MajorVersion = instance.MajorVersion;
			MinorVersion = instance.MinorVersion;
			Method = instance.Method;
			Path = instance.Path;
			return *this;
		}
		HttpRequestLine& HttpRequestLine::operator=(HttpRequestLine&& instance)
		{
			swap(MajorVersion, instance.MajorVersion);
			swap(MinorVersion, instance.MinorVersion);
			swap(Method, instance.Method);
			swap(Path, instance.Path);
			return *this;
		}

		string HttpRequestLine::ToString() const
		{
			stringstream ss;
			ss << Method << ' ' << Path << ' ' << "HTTP/" << MajorVersion << '.' << MinorVersion << '\n';
			return ss.str();
		}
		
		//
		// HttpResponseLine
		//

		HttpStatusLine::HttpStatusLine(HttpStatusLine&& instance)
			: HttpStatusLine()
		{
			swap(MajorVersion, instance.MajorVersion);
			swap(MinorVersion, instance.MinorVersion);
			swap(Status, instance.Status);
		}
		HttpStatusLine::HttpStatusLine(long major, long minor, long statusCode, string status)
			: MajorVersion(major)
			, MinorVersion(minor)
			, StatusCode(statusCode)
			, Status(status)
		{
		}

		HttpStatusLine& HttpStatusLine::operator=(const HttpStatusLine& instance)
		{
			MajorVersion = instance.MajorVersion;
			MinorVersion = instance.MinorVersion;
			Status = instance.Status;
			return *this;
		}
		HttpStatusLine& HttpStatusLine::operator=(HttpStatusLine&& instance)
		{
			swap(MajorVersion, instance.MajorVersion);
			swap(MinorVersion, instance.MinorVersion);
			swap(Status, instance.Status);
			return *this;
		}

		string HttpStatusLine::ToString() const
		{
			stringstream ss;
			ss << "HTTP/" << MajorVersion << '.' << MinorVersion << ' ' << Status << '\n';
			return ss.str();
		}

		//
		// HttpHeader
		//

		// Private
		
		void HttpHeader::AddElements(string& key, string& value)
		{
			_Headers[key] = value;
		}

		// Public

		const char
			*HttpHeader::General::CacheControl = "Cache-Control",
			*HttpHeader::General::Connection = "Connection",
			*HttpHeader::General::Date = "Date",
			*HttpHeader::General::Pragma = "Pragma",
			*HttpHeader::General::Trailer = "Trailer",
			*HttpHeader::General::TransferEncoding = "Transfer-Encoding",
			*HttpHeader::General::Upgrade = "Upgrade",
			*HttpHeader::General::Via = "Via",
			*HttpHeader::General::Warning = "Warning";
		const char
			*HttpHeader::Request::Accept = "Accept",
			*HttpHeader::Request::AcceptCharset = "Accept-Charset",
			*HttpHeader::Request::AcceptEncoding = "Accept-Encoding",
			*HttpHeader::Request::AcceptLanguage = "Accept-Language",
			*HttpHeader::Request::Authorization = "Authorization",
			*HttpHeader::Request::Except = "Except",
			*HttpHeader::Request::From = "From",
			*HttpHeader::Request::Host = "Host",
			*HttpHeader::Request::IfMatch = "If-Match",
			*HttpHeader::Request::IfModifiedSince = "If-Modified-Since",
			*HttpHeader::Request::IfNoneMatch = "If-None-Match",
			*HttpHeader::Request::IfUnmodifiedSince = "If-Unmodified-Since",
			*HttpHeader::Request::MaxForwards = "Max-Forwards",
			*HttpHeader::Request::ProxyAuthorization = "Proxy-Authorization",
			*HttpHeader::Request::Range = "Range",
			*HttpHeader::Request::Referer = "Referer",
			*HttpHeader::Request::TE = "TE",
			*HttpHeader::Request::UserAgent = "User-Agent";
		const char
			*HttpHeader::Response::AcceptRange = "Accept-Range",
			*HttpHeader::Response::Age = "Age",
			*HttpHeader::Response::ETag = "ETag",
			*HttpHeader::Response::Location = "Location",
			*HttpHeader::Response::ProxyAuthenticate = "Proxy-Authenticate",
			*HttpHeader::Response::RetryAfter = "Retry-After",
			*HttpHeader::Response::Server = "Server",
			*HttpHeader::Response::Vary = "Vary",
			*HttpHeader::Response::WwwAuthenticate = "WWW-Authenticate";
		const char
			*HttpHeader::Entity::Allow = "Allow",
			*HttpHeader::Entity::ContentEncoding = "Content-Encoding",
			*HttpHeader::Entity::ContentLanguage = "Content-Language",
			*HttpHeader::Entity::ContentLength = "Content-Length",
			*HttpHeader::Entity::ContentLocation = "Content-Location",
			*HttpHeader::Entity::ContentMd5 = "Content-MD5",
			*HttpHeader::Entity::ContentRange = "Content-Range",
			*HttpHeader::Entity::ContentType = "Content-Type",
			*HttpHeader::Entity::Expires = "Expires",
			*HttpHeader::Entity::LastModified = "Last-Modified";

		HttpHeader::HttpHeader(HttpHeader&& instance)
		{
			swap(*this, instance);
		}

		HttpHeader& HttpHeader::operator=(const HttpHeader& instance)
		{
			_Headers = instance._Headers;
			return *this;
		}
		HttpHeader& HttpHeader::operator=(HttpHeader&& instance)
		{
			swap(*this, instance);
			return *this;
		}
		string& HttpHeader::operator[](string& key)
		{
			return _Headers[key];
		}

		bool HttpHeader::Contains(string& key) const
		{
			return (_Headers.find(key) != _Headers.end());
		}
		// This will return the boolean value which indicates whether the key/value pair is NOT existing.
		void HttpHeader::Remove(string& key)
		{
			_Headers.erase(key);
		}
		string HttpHeader::ToString() const
		{
			stringstream ss;
			for (auto& header : _Headers)
				ss << header.first << ": " << header.second << '\n';
			return ss.str();
		}

		//
		// HttpRequest
		//

		HttpMessage::HttpMessage(HttpHeader header, Buffer content)
			: Header(header)
			, Content(content)
		{
		}

		Buffer HttpMessage::ToBuffer(const HttpLine& _line, const HttpHeader& _header, const Buffer& _content)
		{
			string line = _line.ToString();
			string header = _header.ToString();

			Buffer buffer(line.length() + header.length() + 1 + _content.Length()); // +1 for the extra '\n'.
			memcpy(buffer.Field(), line.c_str(), line.length());
			memcpy(buffer.Field() + line.length(), header.c_str(), header.length());
			buffer.Field()[line.length() + header.length()] = '\n';
			memcpy(buffer.Field() + line.length() + header.length() + 1, _content.Field(), _content.Length());
			return buffer;
		}

		//
		// HttpRequest
		//

		HttpRequest::HttpRequest(HttpHeader& header, HttpRequestLine& line, Buffer& content)
			: HttpMessage(header, content)
			, RequestLine(line)
		{
		}

		Buffer HttpRequest::ToBuffer() const
		{
			return forward<Buffer>(HttpMessage::ToBuffer(RequestLine, Header, Content));
		}

		//
		// HttpResponse
		//

		HttpResponse::HttpResponse(HttpHeader& header, HttpStatusLine& line, Buffer& content)
			: HttpMessage(header, content)
			, StatusLine(line)
		{
		}

		Buffer HttpResponse::ToBuffer() const
		{
			return forward<Buffer>(HttpMessage::ToBuffer(StatusLine, Header, Content));
		}
	}
}

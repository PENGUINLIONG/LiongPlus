// File: HttpMessage.cpp
// Author: Rendong Liang (Liong)
#include "HttpMessage.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;
		using std::swap;

		//
		// HttpLine
		//

		HttpLine::HttpLine(long major, long minor)
			: MajorVersion(major)
			, MinorVersion(minor)
		{
		}

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
			: HttpLine(major, minor)
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
		
		// Notice that the offset will be set to the beginning of the HTTP header part.
		bool HttpRequestLine::FromBuffer(Buffer& buffer, size_t& offset)
		{
			HttpRequestLine line;
			auto end = buffer.Field() + buffer.Length(); // Should not be read.
			auto beg = buffer.Field() + offset;
			auto pos = beg;
			auto eol = HttpUtils::SeekForEOL(beg, end);

			HttpUtils::CaptureText(pos, eol, line.Method);
			if (*(pos++) != ' ') return false;
			HttpUtils::CaptureText(pos, eol, line.Path);
			if (*(pos++) != ' ') return false;
			if (!HttpUtils::CaptureHttpVersion(pos, eol, line.MinorVersion, line.MinorVersion))
				return false;
			// Here should be the end of line.
			if (pos != eol) return false;
			// Set offset.
			offset += HttpUtils::GetOffset(beg, eol, end);

			*this = std::move(line);
			return true;
		}
		
		//
		// HttpResponseLine
		//

		HttpStatusLine::HttpStatusLine(HttpStatusLine&& instance)
			: HttpStatusLine()
		{
			swap(MajorVersion, instance.MajorVersion);
			swap(MinorVersion, instance.MinorVersion);
			swap(StatusCode, instance.StatusCode);
			swap(Reason, instance.Reason);
		}
		HttpStatusLine::HttpStatusLine(long major, long minor, long statusCode, string reason)
			: HttpLine(major, minor)
			, StatusCode(statusCode)
			, Reason(reason)
		{
		}

		HttpStatusLine& HttpStatusLine::operator=(const HttpStatusLine& instance)
		{
			MajorVersion = instance.MajorVersion;
			MinorVersion = instance.MinorVersion;
			StatusCode = instance.StatusCode;
			Reason = instance.Reason;
			return *this;
		}
		HttpStatusLine& HttpStatusLine::operator=(HttpStatusLine&& instance)
		{
			swap(MajorVersion, instance.MajorVersion);
			swap(MinorVersion, instance.MinorVersion);
			swap(StatusCode, instance.StatusCode);
			swap(Reason, instance.Reason);
			return *this;
		}

		string HttpStatusLine::ToString() const
		{
			stringstream ss;
			ss << "HTTP/" << MajorVersion << '.' << MinorVersion << ' ' << StatusCode << ' ' << Reason << '\n';
			return ss.str();
		}

		// Notice that the offset will be set to the beginning of the HTTP header part.
		bool HttpStatusLine::FromBuffer(Buffer buffer, size_t& offset)
		{
			HttpStatusLine line;
			auto end = buffer.Field() + buffer.Length(); // Should not be read.
			auto beg = buffer.Field() + offset;
			auto pos = beg;
			auto eol = HttpUtils::SeekForEOL(beg, end);

			if (!HttpUtils::CaptureHttpVersion(pos, eol, line.MajorVersion, line.MinorVersion))
				return false;
			if (*(pos++) != ' ') return false;
			HttpUtils::CaptureNumber(pos, eol, line.StatusCode);
			if (*(pos++) != ' ') return false;
			line.Reason = string(pos, eol);
			// Here should be the end of line but we will do nothing.
			// (Because $pos must equal to $eol)
			// Set offset.
			offset += HttpUtils::GetOffset(beg, eol, end);

			*this = std::move(line);
			return true;
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
			swap(_Headers, instance._Headers);
		}

		HttpHeader& HttpHeader::operator=(const HttpHeader& instance)
		{
			_Headers = instance._Headers;
			return *this;
		}
		HttpHeader& HttpHeader::operator=(HttpHeader&& instance)
		{
			swap(_Headers, instance._Headers);
			return *this;
		}
		string& HttpHeader::operator[](const char* key)
		{
			return _Headers[key];
		}

		bool HttpHeader::Contains(const char* key) const
		{
			return (_Headers.find(key) != _Headers.end());
			for (pair<string, string> pr : _Headers)
			{
#ifdef _L_WINDOWS
				if (_stricmp(pr.first.c_str(), key))
#else
				if (strcasecmp(pr.first.c_str(), key))
#endif
					return true;
			}
			return false;
		}
		bool HttpHeader::Contains(string& key) const
		{
			return Contains(key.c_str());
		}
		// This will return the boolean value which indicates whether the key/value pair is NOT existing.
		void HttpHeader::Remove(const char* key)
		{
			_Headers.erase(key);
		}
		void HttpHeader::Remove(string& key)
		{
			_Headers.erase(key);
		}
		string HttpHeader::ToString() const
		{
			stringstream ss;
			for (pair<string, string> header : _Headers)
				ss << header.first << ": " << header.second << '\n';
			return ss.str();
		}
		bool HttpHeader::FromBuffer(Buffer& buffer, size_t& offset)
		{
			auto last = _Headers.end();
			for (;;)
			{
				auto beg = buffer.Field() + offset;
				auto end = buffer.Field() + buffer.Length();
				auto divisor = beg;
				auto eol = HttpUtils::SeekForEOL(beg, end);
				
				// An empty line should exist, which is the end of the HTTP header.
				if (beg == eol) // This actually equals to the case of $divisor == $eol.
				{
					// There is no empty line, improper format.
					if (eol == end)
						return false;

					offset += HttpUtils::GetOffset(beg, eol, end);
					return true;
				}

				// If LWS (linear white space) presents, this line is the complement of the previous one.
				if (HttpUtils::IsLWS(beg))
				{
					auto nlws = HttpUtils::SeekForNonLWS(beg, eol);
					if (nlws < eol)
					{
						if (last != _Headers.end())
							(*last).second += string(nlws, eol);
						else
							return false;
					}
					continue;
				}

				// Seek for ':'.
				while (divisor < eol)
				{
					if (*(divisor++) == ':')
						break;
				}

				// Now divisor should be at SP.
				if (divisor < eol)
				{
					offset += HttpUtils::GetOffset(beg, eol, end);
					string key(beg, divisor - 1);
					auto posValue = HttpUtils::SeekForNonLWS(divisor, eol);
					string value(divisor, eol);
					if (Contains(key))
						_Headers[key] += _Headers[key] == "" ? value : ", " + value;
					else
						last = _Headers.emplace(key, value).first;
				}
				// Improper format.
				else
					return false;
			}
		}

		//
		// HttpMessage
		//
		
		HttpMessage::HttpMessage(HttpMessage&& instance)
			: HttpMessage()
		{
			swap(Header, instance.Header);
			swap(Content, instance.Content);
		}
		HttpMessage::HttpMessage(HttpHeader& header, Buffer& content)
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

		HttpRequest::HttpRequest(HttpRequest&& instance)
			: HttpRequest()
		{
			swap(RequestLine, instance.RequestLine);
			swap(Header, instance.Header);
			swap(Content, instance.Content);
		}
		HttpRequest::HttpRequest(HttpRequestLine& line, HttpHeader& header, Buffer& content)
			: HttpMessage(header, content)
			, RequestLine(line)
		{
		}

		Buffer HttpRequest::ToBuffer() const
		{
			return forward<Buffer>(HttpMessage::ToBuffer(RequestLine, Header, Content));
		}

		size_t HttpRequest::FromBuffer(Buffer& buffer, size_t offset)
		{
			auto beg = buffer.Field() + offset;
			auto end = buffer.Field() + buffer.Length();
			auto pos = beg;

			if (!RequestLine.FromBuffer(buffer, offset))
				return 0;
			if (!Header.FromBuffer(buffer, offset))
				return 0;
			if (Header.Contains(HttpHeader::Entity::ContentLength))
			{
				size_t length = std::stoull(Header[HttpHeader::Entity::ContentLength]);
				Content = Buffer(length);
				buffer.CopyTo(Content.Field(), offset, buffer.Length() - offset);
			}
			return offset;
		}

		//
		// HttpResponse
		//

		HttpResponse::HttpResponse(HttpResponse&& instance)
			: HttpResponse()
		{
			swap(StatusLine, instance.StatusLine);
			swap(Header, instance.Header);
			swap(Content, instance.Content);
		}
		HttpResponse::HttpResponse(HttpStatusLine& line, HttpHeader& header, Buffer& content)
			: HttpMessage(forward<HttpHeader>(header), forward<Buffer>(content))
			, StatusLine(line)
		{
		}

		Buffer HttpResponse::ToBuffer() const
		{
			return forward<Buffer>(HttpMessage::ToBuffer(StatusLine, Header, Content));
		}

		size_t HttpResponse::FromBuffer(Buffer& buffer, size_t offset)
		{
			auto beg = buffer.Field() + offset;
			auto end = buffer.Field() + buffer.Length();
			auto pos = beg;
			
			if (!StatusLine.FromBuffer(buffer, offset))
				return 0;
			if (!Header.FromBuffer(buffer, offset))
				return 0;
			if (Header.Contains(HttpHeader::Entity::ContentLength))
			{
				size_t length = std::stoull(Header[HttpHeader::Entity::ContentLength]);
				Content = Buffer(length);
				if (length > buffer.Length() - offset)
					buffer.CopyTo(Content.Field(), offset, buffer.Length() - offset);
				else
					buffer.CopyTo(Content.Field(), offset, length);
			}
			return offset;
		}
	}
}

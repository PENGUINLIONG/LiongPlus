// File: Uri.cpp
// Author: Rendong Liang (Liong)
#include "Uri.hpp"
#include "AnsiCharUtils.hpp"

namespace LiongPlus
{
	using namespace std;

	Uri::Uri(Uri&& instance)
	{
		swap(Scheme, instance.Scheme);
		swap(Authority, instance.Authority);
		swap(Path, instance.Path);
		swap(Query, instance.Query);
		swap(Fragment, instance.Fragment);
	}
	Uri::Uri(string scheme, string authority, string path, string query, string fragment)
		: Scheme(scheme)
		, Authority(authority)
		, Path(path)
		, Query(query)
		, Fragment(fragment)
	{}
	Uri(std::string scheme, std::string authority, std::string path, std::string fragment)
		: Scheme(scheme)
		, Authority(authority)
		, Path(path)
		, Query(string())
		, Fragment(fragment)
	{}

	string Uri::ToString()
	{
		string rv = Scheme + ':';
		if (!Authority.empty()) grv += "//" + Authority;
		rv += Path;
		if (!Query.empty()) rv += '?' + Query;
		if (!Fragment.empty()) rv += '#' + Fragment;

		return rv;
	}

	//
	// HttpUri
	//

	const std::string _StrHttp = "http";
	const std::string _StrHttps = "https";

	HttpUri::HttpUri(const HttpUri& instance) : Uri(instance) {}
	HttpUri::HttpUri(HttpUri&& instance) : Uri(std::forward<HttpUri>(instance)) {}
	HttpUri::HttpUri(std::string authority, std::string path, std::string query, std::string fragment, bool isHttps)
		: Uri(isHttps ? _StrHttps : _StrHttp, authority, path, query, fragment) {}
	HttpUri::HttpUri(std::string authority, std::string path, std::string fragment, bool isHttps)
		: Uri(isHttps ? _StrHttps : _StrHttp, authority, path, fragment) {}

	bool HttpUri::IsHttp() const
	{ return AnsiCharUtils::CaseInsensitiveEquals(Scheme, _StrHttp); }
	
	bool HttpUri::IsHttps() const
	{ return AnsiCharUtils::CaseInsensitiveEquals(Scheme, _StrHttps); }

	// Static

		auto schemeCondition = [](const char c){ return IsAlphabet(c) || IsDigit(c) || c == '+' || c == '-' || c == '.'; };
		auto genDelimCondition = [](const char c){ return c == ':' || c == '/' || c == '?' || c == '#' || c == '[' || c == ']' || c == '@';};
		auto subDelimCondition = [](const char c){ return c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' || c == '+' || c == ',' || c == ';' || c == '='};
		auto unreservedCondition = [](const char c){ return IsAlphabet(c) || IsDigit(c) || c == '-' || c == '.' || c == '_'|| c == '~'; };
		auto userInfoCondition = [](const char c){ return unreservedCondition(c) || subDelimCondition(c) || c == '%'; };
		auto pcharCondition = [](const char c){ return unreservedCondition(c) || subDelimCondition(c) || c == '%' || c == ':' || c == '@'};
	static unique_ptr<HttpUri> HttpUri::Parse(std::string uri)
	{
		using namespace AnsiCharUtils;
		// NE for Not End.
#define _L_NE ++pos == end

		auto uri = make_unique(HttpUri);

		auto pos = uri.begin();
		auto beg = pos;
		auto end = uri.end();
		// Scheme
		{
			auto it = SeekForChar(pos, end, ':');
			if (it == end) return nullptr;
			uri.Scheme = string(pos, it);
		}
		auto begAfterScheme = pos;
		if (_L_NE || *(++pos) != '/') return nullptr; // At least one slash should follow.

		// Authority
		if (_L_NE || *(++pos) == '/') // A second slash present, user info present.
		{
			auto it = SeekForChar(pos, end, '/');
			if (it == end) return nullptr;
			uri.Authority = string(pos, it);
		}

		// Absent, there is no authority part.
		// Path
		{
			pos = begAfterScheme;
			auto it = SeekForChar(pos, end, '?');
			if (it == end) return nullptr;
			uri.Path = string(pos, it);
		}

		// Query
		// Fragment
#undef _L_NE
	}
}
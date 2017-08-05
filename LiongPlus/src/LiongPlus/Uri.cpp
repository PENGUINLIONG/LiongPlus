// File: Uri.cpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/Uri.hpp"
#include "LiongPlus/AnsiCharUtils.hpp"

_L_NS_BEG
using namespace std;
using namespace LiongPlus::AnsiCharUtils;

Uri::Uri(Uri&& instance)
{
	swap(Scheme, instance.Scheme);
	swap(Authority, instance.Authority);
	swap(Path, instance.Path);
	swap(Query, instance.Query);
	swap(Fragment, instance.Fragment);
}
Uri::Uri(const string& scheme, const string& authority, const string& path, const string& query, const string& fragment)
	: Scheme(scheme)
	, Authority(authority)
	, Path(path)
	, Query(query)
	, Fragment(fragment)
{}

bool Uri::Verify()
{
		

	return false;
}

string Uri::ToString()
{
	string rv = Scheme + ':';
	if (!Authority.empty()) rv += "//" + Authority;
	rv += Path;
	if (!Query.empty()) rv += '?' + Query;
	if (!Fragment.empty()) rv += '#' + Fragment;

	return rv;
}

unique_ptr<Uri> Uri::Parse(const string& uri)
{
	using namespace AnsiCharUtils;

	auto rv = make_unique<Uri>();

	auto pos = uri.begin(), beg = pos, end = uri.end();
	string::const_iterator it;

	// Scheme
	it = SeekForChar(pos, end, ':');
	if (it == end) return nullptr; // Scheme absent.
	rv->Scheme = string(pos, it);
	pos = ++it;

	if (pos == end || *(pos++) != '/') return nullptr; // At least one slash should follow.
	auto posOfFirstSlash = pos;

	// Authority
	if (pos != end && *(pos++) == '/') // A second slash present.
	{
		it = SeekForChar(pos, end, '/');
		if (it == end) return nullptr; // Path absent.
		rv->Authority = string(pos, it);
		pos = it;
	}
	// Absent, there is no authority part.
	else pos = posOfFirstSlash; // The first slash is the beginning of path.

	// Path
	it = SeekForChar(pos, end, '?');
	rv->Path = string(pos, it);
	if (it == end) return rv;
	pos = it;

	// Query
	if (pos != end && *(pos++) == '?')
	{
		it = SeekForChar(pos, end, '#');
		rv->Query = string(pos, it);
		if (it == end) return rv;
		pos = it;
	}

	// Fragment
	if (pos != end && *(pos++) == '#')
		rv->Fragment = string(pos, end);

	return rv;
}

auto schemeCondition = [](const char c){ return IsAlphabet(c) || IsDigit(c) || c == '+' || c == '-' || c == '.'; };
auto genDelimCondition = [](const char c){ return c == ':' || c == '/' || c == '?' || c == '#' || c == '[' || c == ']' || c == '@';};
auto subDelimCondition = [](const char c) { return c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' || c == '+' || c == ',' || c == ';' || c == '='; };
auto unreservedCondition = [](const char c){ return IsAlphabet(c) || IsDigit(c) || c == '-' || c == '.' || c == '_'|| c == '~'; };
auto userInfoCondition = [](const char c){ return unreservedCondition(c) || subDelimCondition(c) || c == '%'; };
auto pcharCondition = [](const char c) { return unreservedCondition(c) || subDelimCondition(c) || c == '%' || c == ':' || c == '@'; };

_L_NS_END

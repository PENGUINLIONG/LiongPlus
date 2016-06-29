// File: Uri.cpp
// Author: Rendong Liang (Liong)
#include "Uri.hpp"
#include "AnsiCharUtils.hpp"

namespace LiongPlus
{
	Uri::Uri(Uri&& instance)
	{
		swap(Scheme, instance.Scheme);
		swap(Authority.UserInfo.Username, instance.Authority.UserInfo.Username);
		swap(Authority.UserInfo.Password, instance.Authority.UserInfo.Password);
		swap(Authority.Host, instance.Authority.Host);
		swap(Authority.Port, instance.Authority.Port);
		swap(Path, instance.Path);
		swap(Query, instance.Query);
		swap(Fragment, instance.Fragment);
	}
	Uri::Uri(string scheme, string username, string password, string host, string port, string path, string query, string fragment)
		: Scheme(scheme)
		, Authority{ { username, password }, host, port }
		, Path(path)
		, Query(query)
		, Fragment(fragment)
	{
	}
	Uri::Uri(string scheme, string host, string port, string path)
		: Uri(scheme, "", "", host, port, path, "", "")
	{
	}
	Uri::Uri(string scheme, string host, string path)
		: Uri(scheme, host, "", path)
	{
	}

	string Uri::ToString()
	{
		stringstream ss;
		ss << Scheme << ":";
		if (!Authority.IsEmpty())
		{
			ss << "//";
			if (Authority.UserInfo.IsEmpty())
				ss << Authority.UserInfo.Username << ':' << Authority.UserInfo.Password << '@';
			ss << Authority.Host;
			if (Authority.Port.empty())
				ss << ':' << Authority.Port;
		}
		ss << Path;
		if (Query.empty())
			ss << '?' << Query;
		if (Fragment.empty())
			ss << '#' << Fragment;

		return ss.str();
	}

	//
	// UriAuthority
	//

	bool Uri::UriAuthority::IsEmpty()
	{
		return UserInfo.IsEmpty() && Host == "" && Port == "";
	}

	//
	// UriUserInfo
	//

	bool Uri::UriAuthority::UriUserInfo::IsEmpty()
	{
		return Username == "" && Password == "";
	}
}
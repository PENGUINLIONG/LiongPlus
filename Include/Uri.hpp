// File: Uri.hpp
// Author: Rendong Liang (Liong)
#include "Fundamental.hpp"

using namespace std;

namespace LiongPlus
{
	struct Uri
	{
		string Scheme;
		struct UriAuthority
		{
			struct UriUserInfo
			{
				string Username;
				string Password;

				bool IsEmpty();
			} UserInfo;
			string Host;
			string Port;

			bool IsEmpty();
		} Authority;
		string Path;
		string Query;
		string Fragment;

		Uri() = default;
		Uri(const Uri&) = default;
		Uri(Uri&&);
		Uri(string scheme, string username, string password, string host, string port, string path, string query, string fragment);
		Uri(string scheme, string host, string port, string path);
		Uri(string scheme, string host, string path);

		string ToString();
	};
}
// File: Uri.hpp
// Author: Rendong Liang (Liong)
#include "Fundamental.hpp"

namespace LiongPlus
{
	/*
	 * General Delimiter:
	 *   : / ? # [ ] @
	 * Sub Delimiter:
	 *   ! $ & ' ( ) * + , ; =
	 * Unreserved:
	 *   Alphabets Digits - . _ ~
	 */
	struct Uri
	{
	public:
		std::string Scheme;
		std::string Authority;
		std::string Path;
		std::string Query;
		std::string Fragment;

		Uri() = default;
		Uri(const Uri&) = default;
		Uri(Uri&&);
		Uri(std::string scheme, std::string authority, std::string path, std::string query = std::string(), std::string fragment = std::string());
		Uri(std::string scheme, std::string authority, std::string path, std::string fragment);

		virtual std::string ToString();

		static Uri Parse(std::string uri);
	};

	struct HttpUri
		: public Uri
	{
	private:
		static const std::string _StrHttp;
		static const std::string _StrHttps;

	public:
		HttpUri() = delete;
		HttpUri(const HttpUri&);
		HttpUri(HttpUri&&);
		HttpUri(std::string authority, std::string path, std::string query = std::string(), std::string fragment = std::string(), bool isHttps = false);
		HttpUri(std::string authority, std::string path, std::string fragment, bool isHttps = false);

		bool IsHttp() const;
		bool IsHttps() const;

		static Uri Parse(std::string uri);
	};

	struct FileUri
		: public Uri
	{
		FileUri() = delete;
		FileUri(const FileUri&);
		FileUri(FileUri&&);
		FileUri(std::string path);

		virtual std::string ToString();

		static HttpUri Parse(std::string uri);
	}


}

#undef _L_URI_MAKE_CONSTRUCTORS

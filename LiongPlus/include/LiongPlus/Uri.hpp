// File: Uri.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <string>
#include <memory>
#include "_"

_L_NS_BEG

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
	Uri(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query = std::string(), const std::string& fragment = std::string());

	/*
		* Verify if the chars are valid in each section.
		* Returns:
		*   True if all the chars are valid.
		*/
	virtual bool Verify();
	virtual std::string ToString();

	static std::unique_ptr<Uri> Parse(const std::string& uri);
};

_L_NS_END

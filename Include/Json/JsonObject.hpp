#pragma once
#include <map>
#include <cstdint>
#include <string>
#include <utility>
#include "_"
#include "../Array.hpp"
#include "../StringLiteral.hpp"

_L_NS_BEG_JSON


#define _L_JSON_FIELD

/*
template<typename T, typename TStringLiteral>
struct JsonField;
template<typename T, char ... TChars>
struct JsonField<T, StringLiteral::StringLiteral<TChars ...>>
{
public:
	typedef StringLiteral::StringLiteral<TChars ...> key_type;
	typedef T value_type;
	T Value;
};
*/

enum JsonParseErrorCodes : int_fast16_t
{
	Success_Perfect             = 0b0'000'0000'0000'0000,

	Error_Mask                  = 0b1'000'0000'0000'0000,
};

class JsonObject
{
private:
	std::map<std::string, std::string> _Fields;
public:

	template<typename T> T Get(const std::string& key);
	template<> JsonObject Get<JsonObject>(const std::string& key)
	{
		JsonObject target;
		TryParse(target, _Fields[key]);
		return target;
	}
	template<> Array<JsonObject> Get<Array<JsonObject>>(const std::string& key)
	{
		
	}
	template<> std::string Get<std::string>(const std::string& key) { return _Fields[key]; }
	template<> int Get<int>(const std::string& key) { return std::stoi(_Fields[key]); }
	template<> long Get<long>(const std::string& key) { return std::stol(_Fields[key]); }
	template<> unsigned long Get<unsigned long>(const std::string& key) { return std::stoul(_Fields[key]); }
	template<> long long Get<long long>(const std::string& key) { return std::stoll(_Fields[key]); }
	template<> unsigned long long Get<unsigned long long>(const std::string& key) { return std::stoull(_Fields[key]); }
	template<> float Get<float>(const std::string& key) { return std::stof(_Fields[key]); }
	template<> double Get<double>(const std::string& key) { return std::stod(_Fields[key]); }
	template<> long double Get<long double>(const std::string& key) { return std::stold(_Fields[key]); }

	template<typename T> void Add(const std::string& key, const T& value) { }

	static bool TryParse(JsonObject target, const std::string& json)
	{

	}
};

_L_NS_END_JSON

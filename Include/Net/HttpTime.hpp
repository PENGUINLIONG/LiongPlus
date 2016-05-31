#pragma once
#include<tuple>
#include<string>

namespace LiongPlus
{
	namespace Net
	{
		class HttpTime
		{
		private:
			// HttpTime store date time information in form of string, formatted in RFC1123.
			// Rfc1123 date format is:
			// [wkday], [day] [monthAbbr] [year] [hour]:[min]:[sec] GMT
			// e.g., Sun, 06 Nov 1994 08:49:37 GMT
			std::string _DateTime;
			
			void ParseFormat(string& str);
			void ParseRfc850(string& str);
			void ParseAsctime(string& str);
			
			static const char* MonthsAbbr[12];
			static const char* WkdaysAbbr[7];
		public:
			// Retrieve the time 'now'.
			HttpTime();
			HttpTime(long weekday, long year, long month, long date, long hour, long min, long sec);
			// Parse.
			HttpTime(const char* str);
			
			long GetWeekday();
			long GetDay();
			long GetMonth();
			long GetYear();
			std::tuple<long, long, long> GetTime();
			
			void Plus(const long seconds);
			void Minus(const long seconds);
		}
	}
}
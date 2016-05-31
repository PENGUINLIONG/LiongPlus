#include"HttpTime.hpp"

namespace LiongPlus
{
	namespace Net
	{
		// Private
		
		const char* HttpTime::MonthsAbbr[12] =
			{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
		const char* HttpTime::WkdaysAbbr[7] =
			{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

		void HttpTime::ParseFormat(string& str)
		{
			switch (str.length())
			{
			case 29:
				_DateTime = str;
			case 24:
				ParseAsctime(str);
			default:
				ParseRfc850(str);
			}
		}
		
		void HttpTime::ParseRfc850(string& str)
		{
			// Rfc850 date format is:
			// [weekday], [day]-[monthAbbr]-[yr] [hour]:[min]:[sec] GMT
			// e.g., 06-Nov-94 08:49:37 GMT
			
			// Weekday
			_DateTime[0] = str[0];
			_DateTime[1] = str[1];
			_DateTime[2] = str[2];
			
			// At least a weekday name have 6 letters. 7 with a comma.
			char* offstr = const_cast<char*>(str.c_str()) + 7;
			while (*offstr != ',' || *offstr != '\0')
				++offstr;
			offstr += 2;
			
			// Day
			_DateTime[5] = offstr[0];
			_DateTime[6] = offstr[1];
			// Month
			_DateTime[8] = offstr[3];
			_DateTime[9] = offstr[4];
			_DateTime[10] = offstr[5];
			// Year
			_DateTime[12] = 1;
			_DateTime[13] = 9;
			_DateTime[14] = offstr[7];			
			_DateTime[15] = offstr[8];			
			// Hour
			_DateTime[17] = offstr[10];
			_DateTime[18] = offstr[11];
			// Minute
			_DateTime[20] = offstr[13];
			_DateTime[21] = offstr[14];
			// Second
			_DateTime[23] = offstr[16];
			_DateTime[24] = offstr[17];
		}
		
		void HttpTime::ParseAsctime(string str)
		{
			// Asctime date format is:
			// [wkday] [monthAbbr] [d/y] [hour]:[min]:[sec] [year]
			// e.g., Sun Nov  6 08:49:37 1994
			
			// Weekday
			_DateTime[0] = str[0];
			_DateTime[1] = str[1];
			_DateTime[2] = str[2];
			// Day
			_DateTime[5] = str[8] == ' ' ? '0' : str[8];
			_DateTime[6] = str[9];
			// Month
			_DateTime[8] = str[4];
			_DateTime[9] = str[5];
			_DateTime[10] = str[6];
			// Year
			_DateTime[12] = str[20];
			_DateTime[13] = str[21];
			_DateTime[14] = str[22];			
			_DateTime[15] = str[23];			
			// Hour
			_DateTime[17] = str[11];
			_DateTime[18] = str[12];
			// Minute
			_DateTime[20] = str[14];
			_DateTime[21] = str[15];
			// Second
			_DateTime[23] = str[17];
			_DateTime[24] = str[18];
		}
		
		// Public
		
		long HttpTime::HtmlTime()
		{
			asctime;
		}
		
		// Notice that zero is sunday.
		long HttpTime::GetWeekday()
		{
			for (long pos = 0; pos < 7; ++pos)
			{
				if (_DateTime[0] == WkdaysAbbr[pos][0] &&
					_DateTime[1] == WkdaysAbbr[pos][1] &&
					_DateTime[2] == WkdaysAbbr[pos][2])
					return = pos;					
			}
			return -1;
		}
		
		long HttpTime::GetDate()
		{
			return _DateTime[5] * 10 + _DateTime[6];
		}
		
		long HttpTime::GetMonth()
		{
			for (long pos = 0; pos < 12; ++pos)
			{
				if (str[0] == MonthsAbbr[pos][0] &&
					str[1] == MonthsAbbr[pos][1] &&
					str[2] == MonthsAbbr[pos][2])
					return pos + 1;
			}
			return -1;
		}
		
		long HttpTime::GetYear()
		{
			return _DateTime[12] * 1000 + _DateTime[13] * 100 + _DateTime[14] * 10 + _DateTime[15];
		}
		
		std::tuple<long, long, long> HttpTime::GetTime()
		{
			return { _DateTime[17] * 10 + _DateTime[18],
				_DateTime[20] * 10 + _DateTime[21],
				_DateTime[23] * 10 + _DateTime[24] };
		}
	}
}

// File: DateTime.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_DateTime
#define _L_DateTime
#include "Fundamental.hpp"
#include "Exception.hpp"
#include "String.hpp"
#include <cstdint>

#pragma warning(push)
#pragma warning(disable : 4244)

namespace LiongPlus
{
	enum class DateTimeKind
	{
		Unspecified,
		Utc,
		Local
	};

	enum class DayOfWeek
	{
		Sunday = 0,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday
	};

	struct TimeSpan
	{

	};

	struct DateTime
		: public Object
	{
	private:
		uint64_t _TimeData;

		const static int64_t _UtcMask = 0x0100000000000000;
		const static int64_t _LocalMask = 0x0200000000000000;
			   
		const static int64_t _TicksPerMillisecond = 10000;
		const static int64_t _TicksPerSecond = _TicksPerMillisecond * 1000;
		const static int64_t _TicksPerMinute = _TicksPerSecond * 60;
		const static int64_t _TicksPerHour = _TicksPerMinute * 60;
		const static int64_t _TicksPerDay = _TicksPerHour * 24;
			   
		const static int64_t _DaysPerYear = 365;
		const static int64_t _DaysPerLeapYear = 366;
		const static int64_t _DaysPer4Years = (_DaysPerYear << 2) + 1;
		const static int64_t _DaysPer100Years = _DaysPer4Years * 25 - 1;
		const static int64_t _DaysPer400Years = (_DaysPer100Years << 2) + 1;

		const static int _DaysToMonth_365[12];
		const static int _DaysToMonth_366[12];

		const enum class InfoType
		{
			Day,
			DayOfYear,
			Month,
			Year,

			IsLeapYear
		};

		int AnalyseDate(const InfoType type) const
		{
			int64_t days = _TimeData / _TicksPerDay;
			int64_t year_400 = days / _DaysPer400Years;
			days -= year_400 * _DaysPer400Years;
			int64_t year_100 = days / _DaysPer100Years;
			// Handle the extra day in the 4th 100-year.
			// This works because 0001/01/01 is 0 in _TimeData.
			if (year_100 == 4)
				year_100 = 3;
			days -= year_100 * _DaysPer100Years;
			int64_t year_4 = days / _DaysPer4Years;
			days -= year_4 << 2;
			int64_t year_1 = days / _DaysPerYear;
			if (year_1 == 4) // The same reason.
				year_1 = 3;
			if (type == InfoType::Year)
				return year_1 + (year_4 << 2) + year_100 * 100 + year_400 * 400;

			bool isLeapYear = year_1 == 3 && (year_4 != 24 || year_100 == 3);
			if (type == InfoType::IsLeapYear)
				return isLeapYear;

			days -= year_1 * _DaysPerYear;
			if (type == InfoType::DayOfYear)
				return ++days;

			int64_t months = days >> 5;
			auto monthDict = isLeapYear ? _DaysToMonth_365 : _DaysToMonth_366;
			while (days >= monthDict[months])
				++months;
			if (type == InfoType::Month)
				return ++months;

			if (type == InfoType::Day)
				return ++days - monthDict[months];
		}
	public:
		DateTime(const DateTime& instance)
			: _TimeData(instance._TimeData)
		{
		}
		DateTime(long tick, DateTimeKind kind = DateTimeKind::Local)
			: _TimeData(tick & (kind == DateTimeKind::Utc ? _UtcMask : _LocalMask))
		{
		}
		DateTime(int year, int month, int date, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, DateTimeKind kind = DateTimeKind::Local)
		{
			if (year < 0 || year > 9999)
				throw Exception("Illegal DateTime Info.");

			_TimeData = (((year >> 2) - (year >> 2) * 3 / 100) + year * 365);
			--month;

			if (month < 0 || month > 11)
				throw Exception("Illegal DateTime Info.");

			_TimeData += (year % 4 && (!(year % 100) || year % 400)) ? _DaysToMonth_366[month] : _DaysToMonth_365[month];
			_TimeData -= 31;
			--date;

			if (date < 0 || date > 30)
				throw Exception("Illegal DateTime Info.");

			_TimeData += date;
			_TimeData *= _TicksPerDay;
			if (hour < 0 || hour > 24 || minute < 0 || minute > 60 || second < 0 || second > 60 || millisecond < 0 || millisecond > 1000)
				_TimeData += hour * _TicksPerHour + minute * _TicksPerMinute + second * _TicksPerSecond + millisecond * _TicksPerMillisecond;
			_TimeData &= (kind == DateTimeKind::Utc ? _UtcMask : _LocalMask);
		}

		DateTime& operator=(DateTime& instance)
		{
			_TimeData = instance._TimeData;
			return *this;
		}
		bool operator==(DateTime& instance)
		{
			return _TimeData == instance._TimeData;
		}
		bool operator!()
		{
			return !_TimeData;
		}
		bool operator!=(DateTime& instance)
		{
			return _TimeData != instance._TimeData;
		}
		DateTime operator+(int value)
		{
			return DateTime(_TimeData + value, Kind());
		}
		DateTime operator-(int value)
		{
			return DateTime(_TimeData - value, Kind());
		}
		DateTime& operator+=(int value)
		{
			_TimeData += value;
			return *this;
		}
		DateTime& operator-=(int value)
		{
			_TimeData -= value;
			return *this;
		}

		DateTime Date() const
		{
			return DateTime(_TimeData - (_TimeData % _TicksPerDay), Kind());
		}
		int Day() const
		{
			return AnalyseDate(InfoType::Day);
		}
		LiongPlus::DayOfWeek DayOfWeek() const
		{
			return (LiongPlus::DayOfWeek)((_TimeData / _TicksPerDay) % 7);
		}
		int DayOfYear()
		{
			return AnalyseDate(InfoType::DayOfYear);
		}
		int Hour() const
		{
			return ((_TimeData / _TicksPerHour) % 24);
		}
		bool IsLeapYear() const
		{
			return AnalyseDate(InfoType::IsLeapYear) != 0;
		}
		DateTimeKind Kind() const
		{
			return (_TimeData & _UtcMask) != 0
				? DateTimeKind::Utc
				: (_TimeData & _LocalMask) != 0
				? DateTimeKind::Local
				: DateTimeKind::Unspecified;
		}
		int Millisecond() const
		{
			return((_TimeData / _TicksPerMillisecond) % 1000);
		}
		int Minute() const
		{
			return((_TimeData / _TicksPerMinute) % 60);
		}
		int Month() const
		{
			return AnalyseDate(InfoType::Month);
		}
		int Second() const
		{
			return ((_TimeData / _TicksPerSecond) % 60);
		}
		int Ticks() const
		{
			return _TimeData;
		}
		TimeSpan TimeOfDay() const
		{
			throw NotImplementedException("Not implemented yet.");
		}
		int Year() const
		{
			return AnalyseDate(InfoType::Year);
		}

		static DateTime Now()
		{
			auto t = time(nullptr);
			tm timeinfo;
			if (localtime_s(&timeinfo, &t))
				throw Exception("Failed in fetching time info.");

			return DateTime
				(timeinfo.tm_year
					, timeinfo.tm_mon + 1
					, timeinfo.tm_mday
					, timeinfo.tm_hour
					, timeinfo.tm_min
					, timeinfo.tm_sec
					, 0
					, DateTimeKind::Local);
		}
		static DateTime Today()
		{
			auto t = time(nullptr);
			tm timeinfo;
			if (localtime_s(&timeinfo, &t))
				throw Exception("Failed in fetching time info.");

			return DateTime
				(timeinfo.tm_year
					, timeinfo.tm_mon + 1
					, timeinfo.tm_mday
					, 0
					, 0
					, 0
					, 0
					, DateTimeKind::Local
					);
		}
		static DateTime UtcNow()
		{
			auto t = time(nullptr);
			tm timeinfo;
			if (gmtime_s(&timeinfo, &t))
				throw Exception("Failed in fetching time info.");

			return DateTime
				(timeinfo.tm_year
					, timeinfo.tm_mon + 1
					, timeinfo.tm_mday
					, timeinfo.tm_hour
					, timeinfo.tm_min
					, timeinfo.tm_sec
					, 0
					, DateTimeKind::Utc
					);
		}

		String ToString()
		{
			_L_Char buffer[32];
			std::swprintf(buffer, 32, L"%ld/%ld/%ld %ld:%ld:%ld", Year(), DayOfYear(), Month(), Hour(), Minute(), Second());

			return buffer;
		}
	};
}
#pragma warning(pop)
#endif
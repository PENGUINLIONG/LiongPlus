// File: DateTime.cpp
// Author: Rendong Liang (Liong)

#include "DateTime.hpp"

namespace LiongPlus
{
	// Public

	DateTime::DateTime()
		: _TimeData(0)
	{
	}
	DateTime::DateTime(const DateTime& instance)
		: _TimeData(instance._TimeData)
	{
	}
<<<<<<< HEAD
	DateTime::DateTime(uint64_t tick, DateTimeKind kind)
		: _TimeData(tick & (kind == DateTimeKind::Utc ? _UtcMask :(kind == DateTimeKind::Local ? _LocalMask : 0)))
=======
	DateTime::DateTime(DateTime&& instance)
		: _TimeData(0)
>>>>>>> master
	{
		Swap(_TimeData, instance._TimeData);
	}
<<<<<<< HEAD
	DateTime::DateTime(long year, long month, long date, long hour, long minute, long second, long millisecond, DateTimeKind kind)
=======
	DateTime::DateTime(long tick, DateTimeKind kind)
		: _TimeData(tick | (kind == DateTimeKind::Utc ? _UtcMask : _LocalMask))
>>>>>>> master
	{
	}
	DateTime::DateTime(long year, long month, long date, long hour, long minute, long second, long millisecond, DateTimeKind kind)
	{
		assert(year >= 0 && year <= 9999, "Illegal DateTime Info.");
		_TimeData = (((year >> 2) - (year >> 2) * 3 / 100) + year * 365);
		--month;
		assert(month >= 0 && month <= 11, "Illegal DateTime Info.");
		_TimeData += (year % 4 && (!(year % 100) || year % 400)) ? _DaysToMonth_366[month] : _DaysToMonth_365[month];
		_TimeData -= 31;
		--date;
		assert(date >= 0 && date <= 30, "Illegal DateTime Info.");
		_TimeData += date;
		_TimeData *= _TicksPerDay;
		assert(hour >= 0 || hour < 24 || minute >= 0 || minute < 60 || second >= 0 || second < 60 || millisecond >= 0 || millisecond < 1000);
		_TimeData += hour * _TicksPerHour + minute * _TicksPerMinute + second * _TicksPerSecond + millisecond * _TicksPerMillisecond;
		_TimeData |= (kind == DateTimeKind::Utc ? _UtcMask : _LocalMask);
	}

	DateTime& DateTime::operator=(DateTime& instance)
	{
		_TimeData = instance._TimeData;
		return *this;
	}
	bool DateTime::operator==(DateTime& instance)
	{
		return _TimeData == instance._TimeData;
	}
	bool DateTime::operator!()
	{
		return !_TimeData;
	}
	bool DateTime::operator!=(DateTime& instance)
	{
		return _TimeData != instance._TimeData;
	}
	DateTime DateTime::operator+(long value)
	{
		return DateTime(_TimeData + value, Kind());
	}
	DateTime DateTime::operator-(long value)
	{
		return DateTime(_TimeData - value, Kind());
	}
	DateTime& DateTime::operator+=(long value)
	{
		_TimeData += value;
		return *this;
	}
	DateTime& DateTime::operator-=(long value)
	{
		_TimeData -= value;
		return *this;
	}

	DateTime DateTime::Date() const
	{
		return DateTime((_TimeData & _TickMask) - ((_TimeData & _TickMask) % _TicksPerDay), Kind());
	}
	long DateTime::Day() const
	{
		return AnalyseDate(InfoType::Day);
	}
	LiongPlus::DayOfWeek DateTime::DayOfWeek() const
	{
		return (LiongPlus::DayOfWeek)(((_TimeData & _TickMask) / _TicksPerDay) % 7);
	}
	long DateTime::DayOfYear()
	{
		return AnalyseDate(InfoType::DayOfYear);
	}
	long DateTime::Hour() const
	{
		return (((_TimeData & _TickMask) / _TicksPerHour) % 24);
	}
	bool DateTime::IsLeapYear() const
	{
		return AnalyseDate(InfoType::IsLeapYear) != 0;
	}
	DateTimeKind DateTime::Kind() const
	{
		return (_TimeData & _UtcMask) != 0
			? DateTimeKind::Utc
			: (_TimeData & _LocalMask) != 0
			? DateTimeKind::Local
			: DateTimeKind::Unspecified;
	}
	long DateTime::Millisecond() const
	{
		return(((_TimeData & _TickMask) / _TicksPerMillisecond) % 1000);
	}
	long DateTime::Minute() const
	{
		return(((_TimeData & _TickMask) / _TicksPerMinute) % 60);
	}
	long DateTime::Month() const
	{
		return AnalyseDate(InfoType::Month);
	}
	long DateTime::Second() const
	{
		return (((_TimeData & _TickMask) / _TicksPerSecond) % 60);
	}
<<<<<<< HEAD
	uint64_t DateTime::Ticks() const
=======
	long DateTime::Ticks() const
>>>>>>> master
	{
		return (_TimeData & _TickMask);
	}
	TimeSpan DateTime::TimeOfDay() const
	{
		throw std::logic_error("Not implemented yet.");
	}
	std::string DateTime::To_L_String()
	{
<<<<<<< HEAD
		char buffer[32];
		std::sprintf(buffer, "%ld/%ld/%ld %ld:%ld:%ld", Year(), DayOfYear(), Month(), Hour(), Minute(), Second());

		return buffer;
=======
		LiongPlus::Text::StringBuilder sb;
		sb.Append(String::FromValue(Year()));
		sb.Append(_LT('/'));
		sb.Append(String::FromValue(Month()));
		sb.Append(_LT('/'));
		sb.Append(String::FromValue(Day()));
		sb.Append(_LT(' '));
		sb.Append(String::FromValue(Hour()));
		sb.Append(_LT(':'));
		sb.Append(String::FromValue(Minute()));
		sb.Append(_LT(':'));
		sb.Append(String::FromValue(Second()));
		return sb.ToString();
>>>>>>> master
	}
	long DateTime::Year() const
	{
		return AnalyseDate(InfoType::Year);
	}

	// Static

	DateTime DateTime::Now()
	{
		auto t = time(nullptr);
		tm timeinfo;
		if (localtime_s(&timeinfo, &t))
			throw std::runtime_error("Failed in fetching time info.");

		return DateTime
			(timeinfo.tm_year + 1900
				, timeinfo.tm_mon + 1
				, timeinfo.tm_mday
				, timeinfo.tm_hour
				, timeinfo.tm_min
				, timeinfo.tm_sec
				, 0
				, DateTimeKind::Local);
	}
	DateTime DateTime::Today()
	{
		auto t = time(nullptr);
		tm timeinfo;
		if (localtime_s(&timeinfo, &t))
			throw std::runtime_error("Failed in fetching time info.");

		return DateTime
			(timeinfo.tm_year + 1900
				, timeinfo.tm_mon + 1
				, timeinfo.tm_mday
				, 0
				, 0
				, 0
				, 0
				, DateTimeKind::Local
				);
	}
	DateTime DateTime::UtcNow()
	{
		auto t = time(nullptr);
		tm timeinfo;
		if (gmtime_s(&timeinfo, &t))
			throw std::runtime_error("Failed in fetching time info.");

		return DateTime
			(timeinfo.tm_year + 1900
				, timeinfo.tm_mon + 1
				, timeinfo.tm_mday
				, timeinfo.tm_hour
				, timeinfo.tm_min
				, timeinfo.tm_sec
				, 0
				, DateTimeKind::Utc
				);
	}

	// Private

<<<<<<< HEAD
	const int DateTime::_DaysToMonth_365[12] = { 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	const int DateTime::_DaysToMonth_366[12] = { 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
=======
	const uint64_t DateTime::_TickMask = 0x3FFFFFFFFFFFFFFFULL;
	const uint64_t DateTime::_UtcMask =  0x4000000000000000ULL;
	const uint64_t DateTime::_LocalMask = 0x8000000000000000ULL;

	const uint64_t DateTime::_TicksPerMillisecond = 10000;
	const uint64_t DateTime::_TicksPerSecond = _TicksPerMillisecond * 1000;
	const uint64_t DateTime::_TicksPerMinute = _TicksPerSecond * 60;
	const uint64_t DateTime::_TicksPerHour = _TicksPerMinute * 60;
	const uint64_t DateTime::_TicksPerDay = _TicksPerHour * 24;

	const uint64_t DateTime::_DaysPerYear = 365;
	const uint64_t DateTime::_DaysPerLeapYear = 366;
	const uint64_t DateTime::_DaysPer4Years = (_DaysPerYear << 2) + 1;
	const uint64_t DateTime::_DaysPer100Years = _DaysPer4Years * 25 - 1;
	const uint64_t DateTime::_DaysPer400Years = (_DaysPer100Years << 2) + 1;

	const long DateTime::_DaysToMonth_365[12] = { 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	const long DateTime::_DaysToMonth_366[12] = { 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
>>>>>>> master

	long DateTime::AnalyseDate(const InfoType type) const
	{
		int64_t days = (_TimeData & _TickMask) / _TicksPerDay;
		int64_t year_400 = days / _DaysPer400Years;
		days -= year_400 * _DaysPer400Years;
		int64_t year_100 = days / _DaysPer100Years;
		// Handle the extra day in the 4th 100-year.
		// This works because 0001/01/01 is 0 in _TimeData.
		if (year_100 == 4)
			year_100 = 3;
		days -= year_100 * _DaysPer100Years;
		int64_t year_4 = days / _DaysPer4Years;
		days -= year_4 * _DaysPer4Years;
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
<<<<<<< HEAD
			return ++days - monthDict[months];
		return -1;
=======
			return ++days - (months == 0 ? 0 : monthDict[months]);
>>>>>>> master
	}
}
// File: DateTime.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_DateTime
#define _L_DateTime
#include "Fundamental.hpp"

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
	{
	public:
		DateTime(const DateTime& instance);
		DateTime(uint64_t tick, DateTimeKind kind = DateTimeKind::Local);
		DateTime(long year, long month, long date, long hour = 0, long minute = 0, long second = 0, long millisecond = 0, DateTimeKind kind = DateTimeKind::Local);

		DateTime& operator=(DateTime& instance);
		bool operator==(DateTime& instance);
		bool operator!();
		bool operator!=(DateTime& instance);
		DateTime operator+(long value);
		DateTime operator-(long value);
		DateTime& operator+=(long value);
		DateTime& operator-=(long value);

		DateTime Date() const;
		int Day() const;
		LiongPlus::DayOfWeek DayOfWeek() const;
		long DayOfYear();
		long Hour() const;
		bool IsLeapYear() const;
		DateTimeKind Kind() const;
		long Millisecond() const;
		long Minute() const;
		long Month() const;
		long Second() const;
		uint64_t Ticks() const;
		TimeSpan TimeOfDay() const;
		std::string To_L_String();
		long Year() const;

		static DateTime Now();
		static DateTime Today();
		static DateTime UtcNow();

	private:
		uint64_t _TimeData;

		const int64_t _UtcMask = 0x0100000000000000;
		const int64_t _LocalMask = 0x0200000000000000;

		const int64_t _TicksPerMillisecond = 10000;
		const int64_t _TicksPerSecond = _TicksPerMillisecond * 1000;
		const int64_t _TicksPerMinute = _TicksPerSecond * 60;
		const int64_t _TicksPerHour = _TicksPerMinute * 60;
		const int64_t _TicksPerDay = _TicksPerHour * 24;

		const int64_t _DaysPerYear = 365;
		const int64_t _DaysPerLeapYear = 366;
		const int64_t _DaysPer4Years = (_DaysPerYear << 2) + 1;
		const int64_t _DaysPer100Years = _DaysPer4Years * 25 - 1;
		const int64_t _DaysPer400Years = (_DaysPer100Years << 2) + 1;

		const static int _DaysToMonth_365[12];
		const static int _DaysToMonth_366[12];

		enum class InfoType
		{
			Day,
			DayOfYear,
			Month,
			Year,

			IsLeapYear
		};

		int AnalyseDate(const InfoType type) const;
	};
}
#pragma warning(pop)
#endif
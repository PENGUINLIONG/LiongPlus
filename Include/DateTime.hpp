// File: DateTime.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_DateTime
#define _L_DateTime
#include "Fundamental.hpp"
#include "Exception.hpp"
#include "String.hpp"
#include "Text/StringBuilder.hpp"

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
	public:
		DateTime();
		DateTime(const DateTime& instance);
		DateTime(DateTime&& instance);
		DateTime(long tick, DateTimeKind kind = DateTimeKind::Local);
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
		long Day() const;
		LiongPlus::DayOfWeek DayOfWeek() const;
		long DayOfYear();
		long Hour() const;
		bool IsLeapYear() const;
		DateTimeKind Kind() const;
		long Millisecond() const;
		long Minute() const;
		long Month() const;
		long Second() const;
		long Ticks() const;
		TimeSpan TimeOfDay() const;
		String ToString();
		long Year() const;

		static DateTime Now();
		static DateTime Today();
		static DateTime UtcNow();

	private:
		uint64_t _TimeData;

		const static uint64_t _TickMask;
		const static uint64_t _UtcMask;
		const static uint64_t _LocalMask;

		const static uint64_t _TicksPerMillisecond;
		const static uint64_t _TicksPerSecond;
		const static uint64_t _TicksPerMinute;
		const static uint64_t _TicksPerHour;
		const static uint64_t _TicksPerDay;

		const static uint64_t _DaysPerYear;
		const static uint64_t _DaysPerLeapYear;
		const static uint64_t _DaysPer4Years;
		const static uint64_t _DaysPer100Years;
		const static uint64_t _DaysPer400Years;

		const static long _DaysToMonth_365[12];
		const static long _DaysToMonth_366[12];

		enum class InfoType
		{
			Day,
			DayOfYear,
			Month,
			Year,

			IsLeapYear
		};

		long AnalyseDate(const InfoType type) const;
	};
}
#pragma warning(pop)
#endif
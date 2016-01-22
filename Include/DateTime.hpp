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
	public:
		DateTime(const DateTime& instance);
		DateTime(long tick, DateTimeKind kind = DateTimeKind::Local);
		DateTime(int year, int month, int date, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, DateTimeKind kind = DateTimeKind::Local);

		DateTime& operator=(DateTime& instance);
		bool operator==(DateTime& instance);
		bool operator!();
		bool operator!=(DateTime& instance);
		DateTime operator+(int value);
		DateTime operator-(int value);
		DateTime& operator+=(int value);
		DateTime& operator-=(int value);

		DateTime Date() const;
		int Day() const;
		LiongPlus::DayOfWeek DayOfWeek() const;
		int DayOfYear();
		int Hour() const;
		bool IsLeapYear() const;
		DateTimeKind Kind() const;
		int Millisecond() const;
		int Minute() const;
		int Month() const;
		int Second() const;
		int Ticks() const;
		TimeSpan TimeOfDay() const;
		String ToString();
		int Year() const;

		static DateTime Now();
		static DateTime Today();
		static DateTime UtcNow();

	private:
		uint64_t _TimeData;

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
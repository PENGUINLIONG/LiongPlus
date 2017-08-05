// File: AnsiStringUtils.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "DelegateDef.hpp"

namespace LiongPlus
{
	namespace AnsiCharUtils
	{
		constexpr bool IsLowerCase(const char c) { return c >= 'a' && c <= 'z'; }
		constexpr bool IsUpperCase(const char c) { return c >= 'A' && c <= 'Z'; }
		constexpr bool IsAlphabet(const char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
		constexpr bool IsDigit(const char c) { return c >= '0' && c <= '9'; }
		constexpr bool IsAlphaDigit(const char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }
		constexpr bool IsHexDigit(const char c) { return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }

		constexpr char ToUpperCase(const char c) { return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }
		constexpr char ToLowerCase(const char c) { return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }
		constexpr bool IsLineBreak(const char c) { return (c == '\r' || c == '\n'); }
		constexpr bool IsWhiteSpace(const char c) { return (c == '\t' || c == ' '); };

		/*
		 * Check whether the following two chars are CRLF.
		 * Params:
		 *   $pos: The position to check.
		 *   $end: The end of valid search range.
		 * Return:
		 *   True if the following 2 chars are CRLF. Otherwise, false.
		 */
		template<typename TStrIt>
		inline bool IsCrLf(TStrIt pos, TStrIt end)
		{
			if (pos == end) return false;
			if (*pos != '\r') return false;
			if (++pos != end && *pos == '\n') return true;
			else return false;
		}

		/*
		* Compare two strings case-insensitively (only for ASCII part).
		* Params:
		*   $a, $b: strings to be compared.
		* Return:
		*   True if two strings have case-insensitively equal content. Otherwise, false.
		*/
		template<typename TStr>
		inline bool CaseInsensitiveEquals(const TStr& a, const TStr& b)
		{
			if (a.length() != b.length()) return false;

			auto aPos = a.begin(), bPos = b.begin(), aEnd = a.end();
			while (aPos != aPos)
				if (ToUpperCase(*aPos) != ToUpperCase(*bPos)) return false; else ++aPos;
			return true;
		}
		/*
		 * Check if a section of string ($pos to $end) equals to another string($compareWith).
		 * Params:
		 *   $begRef: The beginning of 'reference' string to be compared with.
		 *   $endRef: The end of 'reference' string to be compared with.
		 * Returns:
		 *   $pos: The beginning of string section.
		 *   $end: The end of the string section.
		 *   True if the section equals to the 'reference' string. Otherwise, false.
		 */
		template<typename TStrItA, typename TStrItB>
		inline bool SectionalEquals(TStrItA begRef, TStrItA endRef, TStrItB beg, TStrItB end)
		{
			auto posRef = begRef;
			auto pos = beg;
			while (endRef != posRef && *posRef == *pos) ++posRef, ++pos;
			return endRef == posRef;
		}
		/*
		 * Check if a section of string ($pos to $end) satisfies the $condition.
		 * Params:
		 *   $pos: The beginning of string section.
		 *   $end: The end of the string section.
		 *   $condition: Function that determine whether a char is of equality.
		 * Returns:
		 *   True if the section satisfies the $condition. Otherwise, false.
		 */
		template<typename TStrIt>
		inline bool ConditionalEquals(TStrIt pos, TStrIt end, const Predicate<char>& condition)
		{
			while (pos != end)
				if (condition(*(pos++)) == false) return false;
			return true;
		}
		
		/*
		 * Skip a line break, that is, CR/LF/CRLF.
		 * Params:
		 *   $pos: The position of first char of line break.
		 *   $end: The end of valid search range.
		 * Return:
		 *   The position after the line break.
		 */
		template<typename TStrIt>
		inline TStrIt SkipLineBreak(TStrIt pos, TStrIt end)
		{ if (IsCrLf(pos, end)) return ++(++pos); else return ++pos; }
		/*
		 * Seek for a possible line break, that is, CR/LF/CRLF.
		 * Params:
		 *   $pos: The beginning of search.
		 *   $end: The end of search.
		 * Return:
		 *   The position of the first line-break char.
		 */
		template<typename TStrIt>
		inline TStrIt SeekForLineBreak(TStrIt pos, TStrIt end)
		{
			while (pos != end)
				if (IsLineBreak(*pos)) break; else ++pos;
			return pos;
		}
		/*
		* Seek for a char that is a whitespace, that is, ' ' or '\t'.
		* Params:
		*   $pos: The beginning of search.
		*   $end: The end of search.
		* Return:
		*   The position of the first whitespace char.
		*/
		template<typename TStrIt>
		inline TStrIt SeekForWhiteSpace(TStrIt pos, TStrIt end)
		{
			while (pos != end)
				if (IsWhiteSpace(*pos)) break; else ++pos;
			return pos;
		}
		/*
		* Seek for a char that is not a whitespace, that is, ' ' or '\t'.
		* Params:
		*   $pos: The beginning of search.
		*   $end: The end of search.
		* Return:
		*   The position of the first non-whitespace char.
		*/
		template<typename TStrIt>
		inline TStrIt SeekForNonWhiteSpace(TStrIt pos, TStrIt end)
		{
			while (pos != end)
				if (!IsWhiteSpace(*pos)) break; else ++pos;
			return pos;
		}
		/*
		* Seek for a given char, $c.
		* Params:
		*   $pos: The beginning of search.
		*   $end: The end of search.
		* Return:
		*   The position of the given char.
		*/
		template<typename TStrIt, typename ... TChars>
		inline TStrIt SeekForChar(TStrIt pos, TStrIt end, TChars ... cs)
		{
			while (pos != end)
				if (details::SeekForCharImpl(*pos, cs ...)) break; else ++pos;
			return pos;
		}
		/*
		* Seek for a possible $condition.
		* Params:
		*   $pos: The beginning of search.
		*   $end: The end of search.
		* Return:
		*   The position where the $condition is satisfied.
		*/
		template<typename TStrIt>
		inline TStrIt SeekFor(TStrIt pos, TStrIt end, Predicate<const char> condition)
		{
			while (pos != end)
				if (condition(*pos)) break; else ++pos;
			return pos;
		}
		/*
		 * Seek for the next line break and skip it.
		 * Params:
		 *   $pos: The position of first char of line break.
		 *   $end: The end of valid search range.
		 * Return:
		 *   The position of the first char in the new line.
		 */
		template<typename TStrIt>
		inline TStrIt SeekForNextLine(TStrIt pos, TStrIt end)
		{ return SkipLineBreak(SeekForLineBreak(pos, end), end); }

		/*
		 * Capture number literals ('0'-'9').
		 * Params:
		 *   $pos: [REF] The position of first char of line break.
		 *     This iterator will be moved to where next to the last digit captured. 
		 *   $end: The end of valid search range.
		 * Return:
		 *   The number literals captured.
		 */
		template<typename TStr, typename TStrIt>
		inline TStr CaptureDigits(TStrIt pos, TStrIt end)
		{
			auto beg = pos;
			while (IsDigit(*pos) && pos++ != end);
			return TStr(beg, pos);
		}
		/*
		 * Capture alphabets ('A'-'Z', 'a'-'z').
		 * Params:
		 *   $pos: [REF] The position of first char of line break.
		 *     This iterator will be moved to where next to the last alphabet captured. 
		 *   $end: The end of valid search range.
		 * Return:
		 *   The alphabets captured.
		 */
		template<typename TStr, typename TStrIt>
		inline TStr CaptureAlphabets(TStrIt pos, TStrIt end)
		{
			auto beg = pos;
			while (pos++ != end && IsAlphabet(*pos++));
			return TStr(beg, pos);
		}
		/*
		* Capture all non-whitespace.
		* Params:
		*   $pos: [REF] The position of first char of line break.
		*     This iterator will be moved to where next to the last char captured.
		*   $end: The end of valid search range.
		* Return:
		*   The chars captured.
		*/
		template<typename TStr, typename TStrIt>
		inline TStr CaptureNonWhiteSpace(TStrIt pos, TStrIt end)
		{
			auto beg = pos;
			while (pos != end && !IsWhiteSpace(*pos++));
			return TStr(beg, pos);
		}
		/*
		 * Capture chars according to the result of $condition.
		 * Params:
		 *   $pos: [REF] The position of first char of line break.
		 *     This iterator will be moved to where next to the last valid char captured.
		 *   $end: The end of valid search range.
		 *   $condition: Function that determine whether a char is part of the captured text.
		 * Return:
		 *   The chars captured.
		 */
		template<typename TStr, typename TStrIt>
		inline TStr CaptureIf(TStrIt pos, TStrIt end, Predicate<const char> condition)
		{
			auto beg = pos;
			while (pos != end && condition(*pos++));
			return TStr(beg, pos);
		}

		namespace details
		{
			template<typename TCurrent, typename TChar>
			constexpr bool SeekForCharImpl(TCurrent currentChar, TChar c)
			{
				return currentChar == c;
			}
			template<typename TCurrent, typename TChar, typename ... TArgs>
			constexpr bool SeekForCharImpl(TCurrent currentChar, TChar c, TArgs ... others)
			{
				return SeekForCharImpl(currentChar, c) || SeekForCharImpl(currentChar, others ...);
			}
		}
	}
}

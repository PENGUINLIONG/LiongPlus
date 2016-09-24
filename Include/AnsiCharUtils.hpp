// File: AnsiStringUtils.hpp
// Author: Rendong Liang (Liong)
#include "Fundamental.hpp"

namespace LiongPlus
{
	namespace AnsiCharUtils
	{
		inline bool IsLowerCase(const char c);
		inline bool IsUpperCase(const char c);
		inline bool IsAlphabet(const char c);
		inline bool IsDigit(const char c);
		inline bool IsAlphaDigit(const char c);
		inline bool IsHexDigit(const char c);

		inline char ToUpperCase(const char c);
		inline char ToLowerCase(const char c);
		/*
		 * Compare two strings case-insensitively (only for ASCII part).
		 * Params:
		 *   $a, $b: strings to be compared.
		 * Return:
		 *   True if two strings have case-insensitively equal content. Otherwise, false.
		 */ 
		inline bool CaseInsensitiveEquals(const std::string& a, const std::string& b);
		/*
		 * Check if the 
		 * Params:
		 *   $pos: Position of the char to be checked.
		 * Return:
		 *   True if the char is a line separator.
		 */
		inline bool IsLineSeparator(const std::string::iterator pos);
		/*
		 * Check whether the following two chars are CRLF.
		 * Params:
		 *   $pos: The position to check.
		 *   $end: The end of valid search range.
		 * Return:
		 *   True if the following 2 chars are CRLF. Otherwise, false.
		 */
		inline bool IsCrLf(const std::string::iterator pos, const std::string::iterator end);
		/*
		 * Seek for a possible line break, that is, CR/LF/CRLF.
		 * Params:
		 *   $pos: The beginning of search.
		 *   $end: The end of search.
		 * Return:
		 *   The position of the first line-break char.
		 */
		inline std::string::iterator SeekForLineBreak(const std::string::iterator pos, const std::string::iterator end);
		inline std::string::iterator SeekForChar(const std::string::iterator pos, const std::string::iterator end, const char c);
		inline std::string::iterator SeekFor(const std::string::iterator pos, const std::string::iterator end, Predicate<const char> condition);
		/*
		 * Skip a line break, that is, CR/LF/CRLF.
		 * Params:
		 *   $pos: The position of first char of line break.
		 *   $end: The end of valid search range.
		 * Return:
		 *   The position after the line break.
		 */
		inline std::string::iterator SkipLineBreak(const std::string::iterator pos, const std::string::iterator end);
		/*
		 * Seek for the next line break and skip it.
		 * Params:
		 *   $pos: The position of first char of line break.
		 *   $end: The end of valid search range.
		 * Return:
		 *   The position of the first char in the new line.
		 */
		inline std::string::iterator SeekForNextLine(const std::string::iterator pos, const std::string::iterator end);
		/*
		 * Capture number literals ('0'-'9').
		 * Params:
		 *   $pos: [REF] The position of first char of line break.
		 *     This iterator will be moved to the end of the number literals. 
		 *   $end: The end of valid search range.
		 * Return:
		 *   The number literals captured.
		 */
		inline std::string CaptureNumber(std::string::iterator& pos, const std::string::iterator end);
		/*
		 * Capture alphabets ('A'-'Z', 'a'-'z').
		 * Params:
		 *   $pos: [REF] The position of first char of line break.
		 *     This iterator will be moved to the end of the alphabets. 
		 *   $end: The end of valid search range.
		 * Return:
		 *   The alphabets captured.
		 */
		inline std::string CaptureWord(std::string::iterator& pos, const std::string::iterator end);
		/*
		 * Capture chars according to the result of $condition.
		 * Params:
		 *   $pos: [REF] The position of first char of line break.
		 *     This iterator will be moved to the end of the valid chars. 
		 *   $end: The end of valid search range.
		 *   $condition: Function that determine whether a char is part of the captured text.
		 * Return:
		 *   The chars captured.
		 */
		inline std::string Capture(std::string::iterator& pos, const std::string::iterator end, Predicate<const char> condition);

		inline bool CheckEquality(std::string::iterator& pos, const std::string::iterator end, const std::string compareWith);
	};
}
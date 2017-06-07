// File: SafeError.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <limits>
#include "_"

_L_NS_BEG
/*
 * Provide error reporting functionality without exception throwing.
 *
 * Usage:
 *   Define enumeration of type following convention: 
 *     enum [Error Name]Code : [Unsigned Integral]
 *     { ... }
 *   Alias to error type:
 *     using [Error Name] = SafeError<[Error Enum], [Unsigned Integral]>;
 *
 * Naming Error Enum:
 *   When the code is not representing an error, name it with prefix
 *    'Success_'. Otherwise, start it with 'Error_'.
 *
 * Special Error Codes:
 *   Success_Perfect: 0x0'000'0000; Reported when there is no error
 *     through out the process. When there is minor, non-fatal errors
 *     in execution, other success codes should be reported instead
 *     of this.
 *   Error_Mask: Bit mask indicates whether the current error code
 *     is representing an error. Must be the most significant bit of
 *     that integral type.
 */
template<typename TErrorEnum, typename T>
struct SafeError
{
private:
	T _Code;

public:
	SafeError(TErrorEnum code) :
		_Code(code)
	{
	}

	operator T() const
	{
		return _Code;
	}
	operator TErrorEnum() const
	{
		return (TErrorEnum)_Code;
	}

	bool IsSuccess() const
	{
		return !(_Code >> (sizeof(T) * 8 - 1));
	}
};

_L_NS_END

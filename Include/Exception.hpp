// File: Exception.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Exception
#define _L_Exception
#include "Fundamental.hpp"
#include <exception>

namespace LiongPlus
{
	/// <summary>
	/// Base type of all int fundamental exceptions.
	/// </summary>
	class Exception
		: public Object
		, protected std::exception
	{
	public:
		Exception(const char* description = "")
			: exception(description)
		{
		}

		virtual const char* Message()
		{
			return this->what();
		}
	};
#define L_KEEP_EXCEPTION_OCCURING(description) while (true) throw Exception(description)

	class ArgumentException
		: public Exception
	{
	public:
		ArgumentException(const char* description = "")
			: Exception(description)
		{
		}
	};

	class ArgumentNullException
		: public Exception
	{
	public:
		ArgumentNullException(const char* description = "")
			: Exception(description)
		{
		}
	};

	class ArgumentOutOfRangeException
		: public Exception
	{
	public:
		ArgumentOutOfRangeException(const char* description = "")
			: Exception(description)
		{
		}
	};

	class InvalidOperationException
		: public Exception
	{
	public:
		InvalidOperationException(const char* description = "")
			: Exception(description)
		{
		}
	};

	class NotImplementedException
		: public Exception
	{
	public:
		NotImplementedException(const char* description = "")
			: Exception(description)
		{
		}
	};

	class NotSupportedException
		: public Exception
	{
	public:
		NotSupportedException(const char* description = "")
			: Exception(description)
		{
		}
	};

	class NullReferenceException
		: public Exception
	{
	public:
		NullReferenceException(const char* description = "")
			: Exception(description)
		{
		}
	};

	/// <remarks>
	/// The method represented by the delegate is an instance method and the target object is null.
	/// -or -
	/// One of the encapsulated methods throws an exception.
	/// </remarks>
	class TargetInvocationException
		: public Exception
	{
	public:
		TargetInvocationException(const char* description = "")
			: Exception(description)
		{
		}
	};

	class UnsafeStandardizationException
		: public Exception
	{
	public:
		UnsafeStandardizationException(const char* description = "")
			: Exception(description)
		{
		}
	};

	namespace IO
	{
		class IOException
			: public Exception
		{
		public:
			IOException(const char* description)
				: Exception(description)
			{
			}
		};

		class PermissionDeniedException
			: public Exception
		{
		public:
			PermissionDeniedException(const char* description)
				: Exception(description)
			{
			}
		};
	}

	namespace Media
	{
		class InvalidFormatException
			: public Exception
		{
		public:
			InvalidFormatException(const char* description)
				: Exception(description)
			{
			}
		};
	}
}
#endif
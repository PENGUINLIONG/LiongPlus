// File: Stream.hpp
// Author: Rendong Liang (Liong)
#include "../Buffer.hpp"

#ifndef Stream_hpp
#define Stream_hpp
namespace LiongPlus
{
	namespace IO
	{
		enum SeekOrigin
		{
			Begin,
			Current,
			End
		};

		enum StreamAccessPermission
		{
			ReadOnly,
			WriteOnly,
			ReadWrite
		};

		class Stream
		{
		protected:
			StreamAccessPermission _Permission;

			Stream(StreamAccessPermission permission = StreamAccessPermission::ReadWrite)
				: _Permission(permission)
			{
			}

		public:
			virtual bool CanRead() = 0;
			virtual bool CanWrite() = 0;
			virtual bool CanSeek() = 0;
			/// <note>After a MemoryStream is closed, DO NOT reuse it because the result will be UNDEFINED.</note>
			virtual void Close() = 0;
			virtual void CopyTo(Stream& stream) = 0;
			virtual void CopyTo(Stream& stream, size_t length) = 0;
			virtual void Flush() = 0;
			virtual size_t Capacity() = 0;
			virtual size_t Length() = 0;
			virtual size_t Position() = 0;
			virtual bool IsEndOfStream() = 0;
			/// <return>A newly allocated buffer contains a serial data section of a specific length read from stream.</return>
			virtual Buffer Read(size_t length) = 0;
			virtual void Read(Byte* buffer, size_t length) = 0;
			virtual Byte ReadByte() = 0;
			/// <note>If $position touches the boundary, it will retreat to the boundary.</note>
			virtual void Seek(size_t distance, SeekOrigin position) = 0;
			/// <return>True if the capacity is changed and data is reallocated successfully. Otherwise, false, such as the capacity is fixed for it uses an external allocated buffer.</return>
			virtual bool SetCapacity(size_t capacity) = 0;
			/// <return>The number of bytes that were really written.</return>
			virtual size_t Write(Byte* data, size_t length) = 0;
			/// <return>The boolean value indicating if the byte was really written.</return>
			virtual bool WriteByte(Byte data) = 0;

		};
	}
}
#endif
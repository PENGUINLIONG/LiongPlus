// File: MemoryStream.cpp
// Author: Rendong Liang (Liong)
#include <cassert>
#include "MemoryStream.hpp"

namespace LiongPlus
{
	namespace IO
	{
		using std::swap;
		MemoryStream::MemoryStream()
			: Stream()
			, _Buffer(DEFAULT_BUFFER_CHUNK_SIZE)
			, _Position(0)
			, _IsClosed(false)
		{
		}
		MemoryStream::MemoryStream(MemoryStream&& instance)
			: Stream()
			, _Buffer()
			, _Position(0)
			, _IsClosed(true)
		{
			swap(_Buffer, instance._Buffer);
			swap(_Position, instance._Position);
			swap(_Permission, instance._Permission);
			swap(_IsClosed, instance._IsClosed);
		}
		MemoryStream::MemoryStream(StreamAccessPermission permission)
			: Stream(permission)
			, _Buffer(DEFAULT_BUFFER_CHUNK_SIZE)
			, _Position(0)
			, _IsClosed(false)
		{
		}
		MemoryStream::MemoryStream(Buffer&& buffer)
			: Stream()
			, _Buffer()
			, _Position(0)
			, _IsClosed(false)
		{
			swap(_Buffer, buffer);
		}
		MemoryStream::MemoryStream(Buffer&& buffer, StreamAccessPermission permission)
			: Stream(permission)
			, _Buffer()
			, _Position(0)
			, _IsClosed(false)
		{
			swap(_Buffer, buffer);
		}

		MemoryStream::~MemoryStream()
		{
			Close();
		}

		Buffer MemoryStream::ToBuffer()
		{
			Buffer rv(_Buffer.Length());
			memcpy(rv.Field(), _Buffer.Field(), _Buffer.Length());
			return rv;
		}

		bool MemoryStream::CanRead()
		{
			return _Permission != StreamAccessPermission::WriteOnly && _IsClosed;
		}

		bool MemoryStream::CanWrite()
		{
			return _Permission != StreamAccessPermission::ReadOnly && _IsClosed;
		}

		bool MemoryStream::CanSeek()
		{
			return _IsClosed;
		}

		void MemoryStream::Close()
		{
		}

		void MemoryStream::CopyTo(Stream& stream)
		{
			stream.Write(_Buffer.Field() + _Position, _Buffer.Length() - _Position);
		}

		void MemoryStream::CopyTo(Stream& stream, size_t length)
		{
			int available = _Buffer.Length() - _Position;
			stream.Write(_Buffer.Field() + _Position, length > available ? available : length);
		}

		void MemoryStream::Flush()
		{
		}

		size_t MemoryStream::Capacity()
		{
			return _Buffer.Length();
		}

		size_t MemoryStream::Length()
		{
			return _Buffer.Length();
		}


		size_t MemoryStream::Position()
		{
			return _Position;
		}

		bool MemoryStream::IsEndOfStream()
		{
			return _Buffer.Length() == _Position + 1;
		}

		Buffer MemoryStream::Read(size_t length)
		{
			assert(CanRead(), "Cannot read from this instance");

			Buffer buffer = Buffer(length);
			Read(buffer.Field(), length);
			return buffer;
		}

		void MemoryStream::Read(Byte* buffer, size_t length)
		{
			assert(CanRead(), "Cannot read from this instance");

			// If available data is less than which is requested, just copy the available part.
			size_t available = length > (_Buffer.Length() - _Position) ? (_Buffer.Length() - _Position) : length;
			memcpy(buffer, _Buffer.Field(), available);
		}

		Byte MemoryStream::ReadByte()
		{
			assert(CanRead(), "Cannot read from this instance");

			return _Buffer[_Position++];
		}

		void MemoryStream::Seek(size_t distance, SeekOrigin position)
		{
			assert(!CanSeek(), "Cannot seek in this instance");
			switch (position)
			{
			case SeekOrigin::Begin:
				_Position = 0;
				break;
			case SeekOrigin::Current:
				break;
			case SeekOrigin::End:
				_Position = _Buffer.Length() - 1;
				break;
			default:
				break;
			}
			_Position += distance;

			if (_Position < 0)
				_Position = 0;
			else if (_Position >= _Buffer.Length())
				_Position = _Buffer.Length() - 1;
		}

		bool MemoryStream::SetCapacity(size_t capacity)
		{
			if (capacity != _Buffer.Length()) // Need to reallocate.
			{
				Buffer newBuffer(capacity);
				memcpy(newBuffer.Field(), _Buffer.Field(), capacity > _Buffer.Length() ? _Buffer.Length() : capacity);
				_Buffer = std::move(newBuffer);
			}

			return true;
		}

		size_t MemoryStream::Write(Byte* data, size_t length)
		{
			assert(CanWrite(), "Cannot write to this instance");
			if (IsEndOfStream())
				return false;
			if (length > _Buffer.Length() - _Position)
				length = _Buffer.Length() - _Position;
			memcpy(_Buffer.Field() + _Position, data, length);
			return length;
		}

		bool MemoryStream::WriteByte(Byte data)
		{
			assert(CanWrite(), "Cannot write to this instance");
			if (IsEndOfStream())
				return false;

			_Buffer[_Position++] = data;
			return true;
		}
	}
}
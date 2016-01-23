// File: MemoryStream.cpp
// Author: Rendong Liang (Liong)
#include "MemoryStream.hpp"

namespace LiongPlus
{
	namespace IO
	{
		MemoryStream::MemoryStream()
			: _Size(DEFAULT_BUFFER_CHUNK_SIZE)
			, _Position(0)
			, _Buffer(new Byte[DEFAULT_BUFFER_CHUNK_SIZE])
			, _Permission(StreamAccessPermission::ReadWrite)
			, _ShouldDeleteBuffer(true)
		{
		}
		MemoryStream::MemoryStream(const MemoryStream& instance)
			: _Size(instance._Size)
			, _Position(instance._Position)
			, _Buffer(instance._Buffer)
			, _Permission(instance._Permission)
			, _ShouldDeleteBuffer(false)
		{
		}
		MemoryStream::MemoryStream(StreamAccessPermission permission)
			: _Size(DEFAULT_BUFFER_CHUNK_SIZE)
			, _Position(0)
			, _Buffer(new Byte[DEFAULT_BUFFER_CHUNK_SIZE])
			, _Permission(permission)
			, _ShouldDeleteBuffer((true))
		{
		}
		MemoryStream::MemoryStream(Byte* buffer, long length)
			: _Size(length)
			, _Position(0)
			, _Buffer(buffer)
			, _Permission(StreamAccessPermission::ReadWrite)
			, _ShouldDeleteBuffer(false)
		{
		}
		MemoryStream::MemoryStream(Byte* buffer, long length, StreamAccessPermission permission)
			: _Size(length)
			, _Position(0)
			, _Buffer(buffer)
			, _Permission(permission)
			, _ShouldDeleteBuffer(false)
		{
		}

		MemoryStream::~MemoryStream()
		{
			Close();
		}

		// IBuffer

		const Byte* MemoryStream::AbandonBuffer()
		{
			const Byte* ptr = _Buffer;
			_Buffer = new Byte[_Size];
			_Position = 0;
			_ShouldDeleteBuffer = true;
			return ptr;
		}

		const Byte* MemoryStream::AccessBuffer()
		{
			return _Buffer;
		}

		bool MemoryStream::IsBufferAccessable()
		{
			return true;
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
			if (!_IsClosed)
			{
				_IsClosed = true;

				if (_ShouldDeleteBuffer)
					delete[] _Buffer;
			}
		}

		void MemoryStream::CopyTo(Stream& stream)
		{
			stream.Write(_Buffer + _Position, _Size - _Position);
		}
		void MemoryStream::CopyTo(Stream& stream, long length)
		{
			long available = _Size - _Position;
			stream.Write(_Buffer + _Position, length > available ? available : length);
		}

		void MemoryStream::Flush()
		{
		}

		long MemoryStream::GetCapacity()
		{
			return _Size;
		}

		long MemoryStream::GetLength()
		{
			return _Size;
		}

		long MemoryStream::GetPosition()
		{
			return _Position;
		}

		bool MemoryStream::IsEndOfStream()
		{
			return _Size == _Position + 1;
		}

		Byte* MemoryStream::Read(long length)
		{
			assert(CanRead(), "Cannot read from this instance");

			Byte* buffer = new Byte[length];
			Read(buffer, length);
			return buffer;
		}

		void MemoryStream::Read(Byte* buffer, long length)
		{
			assert(CanRead(), "Cannot read from this instance");

			// If available data is less than which is requested, just copy the available part.
			long available = length > (_Size - _Position) ? (_Size - _Position) : length;
			memcpy(buffer, _Buffer, available);
		}

		Byte MemoryStream::ReadByte()
		{
			assert(CanRead(), "Cannot read from this instance");

			return _Buffer[_Position++];
		}

		void MemoryStream::Seek(long distance, SeekOrigin position)
		{
			assert(CanSeek(), "Cannot seek in this instance");
			switch (position)
			{
			case SeekOrigin::Begin:
				_Position = 0;
				break;
			case SeekOrigin::Current:
				break;
			case SeekOrigin::End:
				_Position = _Size - 1;
				break;
			default:
				break;
			}
			_Position += distance;

			if (_Position < 0)
				_Position = 0;
			else if (_Position >= _Size)
				_Position = _Size - 1;
		}

		bool MemoryStream::SetCapacity(long capacity)
		{
			if (!_ShouldDeleteBuffer)
				return false;

			if (capacity > _Size) // Need to reallocate.
			{
				Byte* newBuffer = new Byte[capacity];
				memcpy(newBuffer, _Buffer, _Size);
			}
			_Size = capacity; // Reset capacity.

			return true;
		}

		long MemoryStream::Write(Byte* data, long length)
		{
			assert(CanWrite(), "Cannot write to this instance");
			if (IsEndOfStream())
				return false;
			if (length > _Size - _Position)
				length = _Size - _Position;
			memcpy(_Buffer + _Position, data, length);
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
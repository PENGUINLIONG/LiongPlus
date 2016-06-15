// File: MemoryStream.hpp
// Author: Rendong Liang (Liong)
#include "Fundamental.hpp"
#include "Stream.hpp"
#include "../Buffer.hpp"

#ifndef MemoryStream_hpp
#define MemoryStream_hpp

namespace LiongPlus
{
	namespace IO
	{
		class  MemoryStream
			: public Stream
		{
		private:
			static const long DEFAULT_BUFFER_CHUNK_SIZE = 4096;

			size_t _Position;
			Buffer _Buffer;
			StreamAccessPermission _Permission;
			bool _IsClosed;
		public:
			MemoryStream();
			MemoryStream(MemoryStream& instance) = delete;
			MemoryStream(MemoryStream&& instance);
			MemoryStream(StreamAccessPermission permission);
<<<<<<< HEAD
			MemoryStream(Buffer&& buffer);
			MemoryStream(Buffer&& buffer, StreamAccessPermission permission);
=======
			MemoryStream(Byte* buffer, long length);
			MemoryStream(Byte* buffer, long length, StreamAccessPermission permission);
>>>>>>> master
			~MemoryStream();

			MemoryStream& operator=(MemoryStream&& instance);

<<<<<<< HEAD
			Buffer ToBuffer();

			// Stream

			virtual bool CanRead() override;
			virtual bool CanWrite() override;
			virtual bool CanSeek() override;
			virtual void Close() override;
			virtual void CopyTo(Stream& stream) override;
			virtual void CopyTo(Stream& stream, size_t length) override;
			virtual void Flush() override;
			virtual size_t Capacity() override;
			virtual size_t Length() override;
			virtual size_t Position() override;
			virtual bool IsEndOfStream() override;
			virtual Buffer Read(size_t length) override;
			virtual void Read(Byte* buffer, size_t length) override;
			virtual Byte ReadByte() override;
			virtual void Seek(size_t distance, SeekOrigin position) override;
			virtual bool SetCapacity(size_t capacity) override;
			virtual size_t Write(Byte* data, size_t length) override;
			virtual bool WriteByte(Byte data) override;
=======
			virtual Byte* AbandonBuffer();
			virtual const Byte* AccessBuffer();
			virtual bool IsBufferAccessable();

			// Stream

			virtual bool CanRead();
			virtual bool CanWrite();
			virtual bool CanSeek();
			virtual void Close();
			virtual void CopyTo(Stream& stream);
			virtual void CopyTo(Stream& stream, long length);
			virtual void Flush();
			virtual long GetCapacity();
			virtual long GetLength();
			virtual long GetPosition();
			virtual bool IsEndOfStream();
			virtual Byte* Read(long length);
			virtual void Read(Byte* buffer, long length);
			virtual Byte ReadByte();
			virtual void Seek(long distance, SeekOrigin position);
			virtual bool SetCapacity(long capacity);
			virtual long Write(Byte* data, long length);
			virtual bool WriteByte(Byte data);
		private:
			const static long DEFAULT_BUFFER_CHUNK_SIZE = 4096;

			long _Size;
			long _Position;
			Byte* _Buffer;
			StreamAccessPermission _Permission;
			bool _ShouldDeleteBuffer;
			bool _IsClosed;
>>>>>>> master
		};
	}
}
#endif /* MemoryStream_hpp */

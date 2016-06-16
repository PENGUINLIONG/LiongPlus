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
			MemoryStream(Buffer&& buffer);
			MemoryStream(Buffer&& buffer, StreamAccessPermission permission);
			~MemoryStream();

			MemoryStream& operator=(MemoryStream&& instance);

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
		};
	}
}
#endif /* MemoryStream_hpp */

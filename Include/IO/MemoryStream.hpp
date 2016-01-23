// File: MemoryStream.hpp
// Author: Rendong Liang (Liong)
#include "Fundamental.hpp"
#include "Exception.hpp"
#include "IBuffer.hpp"
#include "Stream.hpp"

#ifndef MemoryStream_hpp
#define MemoryStream_hpp

namespace LiongPlus
{
	namespace IO
	{
		class  MemoryStream
			: public Object
			, public IBuffer
		{
		public:
			MemoryStream();
			MemoryStream(const MemoryStream& stream);
			MemoryStream(StreamAccessPermission permission);
			MemoryStream(Byte* buffer, long length);
			MemoryStream(Byte* buffer, long length, StreamAccessPermission permission);
			~MemoryStream();

			// IBuffer

			virtual const Byte* AbandonBuffer();
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
		};
	}
}
#endif /* MemoryStream_hpp */

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
			MemoryStream(Byte* buffer, int length);
			MemoryStream(Byte* buffer, int length, StreamAccessPermission permission);
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
			virtual void CopyTo(Stream& stream, int length);
			virtual void Flush();
			virtual int GetCapacity();
			virtual int GetLength();
			virtual int GetPosition();
			virtual bool IsEndOfStream();
			virtual Byte* Read(int length);
			virtual void Read(Byte* buffer, int length);
			virtual Byte ReadByte();
			virtual void Seek(int distance, SeekOrigin position);
			virtual bool SetCapacity(int capacity);
			virtual int Write(Byte* data, int length);
			virtual bool WriteByte(Byte data);
		private:
			const static int DEFAULT_BUFFER_CHUNK_SIZE = 4096;

			int _Size;
			int _Position;
			Byte* _Buffer;
			StreamAccessPermission _Permission;
			bool _ShouldDeleteBuffer;
			bool _IsClosed;
		};
	}
}
#endif /* MemoryStream_hpp */

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
		{
		public:
			MemoryStream();
			MemoryStream(const MemoryStream& stream);
			MemoryStream(StreamAccessPermission permission);
			MemoryStream(unsigned char* buffer, long length);
			MemoryStream(unsigned char* buffer, long length, StreamAccessPermission permission);
			~MemoryStream();

			unsigned char* ToArray();

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
			virtual unsigned char* Read(long length);
			virtual void Read(unsigned char* buffer, long length);
			virtual unsigned char ReadByte();
			virtual void Seek(long distance, SeekOrigin position);
			virtual bool SetCapacity(long capacity);
			virtual long Write(unsigned char* data, long length);
			virtual bool WriteByte(unsigned char data);
		private:
			static const long DEFAULT_BUFFER_CHUNK_SIZE = 4096;

			long _Size;
			long _Position;
			unsigned char* _Buffer;
			StreamAccessPermission _Permission;
			bool _ShouldDeleteBuffer;
			bool _IsClosed;
		};
	}
}
#endif /* MemoryStream_hpp */

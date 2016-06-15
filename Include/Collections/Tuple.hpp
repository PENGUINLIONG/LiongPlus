// File: Tuple.hpp
// Author: Rendong Liang (Liong)
#ifndef _L_Tuple
#define _L_Tuple
#include "../Fundamental.hpp"
#include "../Buffer.hpp"

namespace LiongPlus
{
	namespace Collections
	{
		template<typename ... Ts>
		class Tuple
			: public Object
		{
		public:
			using TSelf = Tuple<Ts ...>;

			Tuple()
			{
				_Anchors[0] = new Byte[GetDataLength()];
				Byte** pos = reinterpret_cast<Byte**>(_Anchors);
				Byte* fieldPos = reinterpret_cast<Byte*>(_Anchors[0]);
				long i[] = { 0, (*(pos++) = fieldPos, fieldPos += sizeof(Ts), *(reinterpret_cast<Ts*>(fieldPos)) = args, 0)... };
			}
			Tuple(const Ts& ... args)
			{
				_Anchors[0] = new Byte[GetDataLength()];
				Byte** pos = reinterpret_cast<Byte**>(_Anchors);
				Byte* fieldPos = reinterpret_cast<Byte*>(_Anchors[0]);
				long i[] = { 0, (*(pos++) = fieldPos, *(reinterpret_cast<Ts*>(fieldPos)) = args, fieldPos += sizeof(Ts), 0)... };
			}
			Tuple(const TSelf& instance)
			{
				Buffer::Memcpy(_Anchors[0], instance._Anchors[0], GetDataLength());
			}
			Tuple(TSelf&& instance)
			{
				Buffer::Memxchg(_Anchors[0], instance._Anchors[0], GetDataLength());
			}
			~Tuple()
			{
				delete [] (Byte*)(_Anchors[0]);
			}

			TSelf operator=(const TSelf& instance)
			{
				Buffer::Memcpy(_Anchors[0], instance._Anchors[0], GetDataLength());
				return *this;
			}
			TSelf operator=(TSelf&& instance)
			{
				Buffer::Memxchg(_Anchors[0], instance._Anchors[0], GetDataLength());
				return *this;
			}
			bool operator=(TSelf& value)
			{
				return std::memcmp(_Anchors[0], value._Anchors[0], GetDataLength()) == 0;
			}

			long GetDataLength() const
			{
				long len = 0;
				long i[] = { 0, (len += sizeof(Ts), 0)... };
				return len;
			}

			template<typename T>
			bool IsCorrectType(long index)
			{
				bool flag = false;
				long pos = 0;
				long i[] = { 0, (flag = flag || (pos++ == index ? typeid(T) == typeid(Ts) : false), 0)... };
				return flag;
			}

			template<typename T>
			T& GetValue(long index)
			{
				assert(index < sizeof...(Ts), "$index");
				assert(IsCorrectType<T>(index), "$index");
				
				return *reinterpret_cast<T*>(_Anchors[index]);
			}
			
			void* _Anchors[sizeof...(Ts)];
		};
	}
}
#endif
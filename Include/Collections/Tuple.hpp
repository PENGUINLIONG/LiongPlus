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
		template<typename T>
		struct TupleElement
		{
		public:
			T Value;

			TupleElement() = default;
			TupleElement(const T& value)
				: Value(value)
			{
			}
		};

		template<typename ... Ts>
		class Tuple
			: public Object
		{
		public:
			using TSelf = Tuple<Ts ...>;

			Tuple()
			{
				void** pos = _Values;
				long i[] = { 0, (*(pos++) = new TupleElement<Ts>, 0)... };
			}
			Tuple(const Ts& ... args)
			{
				void** pos = _Values;
				long i[] = { 0, (*(pos++) = new TupleElement<Ts>(args), 0)... };
			}
			Tuple(const TSelf& instance) = delete;
			Tuple(TSelf&& instance) = delete;
			~Tuple()
			{
			}

			TSelf operator=(const TSelf& instance) = delete;
			TSelf operator=(TSelf&& instance) = delete;

			template<typename T>
			bool isCorrectType(long index)
			{
				bool flag = false;
				long pos = 0;
				long i[] = { 0, (flag = flag || (pos++ == index ? typeid(T) == typeid(Ts) : false), 0)... };
				return flag;
			}

			template<typename T>
			T& GetValue(long index)
			{
				assert(index < index, "$index");
#ifdef _L_DEBUG
				assert(isCorrectType<T>(), "$index");
#endif
				
				return reinterpret_cast<TupleElement<T>*>(_Value[index])->Value;
			}
			
			void* _Values[sizeof...(Ts)];
		};
	}
}
#endif
// File: HttpBufferPool.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "../Fundamental.hpp"
#include "../Buffer.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;
		
		typedef size_t HttpBufferToken;

		class HttpBufferPool
		{
		private:
			struct Unit
			{
			public:
				Unit();
				Unit(const Unit&);
				Unit(Unit&&);
				~Unit();

				Buffer* Ptr;
				bool IsOccupied;
				mutex Lock;
			};
			static vector<Unit> _Pool;
		public:
			static HttpBufferToken Apply();
			static Buffer& Fetch(HttpBufferToken token);
			static void Release(HttpBufferToken token);
		};
	}
}

// File: ISerializable.hpp
// Author: Rendong Liang (Liong)

#ifndef ISerializable_hpp
#define ISerializable_hpp
#include "Fundamental.hpp"

namespace LiongPlus
{
	namespace Serialization
	{
		/// <summary>
		/// This interface should be implemented by classes which
		/// </summary>
		/// <notice>For every class that implements this interface, there should be a static method to deserialize data into instance.</notice>
		template <typename T>
		class ISerializable
		{
		public:
			/*
			 * [warning] You should delete the pointer when you will not use it anymore.
			 */
			virtual Array<Byte> Serialize() = 0;
		};
	}
}
#endif /* ISerializable_hpp */
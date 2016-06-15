// File: Image.hpp
// Author: Rendong Liang (Liong)

#ifndef Image_hpp
#define Image_hpp

#include "../Fundamental.hpp"
#include "../IO/MemoryStream.hpp"

using namespace LiongPlus::IO;

namespace LiongPlus
{
	namespace Media
	{
		enum PixelType
		{
			Alpha = 0,
			Red = 1,
			Green = 2,
			Blue = 3,
			Rgb = 4,
			Bgr = 5,
			Rgba = 6
		};

		class Image
		{
		public:
			/*
			 * Retrieve a chunk of pixels in the image.
			 * [return] A pointer to a newly allocated buffer which stores sequence of pixels.
			 * [warning] You should delete the pointer when you will not use it anymore.
			 * [note] You should not use this method to retrieve a chunk of pixels. Alternatively, use [intFramework::IO::Image::GetChunk] instead.
			 */
<<<<<<< HEAD
			virtual Buffer GetChunk(Point position, Size size) const = 0;
			virtual size_t GetInterpretedLength(PixelType pixelType) const = 0;
=======
			virtual Byte* GetChunk(Point position, Size size) const = 0;
			virtual long GetInterpretedLength(PixelType pixelType) const = 0;
>>>>>>> master
			/*
			 * Retrieve a single pixel in the image.
			 * [return] A pointer to a newly allocated buffer which stores a single pixel.
			 * [warning] You should delete the pointer when you will not use it anymore.
			 * [note] You should not use this method to retrieve a chunk of pixels. Alternatively, use [intFramework::IO::Image::GetChunk] instead.
			 */
			virtual Buffer GetPixel(Point position) const = 0;
			virtual Size GetSize() const = 0;
			virtual PixelType GetPixelType() const = 0;
			virtual bool IsEmpty() const = 0;
			/*
			 * Interpret data to Bitmap of a specific pixel type and save the result into a newly allocated buffer.
			 * [return] A pointer to a newly allocated which stores sequence of pixels of the same specified type.
			 * [warning] You should delete the pointer when you will not use it anymore.
			 */
			virtual Buffer Interpret(PixelType pixelType) const = 0;

			// Static

			static Image* FromMemory(MemoryStream stream);
		};
	}
}

#endif /* Image_hpp */

// File: Bitmap.hpp
// Author: Rendong Liang (Liong)

#ifndef Bitmap_hpp
#define Bitmap_hpp
#include "Fundamental.hpp"
#include "Buffer.hpp"
#include "Image.hpp"

using namespace LiongPlus;

namespace LiongPlus
{
	namespace Media
	{
		/*
		 * [note] This class is a simple storage of pixel sequence so it is not able to analyze the file format whose extension is '.bmp'. To do that, please use [intFramework::Media::Bmp] instead.
		 */
		class Bitmap
			: public Image
		{
		public:
			Bitmap(const Image& image);
			Bitmap(const Bitmap& image);
			Bitmap(Bitmap&& image);
			Bitmap(Buffer&& buffer, Size size, PixelType pixelType);
			virtual ~Bitmap();

			Bitmap& operator=(const Bitmap& instance);
			Bitmap& operator=(Bitmap&& instance);

			// Static

			/*
			 * Create a new instance of [intStudio::Media::Bitmap].
			 * [warning] You should delete the pointer when you will not use it anymore.
			 */
			static Image* FromMemory(MemoryStream& stream, Size size, PixelType pixelType);

			// Derived from [LiongPlus::Media::Image]

			virtual Buffer GetChunk(Point position, Size size) const override;
			virtual size_t GetInterpretedLength(PixelType pixelType) const override;
			virtual Buffer GetPixel(Point position) const override;
			virtual PixelType GetPixelType() const override;
			virtual Size GetSize() const override;
			virtual bool IsEmpty() const override;
			virtual Buffer Interpret(PixelType pixelType) const override;

		private:
			Buffer _Buffer;
			PixelType _PixelType;
			Size _Size;

			Buffer InterpretMonoTo(PixelType pixelType) const;
			Buffer InterpretTriTo(PixelType pixelType) const;
			Buffer InterpretQuadTo(PixelType pixelType) const;

			Buffer InterpretMonoToTri(size_t factorOffset) const;
			Buffer InterpretMonoToQuad(size_t factorOffset) const;

			Buffer InterpretTriToMono(size_t factorOffset) const;
			Buffer InterpretTriToTri() const;
			Buffer InterpretTriToQuad(bool shouldInverse) const;

			Buffer InterpretQuadToMono(size_t factorOffset) const;
			Buffer InterpretQuadToTri(bool shouldInverse) const;

			// Static

			static size_t CalculatePixelLength(PixelType pixelType);
			static size_t CalculateDataLength(Size size, PixelType pixelType);
		};
	}
}
#endif /* Bitmap_hpp */

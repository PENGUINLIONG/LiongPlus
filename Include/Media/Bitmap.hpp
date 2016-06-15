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

<<<<<<< HEAD
			virtual Buffer GetChunk(Point position, Size size) const override;
			virtual size_t GetInterpretedLength(PixelType pixelType) const override;
			virtual Buffer GetPixel(Point position) const override;
=======
			virtual Byte* GetChunk(Point position, Size size) const override;
			virtual long GetInterpretedLength(PixelType pixelType) const override;
			virtual Byte* GetPixel(Point position) const override;
>>>>>>> master
			virtual PixelType GetPixelType() const override;
			virtual Size GetSize() const override;
			virtual bool IsEmpty() const override;
			virtual Buffer Interpret(PixelType pixelType) const override;

<<<<<<< HEAD
		private:
			Buffer _Buffer;
=======
			// Derived from [intFramework::Serialization::ISerializable<Bitmap>]

			// [note] Image size information will be converted to ints, so they may become invalid if the number of height or height is too large.

			// [architecture] The serialized form of this type of image is very simple as there is few information should be stored.
			// [4] Width of image(long)
			// [4] Height of image(long)
			// [4] PixelType(long)
			// [~] Pixels

			virtual Array<Byte> Serialize() override;

			Ptr<Bitmap> Bitmap::Deserialize(Array<Byte>& arr);
		private:
			Byte* _Data;
			long _Length;
>>>>>>> master
			PixelType _PixelType;
			Size _Size;

			Buffer InterpretMonoTo(PixelType pixelType) const;
			Buffer InterpretTriTo(PixelType pixelType) const;
			Buffer InterpretQuadTo(PixelType pixelType) const;

<<<<<<< HEAD
			Buffer InterpretMonoToTri(int factorOffset) const;
			Buffer InterpretMonoToQuad(int factorOffset) const;

			Buffer InterpretTriToMono(int factorOffset) const;
			Buffer InterpretTriToTri() const;
			Buffer InterpretTriToQuad(bool shouldInverse) const;

			Buffer InterpretQuadToMono(int factorOffset) const;
			Buffer InterpretQuadToTri(bool shouldInverse) const;

			// Static

			static size_t CalculatePixelLength(PixelType pixelType);
			static size_t CalculateDataLength(Size size, PixelType pixelType);
=======
			Byte* InterpretMonoToTri(long factorOffset) const;
			Byte* InterpretMonoToQuad(long factorOffset) const;

			Byte* InterpretTriToMono(long factorOffset) const;
			Byte* InterpretTriToTri() const;
			Byte* InterpretTriToQuad(bool shouldInverse) const;

			Byte* InterpretQuadToMono(long factorOffset) const;
			Byte* InterpretQuadToTri(bool shouldInverse) const;

			// Static

			static long CalculatePixelLength(PixelType pixelType);
			static long CalculateDataLength(Size size, PixelType pixelType);
>>>>>>> master
		};
	}
}
#endif /* Bitmap_hpp */

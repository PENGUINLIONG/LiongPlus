// File: Bitmap.hpp
// Author: Rendong Liang (Liong)

#ifndef Bitmap_hpp
#define Bitmap_hpp
#include "Fundamental.hpp"
#include "Buffer.hpp"
#include "Image.hpp"
#include "Serialization/ISerializable.hpp"

using namespace LiongPlus;
using namespace LiongPlus::Serialization;

namespace LiongPlus
{
	namespace Media
	{
		/*
		 * [note] This class is a simple storage of pixel sequence so it is not able to analyze the file format whose extension is '.bmp'. To do that, please use [intFramework::Media::Bmp] instead.
		 */
		class Bitmap
			: public Image
			, public ISerializable<Bitmap>
		{
		public:
			Bitmap(const Image& image);
			Bitmap(const Bitmap& image);
			Bitmap(Bitmap&& image);
			Bitmap(Byte* buffer, Size size, PixelType pixelType, bool shouldCopy = true, bool shouldDelete = true);
			virtual ~Bitmap();

			Bitmap& operator=(const Bitmap& instance);
			Bitmap& operator=(Bitmap&& instance);

			// Static

			/*
			 * Create a new instance of [intStudio::Media::Bitmap].
			 * [warning] You should delete the pointer when you will not use it anymore.
			 */
			static Image* FromMemory(MemoryStream& stream, Size size, PixelType pixelType);

			// Derived from [intFramework::Media::Image]

			virtual Byte* GetChunk(Point position, Size size) const override;
			virtual int GetInterpretedLength(PixelType pixelType) const override;
			virtual Byte* GetPixel(Point position) const override;
			virtual PixelType GetPixelType() const override;
			virtual Size GetSize() const override;
			virtual bool IsEmpty() const override;
			virtual Byte* Interpret(PixelType pixelType) const override;

			// Derived from [intFramework::Serialization::ISerializable<Bitmap>]

			// [note] Image size information will be converted to ints, so they may become invalid if the number of height or height is too large.

			// [architecture] The serialized form of this type of image is very simple as there is few information should be stored.
			// [4] Width of image(int)
			// [4] Height of image(int)
			// [4] PixelType(int)
			// [~] Pixels

			virtual Array<Byte> Serialize() override;

			Ptr<Bitmap> Bitmap::Deserialize(Array<Byte>& arr);
		private:
			Byte* _Data;
			int _Length;
			PixelType _PixelType;
			Size _Size;

			Byte* InterpretMonoTo(PixelType pixelType) const;
			Byte* InterpretTriTo(PixelType pixelType) const;
			Byte* InterpretQuadTo(PixelType pixelType) const;

			Byte* InterpretMonoToTri(int factorOffset) const;
			Byte* InterpretMonoToQuad(int factorOffset) const;

			Byte* InterpretTriToMono(int factorOffset) const;
			Byte* InterpretTriToTri() const;
			Byte* InterpretTriToQuad(bool shouldInverse) const;

			Byte* InterpretQuadToMono(int factorOffset) const;
			Byte* InterpretQuadToTri(bool shouldInverse) const;

			// Static

			static int CalculatePixelLength(PixelType pixelType);
			static int CalculateDataLength(Size size, PixelType pixelType);
		};
	}
}
#endif /* Bitmap_hpp */

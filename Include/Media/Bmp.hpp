// File: Bmp.hpp
// Author: Rendong Liang (Liong)
#ifndef Bmp_hpp
#define Bmp_hpp

#include "../Fundamental.hpp"
#include "Bitmap.hpp"
#include "Image.hpp"
#include "Exception.hpp"
#include "../Serialization/ISerializable.hpp"

using namespace LiongPlus::Serialization;

namespace LiongPlus
{
	namespace Media
	{
		/*
		 * [note] This class does not support bmp files whose pixels are not of 24-bit in size.
		 * [architecture] The structure of BitMaP File (Used section will be marked as $):
		 *
		 * ---- File header ----
		 * 0x00 $ [2] Type code of the bmp file. Usually "BM"
		 * 0x02   [4] Size of the bmp file (in bytes).
		 * 0x06   [2] Useless.
		 * 0x08   [2] Useless.
		 * 0x0C $ [4] Offset to the pixel data.
		 * ---- File header ----
		 * ---- DIB (info) header ----
		 * 0x00   [4] Size of this header.
		 * 0x04 $ [4 unsigned] Width of the image.
		 * 0x08 $ [4 unsigned] Height of the image.
		 * 0x0A   [2] Number of color planes. Always 1.
		 * 0x0C $ [2] Bits per pixel.
		 * 0x10   [4] Type of compression. For 24-bit it should be 0.
		 * 0x14   [4] Image size. Can be 0 when it uses 24-bit pixels.
		 * 0x18   [4 unsigned] Number of pixels per meter. (Horizontal)
		 * 0x1C   [4 unsigned] Number of pixels per meter. (Veritical)
		 * 0x20   [4] Number of colors used in palette. For 24-bit it should be 0.
		 * 0x24   [4] Number of important colors used in palette. For 24-bit it should be 0.
		 * ---- DIB (info) header ----
		 */
		class Bmp
			: public Image
			, public ISerializable<Bmp>
		{
		public:
			Bmp(Image& instance);
			Bmp(Byte* buffer, long length, bool shouldDelete = true);
			~Bmp();

			// Derived from [LiongPlus::Media::Image]

			virtual Byte* GetChunk(Point position, Size size) const override;
			virtual long GetInterpretedLength(PixelType pixelType) const override;
			virtual Byte* GetPixel(Point position) const override;
			virtual Size GetSize() const override;
			virtual PixelType GetPixelType() const override;
			virtual bool IsEmpty() const override;
			virtual Byte* Interpret(PixelType pixelType) const override;

			// TextureRef ToTexture(_L_Char *path, Flag option = FileReadOption::None);

			// Derived from [LiongPlus::Serialization::ISerializable]

			virtual Array<Byte> Serialize() override;
		private:
			Bitmap _Bitmap;

			Bitmap Init(Byte* buffer, long length, bool shouldDelete);
		};
	}
}
#endif
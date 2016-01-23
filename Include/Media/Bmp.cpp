// File: Bmp.cpp
// Author: Rendong Liang (Liong)

#include "Media/Bmp.hpp"

namespace LiongPlus
{
	namespace Media
	{
		// Public

		Bmp::Bmp(Image& instance)
			: _Bitmap(instance.Interpret(PixelType::Bgr), instance.GetSize(), instance.GetPixelType(), false)
		{
		}
		Bmp::Bmp(Byte* buffer, long length, bool shouldDelete)
			: _Bitmap(Init(buffer, length, shouldDelete))
		{
		}
		Bmp::~Bmp()
		{
		}

		/*    TextureRef Bmp::ToTexture(_L_Char *path, Flag option)
			{
				Log << L"Bmp: Try loading " << path << L"...";
				if (stream.is_open())
					stream.close();
				stream.open(path, stream.in | stream.binary | stream._Nocreate);

				TextureRef ref;

				Size size;
				long dataLength;
				if (InitHeader(size, dataLength))
				{
					Byte* data = ReadData(dataLength);
					if (!data)
						return TextureRef();

					ref = TextureManager.NewTexture(dataLength, data, size, Texture::PixelFormat::BGR, Texture::ByteSize::UByte);
					if ((option & FileReadOption::NoGenerate) == FileReadOption::None)
						ref.lock()->Generate();
				}
				else
				{
					Log.Log((L"Bmp: Failed in loading " + wstring(path) + L"!").c_str(), Logger::WarningLevel::Warn);
					return TextureRef();
				}

				if ((option & FileReadOption::NoClose) == FileReadOption::None)
					stream.close();
				Log << L"Bmp: Succeeded!";
				return ref;
			}*/

			// Derived from [intFramework::Media::Image]

		Byte* Bmp::GetChunk(Point position, Size size) const
		{
			return _Bitmap.GetChunk(position, size);
		}

		long Bmp::GetInterpretedLength(PixelType pixelType) const
		{
			return _Bitmap.GetInterpretedLength(pixelType);
		}

		Byte* Bmp::GetPixel(Point position) const
		{
			return _Bitmap.GetPixel(position);
		}

		PixelType Bmp::GetPixelType() const
		{
			return _Bitmap.GetPixelType();
		}

		Size Bmp::GetSize() const
		{
			return _Bitmap.GetSize();
		}

		bool Bmp::IsEmpty() const
		{
			return _Bitmap.IsEmpty();
		}

		Byte* Bmp::Interpret(PixelType pixelType) const
		{
			return _Bitmap.Interpret(pixelType);
		}

		// Derived from [LiongPlus::Serialization::ISerializable]

		Array<Byte> Bmp::Serialize()
		{
			return Array<Byte>(1);
		}

		// Private

		Bitmap Bmp::Init(Byte* buffer, long length, bool shouldDelete)
		{
			assert(length > 54, "Incomplete header.");
			assert(buffer[0] == 'B' && buffer[1] == 'M', "Unsupported bmp format.");
			assert(*((unsigned short*)(buffer + 28)) == 24, "Unsupported pixel type."); // Bits per pixel.

			unsigned long offset = *((unsigned long*)(buffer + 10));
			Size size = { *((long*)(buffer + 18)), *((long*)(buffer + 22)) };

			return Bitmap(buffer + offset, size, PixelType::Bgr);

			if (shouldDelete)
				delete[] buffer;
		}
	}
}
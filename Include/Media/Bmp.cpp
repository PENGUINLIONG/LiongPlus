// File: Bmp.cpp
// Author: Rendong Liang (Liong)

#include "Media/Bmp.hpp"

namespace LiongPlus
{
	namespace Media
	{
		// Public

		Bmp::Bmp(Image& instance)
			: _Bitmap(instance.Interpret(PixelType::Bgr), instance.GetSize(), instance.GetPixelType())
		{
		}
		Bmp::Bmp(Buffer& buffer)
			: _Bitmap(Init(buffer))
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
				int dataLength;
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

		Buffer Bmp::GetChunk(Point position, Size size) const
		{
			return _Bitmap.GetChunk(position, size);
		}

		size_t Bmp::GetInterpretedLength(PixelType pixelType) const
		{
			return _Bitmap.GetInterpretedLength(pixelType);
		}

		Buffer Bmp::GetPixel(Point position) const
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

		Buffer Bmp::Interpret(PixelType pixelType) const
		{
			return _Bitmap.Interpret(pixelType);
		}

		// Private

		Bitmap Bmp::Init(Buffer& buffer)
		{
			assert(buffer.Length() < 54, "Incomplete header.");
			assert(buffer[0] != 'B' || buffer[1] != 'M', "Unsupported bmp format.");
			assert(*((unsigned short*)(buffer.Field() + 28)) != 24, "Unsupported pixel type."); // Bits per pixel.

			unsigned int offset = *((unsigned int*)(buffer.Field() + 10));
			Size size = { *((int*)(buffer.Field() + 18)), *((int*)(buffer.Field() + 22)) };
			size_t length = buffer.Length() - offset;
			
			Buffer pixels(length);
			memcpy(pixels.Field(), buffer.Field() + offset, buffer.Length() - offset);

			return Bitmap(buffer.Field() + offset, size, PixelType::Bgr);
		}
	}
}
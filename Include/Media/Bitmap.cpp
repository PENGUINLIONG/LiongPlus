// File: Bitmap.cpp
// Author: Rendong Liang (Liong)

#include "../Media/Bitmap.hpp"

namespace LiongPlus
{
	namespace Media
	{
		// Public

		Bitmap::Bitmap(const Image& image)
			: _Data(image.Interpret(image.GetPixelType()))
			, _Length(image.GetInterpretedLength(image.GetPixelType()))
			, _PixelType(image.GetPixelType())
			, _Size(image.GetSize())
		{
		}
		Bitmap::Bitmap(const Bitmap& instance)
			: _Data(instance._Data)
			, _Length(instance._Length)
			, _PixelType(instance._PixelType)
			, _Size(instance._Size)
		{
		}
		Bitmap::Bitmap(Bitmap&& instance)
			: _Data(nullptr)
			, _Length(0)
			, _PixelType(PixelType::Rgba)
			, _Size({ 0, 0 })
		{
			Swap(_Size, instance._Size);
			Swap(_PixelType, instance._PixelType);
			Swap(_Length, instance._Length);
			Swap(_Data, instance._Data);
		}
		Bitmap::Bitmap(Byte* buffer, Size size, PixelType pixelType, bool shouldCopy, bool shouldDelete)
			: _Data(shouldCopy ? new Byte[CalculateDataLength(size, pixelType)] : buffer)
			, _Length(CalculateDataLength(size, pixelType))
			, _PixelType(pixelType)
			, _Size(size)
		{
			if (shouldCopy)
				Buffer::Memcpy(_Data, buffer, _Length);
			if (shouldDelete)
				delete[] buffer;
		}
		Bitmap::~Bitmap()
		{
			delete[] _Data;
		}

		Bitmap& Bitmap::operator=(const Bitmap& instance)
		{
			_Size = instance.GetSize();
			_PixelType = instance.GetPixelType();
			_Length = CalculateDataLength(_Size, _PixelType);
			_Data = new Byte[_Length];
			return *this;
		}
		Bitmap& Bitmap::operator=(Bitmap&& instance)
		{
			Swap(_Size, instance._Size);
			Swap(_PixelType, instance._PixelType);
			Swap(_Length, instance._Length);
			Swap(_Data, instance._Data);
			return *this;
		}

		// Static

		Image* Bitmap::FromMemory(MemoryStream& stream, Size size, PixelType pixelType)
		{
			int length = CalculateDataLength(size, pixelType);
			Byte* buffer = stream.Read(length);

			return new Bitmap(buffer, size, pixelType);
		}

		// Derived from [LiongFramework::Media::Image]

		Byte* Bitmap::GetChunk(Point position, Size size) const
		{
			if (position.X < 0 || position.X + size.Width > _Size.Width ||
				position.Y < 0 || position.Y + size.Height > _Size.Height)
				return nullptr;

			Byte* buffer = new Byte[CalculateDataLength(size, _PixelType)];

			int pixelLength = CalculatePixelLength(_PixelType);
			int lineData = size.Width * pixelLength;
			int lineOffset = (_Size.Width - size.Width) * pixelLength;
			Byte* pos = _Data + // Origin
				(position.X + position.Y * _Size.Width) * pixelLength; // Offset

			while (size.Height-- > 0)
			{
				memcpy(buffer, pos, lineData); // Copy linear pixels in the same row.
				pos += lineOffset;
			}

			return buffer;
		}

		int Bitmap::GetInterpretedLength(PixelType pixelType) const
		{
			return CalculateDataLength(_Size, pixelType);
		}

		Byte* Bitmap::GetPixel(Point position) const
		{
			int pixelLength = CalculatePixelLength(_PixelType);
			Byte* pixel = new Byte[pixelLength];
			memcpy(pixel, _Data + position.Y * _Size.Width + position.X, pixelLength);

			return pixel;
		}

		PixelType Bitmap::GetPixelType() const
		{
			return _PixelType;
		}

		Size Bitmap::GetSize() const
		{
			return _Size;
		}

		bool Bitmap::IsEmpty() const
		{
			return _Size.Width == 0 || _Size.Height == 0;
		}

		Byte* Bitmap::Interpret(PixelType pixelType) const
		{
			if (pixelType == _PixelType)
			{
				Byte* buffer = new Byte[_Length];
				memcpy(buffer, _Data, _Length);
				return buffer;
			}

			switch (CalculatePixelLength(_PixelType))
			{
			case 1:
				return InterpretMonoTo(pixelType);
			case 3:
				return InterpretTriTo(pixelType);
			case 4:
				return InterpretQuadTo(pixelType);
			}
			return nullptr;
		}

		// Private

		Byte* Bitmap::InterpretMonoTo(PixelType pixelType) const
		{
			if (pixelType < 4) return nullptr; // Mono
			else if (pixelType == PixelType::Rgba) // Quad
				return InterpretMonoToQuad((int)pixelType);
			else // Tri
				return InterpretMonoToTri(pixelType == PixelType::Rgb
					? (int)pixelType - 1
					: 3 - (int)pixelType);
		}

		Byte* Bitmap::InterpretTriTo(PixelType pixelType) const
		{
			if (pixelType == PixelType::Rgba) // Quad
				return InterpretTriToQuad((pixelType != 4));
			else if ((int)pixelType < 4 && pixelType != PixelType::Alpha) // Mono
				return InterpretTriToMono(_PixelType == PixelType::Rgb);
			else // Tri
				return InterpretTriToTri();
		}

		Byte* Bitmap::InterpretQuadTo(PixelType pixelType) const
		{
			if (pixelType < 4) // Mono
				return InterpretQuadToMono((int)pixelType);
			else // Tri
				return InterpretQuadToTri((pixelType != 4));
		}

		Byte* Bitmap::InterpretMonoToTri(int factorOffset) const
		{
			if (factorOffset < 0)
				return nullptr;
			Byte* buffer = new Byte[_Size.Width * _Size.Height * 3];
			for (int i = 0; i < _Length; ++i)
				buffer[i * 3 + factorOffset] = _Data[i];
			return buffer;
		}

		Byte* Bitmap::InterpretMonoToQuad(int factorOffset) const
		{
			Byte* buffer = new Byte[_Size.Width * _Size.Height * 4];
			for (int i = 0; i < _Length; ++i)
			{
				buffer[i * 4 + factorOffset] = _Data[i];
				buffer[i * 4 + 3] = (Byte)0xFF;
			}
			return buffer;
		}

		Byte* Bitmap::InterpretTriToMono(int factorOffset) const
		{
			int pixelCount = _Size.Width * _Size.Height;
			Byte* buffer = new Byte[pixelCount];
			for (int i = 0; i < pixelCount; ++i)
				buffer[i] = _Data[i * 3 + factorOffset];
			return buffer;
		}

		Byte* Bitmap::InterpretTriToTri() const
		{
			int pixelCount = _Size.Width * _Size.Height * 3;
			Byte* buffer = new Byte[pixelCount];
			for (int i = 0; i < pixelCount; i += 3)
			{
				buffer[i] = _Data[i + 2];
				buffer[i + 1] = _Data[i + 1];
				buffer[i + 2] = _Data[i];
			}
			return buffer;
		}

		Byte* Bitmap::InterpretTriToQuad(bool shouldInverse) const
		{
			int pixelCount = _Size.Width * _Size.Height;
			Byte* buffer = new Byte[pixelCount * 4];
			if (shouldInverse)
			{
				for (int i = 0; i < pixelCount; ++i)
				{
					buffer[i * 4] = _Data[i * 3];
					buffer[i * 4 + 1] = _Data[i * 3 + 1];
					buffer[i * 4 + 2] = _Data[i * 3 + 2];
					buffer[i * 4 + 3] = (Byte)0xFF;
				}
			}
			else
			{
				for (int i = 0; i < pixelCount; ++i)
				{
					buffer[i * 4] = _Data[i * 3 + 2];
					buffer[i * 4 + 1] = _Data[i * 3 + 1];
					buffer[i * 4 + 2] = _Data[i * 3];
					buffer[i * 4 + 3] = (Byte)0xFF;
				}
			}
			return buffer;
		}

		Byte* Bitmap::InterpretQuadToMono(int factorOffset) const
		{
			int pixelCount = _Size.Width * _Size.Height;
			Byte* buffer = new Byte[pixelCount];
			for (int i = 0; i < pixelCount; ++i)
				buffer[i] = _Data[i * 4 + factorOffset];
			return buffer;
		}

		Byte* Bitmap::InterpretQuadToTri(bool shouldInverse) const
		{
			int pixelCount = _Size.Width * _Size.Height;
			Byte* buffer = new Byte[pixelCount * 3];
			if (shouldInverse)
			{
				for (int i = 0; i < pixelCount; ++i)
				{
					buffer[i * 3] = _Data[i * 4];
					buffer[i * 3 + 1] = _Data[i * 4 + 1];
					buffer[i * 3 + 2] = _Data[i * 4 + 2];
				}
			}
			else
			{
				for (int i = 0; i < pixelCount; ++i)
				{
					buffer[i * 3] = _Data[i * 4 + 2];
					buffer[i * 3 + 1] = _Data[i * 4 + 1];
					buffer[i * 3 + 2] = _Data[i * 4];
				}
			}
			return buffer;
		}

		// Static

		int Bitmap::CalculatePixelLength(PixelType pixelType)
		{
			switch (pixelType)
			{
			case PixelType::Rgba:
				return 4;
			case PixelType::Bgr:
			case PixelType::Rgb:
				return 3;
			case PixelType::Alpha:
			case PixelType::Red:
			case PixelType::Green:
			case PixelType::Blue:
				return 1;
			default:
				return 0;
			}
		}

		int Bitmap::CalculateDataLength(Size size, PixelType pixelType)
		{
			return size.Width * size.Height * CalculatePixelLength(pixelType);
		}
	}
}
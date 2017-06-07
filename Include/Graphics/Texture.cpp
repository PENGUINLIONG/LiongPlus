// File: Texture.cpp
// Author: Rendong Liang (Liong)
#include "Graphics/Texture.hpp"

_L_NS_BEG_GRAPHICS

using namespace LiongPlus::Media;

Texture::Texture(unsigned long count)
	: _Count(count)
	, _Index(new TextureIndex[count])
	, _MagFilter(Filter::Linear)
	, _MinFilter(Filter::Linear)
{
	glGenTextures(count, _Index);
	while (count > 0)
	{
		--count;
		glBindTexture(GL_TEXTURE_2D, _Index[count]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter::Linear);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter::Linear);
	}
}
Texture::Texture(Filter magFilter, Filter minFilter, unsigned long count)
	: _Count(count)
	, _Index(new TextureIndex[count])
	, _MagFilter(magFilter)
	, _MinFilter(minFilter)
{
	glGenTextures(count, _Index);
	while (count > 0)
	{
		--count;
		glBindTexture(GL_TEXTURE_2D, _Index[count]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	}
}
Texture::~Texture()
{
	glDeleteTextures(_Count, _Index);
	delete[] _Index;
}

TextureIndex Texture::GetIndex(unsigned long position)
{
	return _Index[position];
}

void Texture::SetMagFilter(Filter filter, unsigned long position)
{
	glBindTexture(GL_TEXTURE_2D, _Index[position]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}

void Texture::SetMinFilter(Filter filter, unsigned long position)
{
	glBindTexture(GL_TEXTURE_2D, _Index[position]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
}

void Texture::Update(Image& image, unsigned long position)
{
	Buffer buffer = image.Interpret(PixelType::Rgba);
	glBindTexture(GL_TEXTURE_2D, _Index[position]);
	//gluBuild2DMipmapsIO_TEXTURE_2D, 3, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data); // Cannot work properly.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)image.Width, (GLsizei)image.Height, 0, GL_RGBA, GL_BYTE, buffer.Field()); // Alternative.
}

// Static

//Texture Texture::FromFile(std::string& path)
//{
//	switch (File::Detect(path.c_str()))
//	{
//	case FileFormat::Bmp:
//	}
//}
_L_NS_END_GRAPHICS

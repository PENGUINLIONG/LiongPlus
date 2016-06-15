// File: Texture.hpp
// Author: Rendong Liang (Liong)
#include "../Fundamental.hpp"

#ifdef _L_GRAPHICS

#ifndef Texture_hpp
#define Texture_hpp

#include "../Media/Image.hpp"

using namespace LiongPlus::Media;

namespace LiongPlus
{
	namespace Graphics
	{
		typedef GLuint TextureIndex;

		class Texture
		{
		public:
			/*
			 * [for who is new to OpenGL]
			 * There are two types of filter usage of this enum:
			 * One is for the 'min filter' which is used when the rendered texture is smaller than the original one, represented by GL_TEXTURE_MIN_FILTER.
			 * Another is for the 'mag filter' which is used when the rendered texture is larger than the original one, represented by GL_TEXTURE_MAG_FILTER.
			 * The mip map is a optimization for rendering far objects. GL will make several copies of a image which is smaller in size. These copies are of priority, which means they will be used instead of the original one when the target position is farther than a specific distant.
			 * In some ways, the mip maps and the original image are  by a texture.
			 * [for who is new to OpenGL]
			 */
			enum Filter
			{
				Nearest = GL_NEAREST,
				Linear = GL_LINEAR,
				// The following items are for min filter only.
				NearestWithNearestMipmap = GL_NEAREST_MIPMAP_NEAREST,
				NearestWithLinearMipmap = GL_NEAREST_MIPMAP_LINEAR,
				LinearWithNearestMipmap = GL_LINEAR_MIPMAP_NEAREST,
				LinearWithLinearMipmap = GL_NEAREST_MIPMAP_LINEAR
			};

			Texture(unsigned long count = 1);
			Texture(Filter magFilter, Filter minFilter, unsigned long count = 1);
			~Texture();

			TextureIndex GetIndex(unsigned long position);
			void SetMagFilter(Filter filter, unsigned long position);
			void SetMinFilter(Filter filter, unsigned long position);
			/*
			 * Update image data to texture of the specified position.
			 */
			void Update(Image& image, unsigned long position = 0);

			// Static

			static Texture FromFile(std::string& path);
		private:
			unsigned long _Count;
			TextureIndex* _Index;
			Filter _MagFilter, _MinFilter;
		};
		/*typedef shared_ptr<Texture> TextureStrongRef;
		typedef weak_ptr<Texture> TextureRef;*/

		struct TextureComponent
		{
			Rect Rect;
			Texture *Texture;
		};
	}
}
#endif
#endif
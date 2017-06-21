#ifndef LIB_BACKEND_SFML_TEXTUREFACTORY_HPP
#define LIB_BACKEND_SFML_TEXTUREFACTORY_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/vector2d.hpp>

#include <lib/backend/itexturefactory.hpp>
#include "texture.hpp"

namespace lib
{
	namespace backend
	{
		namespace sfmlb
		{
			class Texture;
			class TextureFactory : public ITextureFactory
			{
			public:
				virtual ITexture *loadFromFile(const str &file) override;
				virtual ~TextureFactory();
			private:
				vector<uptr<Texture>> m_textureCache;
			};
		}
	}
}

#endif

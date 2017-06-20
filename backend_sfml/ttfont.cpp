#include "ttfont.hpp"
#include "texturettfont.hpp"
#include "conversions.hpp"

namespace lib
{
	namespace backend
	{
		namespace sfmlb
		{
			TTFont::TTFont(const sf::Font & f) : m_font{ f } {}
			TTFont::~TTFont() 
			{
				for (auto&& texture : m_fontTexturesCache) {
					delete texture.second;
				}
			}

			const ITTGlyph TTFont::getGlyph(u32 codePoint, u32 characterSize, bool bold, f32 outlineThickness) const
			{
				const sf::Glyph &glyph(m_font.getGlyph(codePoint, characterSize, bold, outlineThickness));
				return ITTGlyph{ toRect<f32>(glyph.bounds), toRect<s32>(glyph.textureRect), glyph.advance };
			}

			f32 TTFont::getLineSpacing(u32 characterSize) const
			{
				return m_font.getLineSpacing(characterSize);
			}

			f32 TTFont::getKerning(u32 first, u32 second, u32 characterSize) const
			{
				return m_font.getKerning(first, second, characterSize);
			}

			ITexture *TTFont::getTexture(u32 characterSize)
			{
				auto iterator(m_fontTexturesCache.find(characterSize));
				if (iterator != m_fontTexturesCache.end()) {
					return iterator->second;
				}

				auto *nTexture(new TextureTTFont( m_font.getTexture(characterSize) ));
				m_fontTexturesCache[characterSize] = nTexture;
				return nTexture;
			}
		}
	}
}

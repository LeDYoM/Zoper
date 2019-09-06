#include "bmpfontfactory.hpp"
#include "bmpfont.hpp"
#include "texture.hpp"

namespace lib::backend::bmpf
{
    IBMPFont* BMPFontFactory::loadFromFile(const str & file)
    {
        uptr<sf::Font> font(muptr<sf::Font>());
        font->loadFromFile(file.c_str());
        uptr<TTFont> t{ muptr<TTFont>(std::move(font), RawMemory{} ) };
        m_fontCache.push_back(std::move(t));
        return (*(m_fontCache.end() - 1)).get();
    }

    IBMPFont * BMPFontFactory::loadFromRawMemory(RawMemory * raw_memory)
    {
        uptr<sf::Font> font(muptr<sf::Font>());
        RawMemory internal_raw_memory(*raw_memory);
        font->loadFromMemory(internal_raw_memory.data(), internal_raw_memory.size());
        uptr<TTFont> t{ muptr<TTFont>(std::move(font), std::move(internal_raw_memory)) };
        m_fontCache.push_back(std::move(t));
        return (*(m_fontCache.end() - 1)).get();
    }

    BMPFontFactory::~BMPFontFactory()
    {
        m_fontCache.clear();
        m_fontCache.shrink_to_fit();
    }
}

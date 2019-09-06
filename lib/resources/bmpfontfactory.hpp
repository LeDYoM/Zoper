#pragma once

#ifndef LIB_BACKEND_BMPF_BMPFONTFACTORY_HPP
#define LIB_BACKEND_BMPF_BMPFONTFACTORY_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/vector2d.hpp>

#include <backend_dev/include/iresourcefactories.hpp>
#include "bmpfont.hpp"

namespace lib::backend::bmpf
{
    class BMPFontFactory : public IBMPFontFactory
    {
    public:
        IBMPFont* loadFromFile(const str &file) override;
        IBMPFont *loadFromRawMemory(RawMemory *raw_memory) override;
        ~BMPFontFactory() override;
    private:
        vector<uptr<scene::BMPFont>> m_fontCache;
    };
}

#endif

#pragma once

#ifndef LIB_SCENE_BMPFONT_INTERFACE_INCLUDE_HPP
#define LIB_SCENE_BMPFONT_INTERFACE_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/rect.hpp>

#include <lib/include/resources/itexture.hpp>
#include <lib/include/resources/ifont.hpp>

namespace lib::scene
{
    class IBMPFont
    {
    public:
        virtual ~IBMPFont() {}
        virtual Rectf32 getBounds(const u32 codePoint) const = 0;
        virtual Rectf32 getTextureBounds(const u32 codePoint) const = 0;
        virtual f32 getAdvance(const u32 codePoint) const = 0;
        virtual f32 getLineSpacing() const = 0;
        virtual f32 getKerning(const u32 first, const u32 second) const = 0;
        virtual sptr<ITexture> getTexture() const = 0;
        virtual vector2df textSize(const str& text) const = 0;

    };
}

#endif

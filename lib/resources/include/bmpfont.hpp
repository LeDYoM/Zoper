#pragma once

#ifndef LIB_SCENE_RESOURCES_BMPFONT_INCLUDE_HPP
#define LIB_SCENE_RESOURCES_BMPFONT_INCLUDE_HPP

#include <mtypes/include/types.hpp>

#include <lib/resources/include/ifont.hpp>
#include <lib/resources/include/iresourcehandler.hpp>
#include <lib/resources/include/itexture.hpp>

namespace lib::scene
{
class BMFontPrivate;

class BMPFont : public IFont
{
public:
    BMPFont(const str &file_name);
    ~BMPFont() override;

    Rectf32 getBounds(const u32 codePoint) const override;
    Rectf32 getTextureBounds(const u32 codePoint) const override;
    f32 getAdvance(const u32 codePoint) const override;
    f32 getLineSpacing() const override;
    f32 getKerning(const u32 first, const u32 second) const override;
    sptr<ITexture> getTexture() const override;
    vector2df textSize(const str &text) const override;
    const vector2du32 &size() const;

    vector<str> textureFileNames() const;
    void setTexturePages(const vector<sptr<ITexture>> &texture_pages);

private:
    BMFontPrivate *fontPrivate;
    bool ParseFont(const str &);
};
} // namespace lib::scene

#endif

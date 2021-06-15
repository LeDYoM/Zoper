#pragma once

#ifndef HAF_RENDER_RENDERIZABLE_INTERNAL_DATA_INCLUDE_HPP
#define HAF_RENDER_RENDERIZABLE_INTERNAL_DATA_INCLUDE_HPP

#include <haf/include/render/renderizable.hpp>

namespace haf::scene
{

struct Renderizable::RenderizableInternalData
{
    FigType_t const& figType;
    htps::Rectf32 const& box;
    Color const& color;
    htps::size_type const& pointCount;
    htps::sptr<res::IShader> const& shader;
    htps::Rects32 const& textureRect;
    htps::sptr<res::ITexture> const& texture;
    htps::function<Color(const RenderizableModifierContext&)> const&
        color_modifier;
};

}  // namespace haf::scene

#endif
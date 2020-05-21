#pragma once

#ifndef HAF_RENDERIZABLE_BUILDER_INCLUDE_HPP
#define HAF_RENDERIZABLE_BUILDER_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/rect.hpp>
#include <mtypes/include/str.hpp>
#include <mtypes/include/function.hpp>

#include <haf/scene/include/color.hpp>
#include <haf/scene/include/renderizable_modifier_context.hpp>

namespace haf::scene
{
class Renderizables;
class Renderizable;
class IShader;
class ITexture;

enum class FigType_t : mtps::u8;

class RenderizableBuilder
{
public:
    RenderizableBuilder(mtps::rptr<Renderizables> renderizables) noexcept;

    mtps::sptr<Renderizable> create();

    RenderizableBuilder& name(mtps::str _name);
    RenderizableBuilder& figType(FigType_t fig_type);
    RenderizableBuilder& box(mtps::Rectf32 _box);
    RenderizableBuilder& color(Color _color);
    RenderizableBuilder& pointCount(mtps::size_type point_count);
    RenderizableBuilder& shader(mtps::sptr<IShader> _shader);
    RenderizableBuilder& texture(mtps::sptr<ITexture> _texture);
    RenderizableBuilder& colorModifier(
        mtps::function<Color(const RenderizableModifierContext&)>
            color_modifier);

    struct Data
    {
        mtps::rptr<Renderizables> renderizables_;
        mtps::str name_;
        FigType_t figType_;
        mtps::Rectf32 box_;
        Color color_;
        mtps::size_type pointCount_;
        mtps::sptr<IShader> shader_;
        mtps::sptr<ITexture> texture_;
        mtps::function<Color(const RenderizableModifierContext&)> color_modifier_;
    };

    const Data& data() const { return data_; }

private:
    RenderizableBuilder::Data data_;
};
}  // namespace haf::scene

#endif

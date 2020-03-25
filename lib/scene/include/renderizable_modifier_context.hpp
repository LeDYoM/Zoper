#pragma once

#ifndef LIB_SCENE_RENDERIZABLE_MODIFIER_CONTEXT_INCLUDE_HPP
#define LIB_SCENE_RENDERIZABLE_MODIFIER_CONTEXT_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/rect.hpp>
#include <mtypes/include/vector2d.hpp>
#include <lib/scene/include/vertex.hpp>

namespace lib::scene
{
struct RenderizableModifierContext
{
    const mtps::Rectf32 box;
    const mtps::Rects32 texture_rect;
    const mtps::vector2du32 texture_size;
    const Vertex vertex;
    constexpr mtps::vector2df normalizedVertexInBox() const
    {
        return (box.rightBottom() - vertex.position) / box.rightBottom();
    }
};
} // namespace lib::scene

#endif

#pragma once

#ifndef HAF_COMPONENT_RENDERIZABLES_INCLUDE_HPP
#define HAF_COMPONENT_RENDERIZABLES_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/function.hpp>
#include <mtypes/include/vector.hpp>

#include <haf/scene/include/renderizable.hpp>
#include <haf/scene/include/renderizable_builder.hpp>

namespace haf::scene
{
class SceneNode;
class RenderizableBuilder;
extern template mtps::vector<mtps::sptr<Renderizable>>;

class Renderizables
{
public:
    Renderizables(mtps::rptr<SceneNode> scene_node) noexcept :
        scene_node_{std::move(scene_node)}
    {}

    /// Method to create a Renderizable
    /// @param name The name of the Renderizable node.
    /// @param figure_type Type of the figure to generate.
    /// @param box Box containing the Renderizable.
    /// @param color Color of the Renderizable.
    /// @param num_points Number of points of the Renderizable.
    /// @returns The created Renderizable
    mtps::sptr<Renderizable> createRenderizable(mtps::str name,
                                                FigType_t figure_type,
                                                mtps::Rectf32 box,
                                                Color color,
                                                mtps::sptr<ITexture> texture,
                                                mtps::sptr<IShader> shader,
                                                mtps::size_type num_points);

    RenderizableBuilder renderizableBuilder();

    void removeRenderizable(const mtps::sptr<Renderizable>& element);
    void clearRenderizables();

    template <typename T>
    constexpr void for_each_node_as(
        mtps::function<void(const mtps::sptr<T>&)> action)
    {
        for_each_node([&action](const mtps::sptr<Renderizable>& node) {
            if (auto tnode = std::dynamic_pointer_cast<T>(node))
            {
                action(tnode);
            }
        });
    }

    void for_each_node(
        mtps::function<void(const mtps::sptr<Renderizable>&)> action) const;

    void updateRenderizables();

private:
    void addRenderizable(mtps::sptr<Renderizable> newElement);
    mtps::rptr<SceneNode> scene_node_;
    mtps::vector<mtps::sptr<Renderizable>> render_nodes_;
};
}  // namespace haf::scene

#endif

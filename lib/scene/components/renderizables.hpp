#pragma once

#ifndef LIB_COMPONENT_RENDERIZABLES_INCLUDE_HPP
#define LIB_COMPONENT_RENDERIZABLES_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/function.hpp>
#include <mtypes/include/vector.hpp>

#include <lib/scene/icomponent.hpp>

#include <lib/scene/renderizables/renderizable.hpp>

namespace lib::scene
{
class Renderizable;

class Renderizables : public IComponent
{
public:
    Renderizables();
    ~Renderizables() override;

    void onAttached() override;

    /// Method to create a Renderizable
    /// @param name The name of the Renderizable node.
    /// @returns The created Renderizable
    sptr<Renderizable> createNode(const str &name)
    {
        auto result(msptr<Renderizable>(attachedNode(), std::move(name), 
            FigType_t::Quad, 0U, Rectf32{}, colors::White, nullptr, nullptr));
        addRenderizable(result);
        return result;
    }

    sptr<Renderizable> createNode(str name,
                                Rectf32 box, Color color = colors::White,
                                size_type num_points = 4U)
    {
        auto result(msptr<Renderizable>(
            attachedNode(), std::move(name), 
            FigType_t::Quad, std::move(num_points),
            std::move(box), std::move(color), nullptr, nullptr));
        addRenderizable(result);
        return result;
    }

    void removeRenderizable(const sptr<Renderizable> &element);
    void clearRenderizables();

    template <typename T>
    constexpr void for_each_node_as(function<void(const sptr<T> &)> action)
    {
        for_each_node([&action](const sptr<Renderizable> &node) {
            if (auto tnode = std::dynamic_pointer_cast<T>(node))
            {
                action(tnode);
            }
        });
    }

    void for_each_node(function<void(const sptr<Renderizable> &)> action) const;

    void postUpdate() override;

private:
    void addRenderizable(sptr<Renderizable> newElement);

    vector<sptr<Renderizable>> render_nodes_;
};
} // namespace lib::scene

#endif

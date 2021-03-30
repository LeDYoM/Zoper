#ifndef HAF_SCENE_SCENENODE_UTILS_INCLUDE_HPP
#define HAF_SCENE_SCENENODE_UTILS_INCLUDE_HPP

#include <haf/haf_export.hpp>
#include <htypes/include/types.hpp>
#include <htypes/include/function.hpp>
#include <haf/scene/include/scenenode.hpp>

namespace haf::scene
{
HAF_API void for_each_sceneNode(
    SceneNode& node,
    htps::function<void(htps::sptr<SceneNode> const&)> action);

HAF_API void for_each_sceneNode(
    SceneNode const& node,
    htps::function<void(htps::sptr<SceneNode const> const&)> action);

template <typename T>
constexpr void for_each_sceneNode_as(
    SceneNode& node,
    htps::function<void(htps::sptr<T> const&)> action)
{
    for_each_sceneNode(node, [&action](htps::sptr<SceneNode> const& node) {
        if (auto const tnode = std::dynamic_pointer_cast<T>(node))
        {
            action(tnode);
        }
    });
}

template <typename T>
constexpr void for_each_sceneNode_as(
    SceneNode const& node,
    htps::function<void(htps::sptr<T const> const&)> action)
{
    for_each_sceneNode(node, [&action](htps::sptr<SceneNode const> const& node) {
        if (auto const tnode = std::dynamic_pointer_cast<T const>(node))
        {
            action(tnode);
        }
    });
}

}  // namespace haf::scene
#endif
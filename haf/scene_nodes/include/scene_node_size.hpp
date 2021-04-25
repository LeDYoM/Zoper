#ifndef HAF_SCENE_SCENENODE_SIZE_PROPERTIES_INCLUDE_HPP
#define HAF_SCENE_SCENENODE_SIZE_PROPERTIES_INCLUDE_HPP

#include <htypes/include/grouping_property.hpp>
#include <htypes/include/rect.hpp>

namespace haf::scene
{
struct NodeSize
{
    using value_type = htps::Rectf32;
};

using SceneNodeSizeProperties = htps::PropertyGroup<NodeSize>;

}  // namespace haf::scene

#endif
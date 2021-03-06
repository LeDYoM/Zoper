#pragma once

#ifndef HAF_SCENE_SCENENODESGROUP_INCLUDE_HPP
#define HAF_SCENE_SCENENODESGROUP_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/vector.hpp>

namespace haf::scene
{
class SceneNode;

class SceneNodesGroup
{
public:
    using SceneNodeVector = mtps::vector<mtps::sptr<SceneNode>>;
    void addSceneNode(mtps::sptr<SceneNode> node);
    bool removeSceneNode(mtps::sptr<SceneNode> element);
    void clearSceneNodes();

    SceneNodeVector const& sceneNodes() const noexcept;
    SceneNodeVector& sceneNodes() noexcept;

private:
    SceneNodeVector groups_;
};

}  // namespace haf::scene

#endif

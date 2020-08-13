#pragma once

#ifndef HAF_SCENE_SCENENODE_INCLUDE_HPP
#define HAF_SCENE_SCENENODE_INCLUDE_HPP

#include <haf/haf_export.hpp>
#include <mtypes/include/types.hpp>
#include <mtypes/include/vector2d.hpp>
#include <mtypes/include/properties.hpp>
#include <mtypes/include/grouping_property.hpp>
#include <haf/scene/include/scenenodeparent.hpp>
#include <haf/scene/include/scenenodes.hpp>
#include <haf/scene/include/renderizables.hpp>
#include <haf/scene/include/transformable.hpp>
#include <haf/scene/include/hasname.hpp>
#include <haf/scene/include/componentcontainer.hpp>
#include <haf/scene/include/scenenode_cast.hpp>
#include <haf/scene/include/interface_getter.hpp>
#include <haf/system/include/datawrappercreator.hpp>
#include <haf/system/include/systemaccess.hpp>

namespace haf::scene
{
class Renderizable;
class Scene;
class SceneManager;

struct Visible
{
    using value_type = bool;
};

using SceneNodeProperties = mtps::PropertyGroup<Visible>;

/**
 * @brief Main class representing all SceneNodes from a Scene.
 * This class serves as main entry point in the hierarchy of the scene.
 * To create new SceneNode types, inherit from this class.
 */
class HAF_API SceneNode : public sys::HasName,
                          public SceneNodeParent,
                          public SceneNodes,
                          public Renderizables,
                          public Transformable,
                          public sys::DataWrapperCreator,
                          public ComponentContainer,
                          public sys::SystemAccess,
                          public InterfaceGetter,
                          public SceneNodeProperties
{
public:
    using SceneNodeProperties::prop;
    using TransformableProperties::prop;

    /**
     * @brief Disabled copy constructor.
     */
    SceneNode(const SceneNode&) = delete;

    /**
     * @brief Disabled copy assignment
     */
    SceneNode& operator=(const SceneNode&) = delete;

    SceneNode(SceneNode&&) noexcept = default;
    SceneNode& operator=(SceneNode&&) noexcept = default;

    /**
     * @brief Constructor normally used to create a scene node.
     *
     * @param[in] parent Parent of this element.
     * @param[in] name Name of this element.
     */
    SceneNode(mtps::rptr<SceneNode> parent, mtps::str name);

    /**
     * @brief Destroy the Scene Node object.
     */
    virtual ~SceneNode();

    /**
     * @brief Method called when adding a new node just after creation.
     * Override it to add code on creation.
     */
    virtual void onCreated() {}

    virtual void update() {}

    template <typename T>
    mtps::rptr<T> snCast() noexcept
    {
        return sceneNodeCast<T>(this);
    }

    template <typename T>
    const mtps::rptr<const T> snCast() const noexcept
    {
        return sceneNodeCast<T>(this);
    }

    void clearAll();
};

using SceneNodeSPtr = mtps::sptr<SceneNode>;
}  // namespace haf::scene

#endif

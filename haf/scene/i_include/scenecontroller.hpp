#pragma once

#ifndef HAF_SCENE_SCENECONTROLLER_INCLUDE_HPP
#define HAF_SCENE_SCENECONTROLLER_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/str.hpp>
#include <mtypes/include/vector.hpp>
#include <mtypes/include/function.hpp>

#include <haf/scene/include/scene.hpp>
#include <haf/scene/include/scenenodefactory.hpp>

namespace haf::scene
{
class SceneManager;

/**
 * @brief Class to control which @b Scene is active and which are not.
 * This controller class manages the creation, destruction and maintenance of
 * the @b Scene instances.
 */
class SceneController final
{
public:
    void setSceneManager(mtps::rptr<SceneManager> scene_manager);

    void switchToNextScene();
    void update();
    void finish();
    bool isActive();

    bool startScene(const mtps::str& sceneName);

    template <typename T>
    bool registerAndStartScene(const mtps::str& sceneName)
    {
        if (scene_factory_.registerSceneNodeType<T>(sceneName))
        {
            return startScene(sceneName);
        }
        return false;
    }

    template <typename T>
    bool registerAndStartScene()
    {
        if (scene_factory_.registerSceneNodeType<T>())
        {
            return startScene(T::StaticTypeName);
        }
        return false;
    }

    template <typename T>
    bool registerSceneNodeType()
    {
        return scene_factory_.registerSceneNodeType<T>();
    }

    template <typename T>
    bool startScene()
    {
        return startScene(T::StaticTypeName);
    }

    SceneNodeFactory& sceneNodeFactory() noexcept;
    const SceneNodeFactory& sceneNodeFactory() const noexcept;
    const mtps::sptr<Scene>& currentScene() const noexcept;
    bool currentSceneIsNull();

    void renderScene(Scene& scene, bool parentTransformationChanged);
    void render(SceneNode& scene, bool parentTransformationChanged);

    void requestExit();
    bool exitRequested() const;

private:
    void startScene(mtps::sptr<Scene> scene);
    void terminateCurrentScene();
    void deferredSwitchScene();

    SceneNodeFactory scene_factory_;
    mtps::rptr<SceneManager> scene_manager_{nullptr};
    mtps::sptr<Scene> current_scene_{nullptr};
    bool switch_scene_{false};
    bool exit_requested_{false};
};

}  // namespace haf::scene

#endif

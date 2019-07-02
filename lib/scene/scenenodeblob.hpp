#pragma once

#ifndef LIB_SCENE_SCENENODE_BLOBL_INCLUDE_HPP
#define LIB_SCENE_SCENENODE_BLOBL_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/rect.hpp>

namespace lib
{
    class IApp;
}

namespace lib::core
{
    class RandomSystem;
}

namespace lib::input
{
    class InputSystem;
}

namespace lib::scene
{
    class SceneNode;
    class SceneManager;

    class SceneNodeBlob
    {
    public:
        SceneNodeBlob(SceneNode& provider);

        const SceneManager &sceneManager() const noexcept;
        SceneManager &sceneManager() noexcept;

        const input::InputSystem& inputSystem() const;
        input::InputSystem& inputSystem();

        const core::RandomSystem& randomSystem() const;
        core::RandomSystem& randomSystem();

        const IApp& app() const;
        IApp& app();

        template <typename T>
        T &app() { return (static_cast<T&>(app())); }

        Rectf32 scenePerspective() const;

    private:
        SceneNode& scene_node_;
    };
}

#endif

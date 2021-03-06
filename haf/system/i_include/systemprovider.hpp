#pragma once

#ifndef HAF_SYSTEM_PROVIDER_INCLUDE_HPP
#define HAF_SYSTEM_PROVIDER_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <haf/system/include/isystemprovider.hpp>

namespace haf
{
class IApp;

namespace backend
{
class BackendFactory;
}

namespace scene
{
class SceneManager;
}

namespace sys
{
class InputSystem;
class Window;
class ResourceManager;
class RandomSystem;
class FileSystem;
class SimulationSystem;
class TimeSystem;
class RenderSystem;
class SharedDataSystem;
}  // namespace sys
}  // namespace haf

namespace haf::sys
{
struct InitSystemOptions
{
    bool init_shared_data_system{false};
    bool init_time_system{false};
    bool init_window_system{false};
    bool init_input_system{false};
    bool init_scene_manager{false};
    bool init_resource_manager{false};
    bool init_render_system{false};
    bool init_random_system{false};
    bool init_file_system{false};
    bool init_simulation_system{false};

    void setAllTrue()
    {
        init_shared_data_system = true;
        init_time_system        = true;
        init_window_system      = true;
        init_input_system       = true;
        init_scene_manager      = true;
        init_resource_manager   = true;
        init_render_system      = true;
        init_random_system      = true;
        init_file_system        = true;
        init_simulation_system  = true;
    }
};

class SystemProvider : public ISystemProvider
{
public:
    SystemProvider();
    ~SystemProvider() override;

    void fastInit(InitSystemOptions const& init_system_options);
    void init(mtps::rptr<IApp> iapp, int const argc, char const* const argv[]);
    void terminate();

    template <typename T>
    T& app()
    {
        return *(dynamic_cast<T*>(&app()));
    }

    template <typename T>
    const T& app() const
    {
        return *(dynamic_cast<T*>(&app()));
    }

    IApp& app();
    const IApp& app() const;

    const Window& parentWindow() const noexcept;
    Window& parentWindow() noexcept;
    const ResourceManager& resourceManager() const noexcept;
    ResourceManager& resourceManager() noexcept;
    const sys::InputSystem& inputSystem() const noexcept;
    sys::InputSystem& inputSystem() noexcept;
    const RandomSystem& randomSystem() const noexcept;
    RandomSystem& randomSystem() noexcept;
    const scene::SceneManager& sceneManager() const noexcept;
    scene::SceneManager& sceneManager() noexcept;
    const FileSystem& fileSystem() const noexcept;
    FileSystem& fileSystem() noexcept;
    const SimulationSystem& simulationSystem() const noexcept;
    SimulationSystem& simulationSystem() noexcept;
    const TimeSystem& timeSystem() const noexcept;
    TimeSystem& timeSystem() noexcept;
    const backend::BackendFactory& backendFactory() const noexcept;
    backend::BackendFactory& backendFactory() noexcept;
    const RenderSystem& renderSystem() const noexcept;
    RenderSystem& renderSystem() noexcept;
    SharedDataSystem& sharedDataSystem() noexcept;
    const SharedDataSystem& sharedDataSystem() const noexcept;

private:
    struct SystemProviderPrivate;
    mtps::uptr<SystemProviderPrivate> p_;
};
}  // namespace haf::sys

#endif

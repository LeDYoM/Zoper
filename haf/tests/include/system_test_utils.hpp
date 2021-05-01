#ifndef HAF_SYSTEM_TESTS_UTILS_INCLUDE_HPP
#define HAF_SYSTEM_TESTS_UTILS_INCLUDE_HPP

#include <htypes/include/types.hpp>
#include <system/i_include/systemprovider.hpp>
#include <system/i_include/systemdatawrappercreator.hpp>
#include <haf/system/include/datawrappercreator.hpp>
#include <haf/scene/include/interface_getter.hpp>

#include <shareddata/i_include/shareddatasystem.hpp>
#include <time/i_include/timesystem.hpp>
#include <scene/i_include/scenemanager.hpp>
#include <system/i_include/system_options_init.hpp>

template <typename T>
class TestSystem
{
public:
    TestSystem() : system_provider_{}, init_system_options_{} {}

    haf::sys::SystemDataWrapperCreator get()
    {
        return haf::sys::SystemDataWrapperCreator{
            systemProvider().template system<T>()};
    }

    void init()
    {
        setInitSystemOptions(init_system_options_);
        system_provider_.fastInit(init_system_options_);
    }

    virtual ~TestSystem() = default;

    void setInitSystemOptions(haf::sys::InitSystemOptions& init_system_options)
    {
        haf::sys::setInitSystem<T>(init_system_options);
    }

    template <typename Y>
    T& systemInterface()
    {
        haf::sys::SystemAccess system_access(&system_provider_);
        return haf::sys::getInterface<Y>(system_access);
    }

    template <typename Y>
    T const& systemInterface() const
    {
        haf::sys::SystemAccess system_access(&system_provider_);
        return haf::sys::getInterface<Y>(system_access);
    }

    haf::sys::SystemProvider& systemProvider() noexcept
    {
        return system_provider_;
    }

    haf::sys::SystemProvider const& systemProvider() const noexcept
    {
        return system_provider_;
    }



    template <typename S>
    S& system()
    {
        return system_provider_.system<S>();
    }

    template <typename S>
    S const& system() const
    {
        return system_provider_.system<S>();
    }

private:
    haf::sys::SystemProvider system_provider_;
    haf::sys::InitSystemOptions init_system_options_;
};

using TestSharedDataSystem = TestSystem<haf::sys::SharedDataSystem>;
using TestTimeSystem = TestSystem<haf::sys::TimeSystem>;
using TestSceneManager = TestSystem<haf::scene::SceneManager>;

template <typename T>
htps::uptr<T> makeTestSystem()
{
    auto t = htps::muptr<T>();
    t->init();
    return t;
}

#endif

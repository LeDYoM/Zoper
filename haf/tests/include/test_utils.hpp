#pragma once

#ifndef HAF_TESTS_UTILS_INCLUDE_HPP
#define HAF_TESTS_UTILS_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <system/i_include/systemprovider.hpp>
#include <shareddata/i_include/shareddatasystem.hpp>

class TestSystem
{
public:
    TestSystem() : system_provider_{}, init_system_options_{} {}

    void init()
    {
        setInitSystemOptions(init_system_options_);
        system_provider_.fastInit(init_system_options_);
    }

    virtual ~TestSystem() = default;

    virtual void setInitSystemOptions(haf::sys::InitSystemOptions&) = 0;

    inline haf::sys::SharedDataSystem& sharedDataSystem()
    {
        return system_provider_.sharedDataSystem();
    }

    haf::sys::SystemProvider& systemProvider() { return system_provider_; }

private:
    haf::sys::SystemProvider system_provider_;
    haf::sys::InitSystemOptions init_system_options_;
};

class TestSharedDataSystem : public TestSystem
{
    void setInitSystemOptions(
        haf::sys::InitSystemOptions& init_system_options) override
    {
        init_system_options.init_shared_data_system = true;
    }
};

template <typename T>
mtps::uptr<T> makeTestSystem()
{
    auto t = mtps::muptr<T>();
    t->init();
    return t;
}
#endif

#pragma once

#ifndef LIB_IAPP_INCLUDE_HPP
#define LIB_IAPP_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/str.hpp>

namespace lib::core
{
    class SystemProvider;
}

namespace lib
{
    class IApp
    {
    public:
        constexpr IApp() noexcept {}
        virtual ~IApp() {}

        virtual void onInit() = 0;
        virtual void onFinish() = 0;

        virtual u16 getVersion() const noexcept = 0;
        virtual u16 getSubVersion() const noexcept = 0;
        virtual u16 getPatch() const noexcept  = 0;
        virtual str getName() const noexcept  = 0;

        inline void setSystemProvider(core::SystemProvider* const system_provider) noexcept { system_provider_ = system_provider; }
        inline core::SystemProvider &systemProvider() const noexcept { return *system_provider_; }
    private:
        core::SystemProvider *system_provider_{nullptr};
    };
}

using p_initApp = lib::IApp *(*)();
using p_finishApp = bool (*)(lib::IApp* const);

#endif

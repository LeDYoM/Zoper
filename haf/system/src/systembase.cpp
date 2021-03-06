#include <system/i_include/systembase.hpp>
#include <haf/system/include/isystemprovider.hpp>
#include <haf/system/i_include/systemprovider.hpp>

namespace haf::sys
{
SystemBase::SystemBase(sys::ISystemProvider& system_provider) noexcept :
    system_provider_{system_provider}
{}

SystemBase::~SystemBase() noexcept = default;

SystemProvider& SystemBase::systemProvider() noexcept
{
    return dynamic_cast<SystemProvider&>(system_provider_);
}

SystemProvider const& SystemBase::systemProvider() const noexcept
{
    return dynamic_cast<SystemProvider&>(system_provider_);
}

ISystemProvider& SystemBase::isystemProvider() noexcept
{
    return system_provider_;
}

ISystemProvider const& SystemBase::isystemProvider() const noexcept
{
    return system_provider_;
}

}  // namespace haf::sys

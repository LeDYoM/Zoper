#ifndef HAF_HOST_HOSTED_APPLICATION_INCLUDE_HPP
#define HAF_HOST_HOSTED_APPLICATION_INCLUDE_HPP

#include <htypes/include/types.hpp>
#include <htypes/include/str.hpp>
#include "app_loader.hpp"
#include "app_state.hpp"
#include <hosted_app/include/iapp.hpp>

namespace haf::host
{
class HostedApplication final
{
public:
    HostedApplication(ManagedApp managed_app,
                      htps::str app_name) noexcept;

    ManagedApp managed_app_;
    htps::str app_name_;
    AppState app_state{AppState::NotInitialized};
};

bool operator==(HostedApplication const& lhs,
                HostedApplication const& rhs) noexcept;

htps::str appDisplayNameAndVersion(IApp const& app);
htps::str appDisplayNameAndVersion(ManagedApp const& app);
htps::str appDisplayNameAndVersion(HostedApplication const& app);

}  // namespace haf::host

#endif

#include "hosted_application.hpp"

using namespace htps;

namespace haf::host
{

HostedApplication::HostedApplication(
    ManagedApp managed_app,
    str app_name) noexcept :
    managed_app_{std::move(managed_app)},
    app_name_{std::move(app_name)}
{}

bool operator==(HostedApplication const& lhs,
                HostedApplication const& rhs) noexcept
{
    return lhs.app_name_ == rhs.app_name_;
}

str appDisplayNameAndVersion(const IApp& app)
{
    return make_str(app.getName(), "(", app.getVersion(), ".",
                    app.getSubVersion(), ".", app.getPatch(), ")");
}

str appDisplayNameAndVersion(ManagedApp const& app)
{
    return appDisplayNameAndVersion(*(app.app));
}

str appDisplayNameAndVersion(HostedApplication const& app)
{
    return appDisplayNameAndVersion(app.managed_app_);
}

}  // namespace haf::host

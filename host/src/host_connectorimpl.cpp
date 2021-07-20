#include "host_connectorimpl.hpp"
#include "host_private.hpp"

namespace haf::host
{
HostConnectorImpl::HostConnectorImpl(Host::HostPrivate& host_private) :
    host_private_{host_private}
{}

HostConnectorImpl::~HostConnectorImpl() = default;

bool HostConnectorImpl::loadApplication(htps::str const& app_name)
{
    return host_private_.loadApplication(app_name);
}

}  // namespace haf::host

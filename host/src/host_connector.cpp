#include "host_connector.hpp"
#include "host_connectorimpl.hpp"

namespace haf::host
{
HostConnector::HostConnector(
    htps::uptr<HostConnectorImpl> host_connector_impl) :
    host_connector_impl_{std::move(host_connector_impl)}
{}

HostConnector::~HostConnector() = default;

bool HostConnector::loadApplication(htps::str const& app_name)
{
    return host_connector_impl_->loadApplication(app_name);
}

}  // namespace haf::host

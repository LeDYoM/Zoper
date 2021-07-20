#ifndef HAF_HOST_HOST_CONNECTOR_IMPL_INCLUDE_HPP
#define HAF_HOST_HOST_CONNECTOR_IMPL_INCLUDE_HPP

#include "host_private.hpp"

namespace haf::host
{
class HostConnectorImpl final
{
public:
    HostConnectorImpl(Host::HostPrivate& host_private);
    ~HostConnectorImpl();

    bool loadApplication(htps::str const& app_name);

private:

    Host::HostPrivate& host_private_;
};

}  // namespace haf::host

#endif

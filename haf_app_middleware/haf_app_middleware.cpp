#include "haf_app_middleware.hpp"

#include <haf/include/scene_components/app_finisher.hpp>
#include <haf/include/scene_components/app_initializer.hpp>
#include <host_connector/include/ihost_connector.hpp>
#include <hlog/include/hlog.hpp>

using namespace htps;
using namespace haf;
using namespace haf::scene;

namespace haf::app_middleware
{
void HafAppMiddleware::onInit(AppInitializer& app_initializer)
{
    (void)(app_initializer);
    DisplayLog::verbose("Initializing HafAppFirmware");
}

void HafAppMiddleware::onFinish(scene::AppFinisher&)
{}

void HafAppMiddleware::receiveHostConnector(
    htps::uptr<host::IHostConnector> ihost_connector)
{
    ihost_connector->loadApplication("Zoper");
}

}  // namespace haf::app_middleware

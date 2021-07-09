#include "haf_app_menu.hpp"

#include <haf/include/scene_components/app_finisher.hpp>
#include <haf/include/scene_components/app_initializer.hpp>

#include <hlog/include/hlog.hpp>

using namespace htps;
using namespace haf;
using namespace haf::scene;

namespace haf::app_menu
{

void HafAppMenu::configureScenes(haf::sys::DataWrapperCreator&)
{}

HafAppMenu::HafAppMenu()  = default;
HafAppMenu::~HafAppMenu() = default;

void HafAppMenu::onInit(AppInitializer& app_initializer)
{
    (void)(app_initializer);
    DisplayLog::verbose("Initializing HafAppMenu");

    //    configureScenes(data_wrapper_creator);
}

void HafAppMenu::onFinish(scene::AppFinisher& finisher)
{
    finisher.NextApp = "Zoper";
}

}  // namespace haf::app_menu

#include "scenemanager.hpp"
#include "scene.hpp"

#include <system/i_include/systemprovider.hpp>
#include <window/i_include/window.hpp>
#include <scene/i_include/scenecontroller.hpp>
#include <resources/i_include/resourcemanager.hpp>
#include <render/i_include/rendertarget.hpp>

#include <hlog/include/hlog.hpp>
#include <haf/resources/include/iresourceretriever.hpp>
#include <haf/resources/include/iresourcehandler.hpp>

using namespace mtps;

namespace haf::scene
{
SceneManager::SceneManager(sys::SystemProvider& system_provider) :
    AppService{system_provider}
{
    scene_controller_ = msptr<SceneController>();
    scene_controller_->setSceneManager(this);
}

SceneManager::~SceneManager() = default;

void SceneManager::start()
{}

void SceneManager::update()
{
    scene_controller_->update();
}

void SceneManager::finish()
{
    scene_controller_->finish();
}

Rectf32 SceneManager::viewPort() const
{
    return systemProvider().parentWindow().renderTarget()->viewPort();
}

void SceneManager::setViewPort(const Rectf32& vp)
{
    systemProvider().parentWindow().renderTarget()->setViewPort(vp);
}

Rectf32 SceneManager::viewRect() const
{
    return systemProvider().parentWindow().renderTarget()->viewRect();
}

void SceneManager::setViewRect(const Rectf32& vr)
{
    systemProvider().parentWindow().renderTarget()->setViewRect(vr);
}

IResourceRetriever& SceneManager::resources()
{
    return systemProvider().resourceManager();
}

IResourceHandler& SceneManager::resourcesLoader()
{
    return systemProvider().resourceManager();
}

const sptr<SceneController>& SceneManager::sceneController() const noexcept
{
    return scene_controller_;
}

sptr<SceneController>& SceneManager::sceneController() noexcept
{
    return scene_controller_;
}

}  // namespace haf::scene

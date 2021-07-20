#include "host_private.hpp"
#include "host.hpp"
#include "host_connector.hpp"
#include "host_connectorimpl.hpp"

using namespace htps;

namespace haf::host
{
Host::HostPrivate::HostPrivate(const int argc, char const* const argv[]) :
    argc_{argc},
    argv_{argv},
    backend_factory_{nullptr, nullptr},
    config_{argc, argv},
    params_{parpar::create(argc, argv)}
{}

str Host::HostPrivate::configuredFirstApp() const
{
    return config_.configuredFirstApp();
}

rptr<sys::ISystemController> Host::HostPrivate::systemController() noexcept
{
    return system_loader_.systemController();
}

rptr<sys::ISystemController const> Host::HostPrivate::systemController()
    const noexcept
{
    return system_loader_.systemController();
}

bool Host::HostPrivate::initialize()
{
    // Initialize and create the backend factory
    backend_factory_ =
        uptr<backend::BackendFactory, void (*)(haf::backend::BackendFactory*)>(
            createBackendFactory(), destroyBackendFactory);

    // Return the status of loading the first application
    return backend_factory_ != nullptr && loadApplication(configuredFirstApp());
}

bool Host::HostPrivate::loopStep()
{
    return systemController()->runStep();
}

bool Host::HostPrivate::update()
{
    switch (app_group_.currentHostedApplication().app_state)
    {
        case AppState::NotInitialized:
            break;
        case AppState::ReadyToStart:
        {
            DisplayLog::info("Starting initialization of new App...");
            auto& app         = app_group_.currentHostedApplication();
            app.app_state     = AppState::Executing;
            auto const result = system_loader_.loadFunctions();
            if (result != SystemControllerLoader::ResultType::Success)
            {
                DisplayLog::error("Cannot load haf system!");
                app.app_state = AppState::ReadyToTerminate;
            }
            else if (!system_loader_.create())
            {
                DisplayLog::error("Cannot create haf system!");
                app.app_state = AppState::ReadyToTerminate;
            }
            else
            {
                auto host_connector_impl = muptr<HostConnectorImpl>(*this);
                auto host_connector =
                    muptr<HostConnector>(std::move(host_connector_impl));
                app.managed_app_.app->receiveHostConnector(
                    std::move(host_connector));
                systemController()->init(
                    app_group_.currentHostedApplication().managed_app_.app,
                    nullptr, backend_factory_.get(), argc_, argv_);

                DisplayLog::info(appDisplayNameAndVersion(app_group_.currentHostedApplication()),
                                 ": Starting execution...");
            }
        }
        break;
        case AppState::Executing:
        {
            if (loopStep())
            {
                auto& app     = app_group_.currentHostedApplication();
                app.app_state = AppState::ReadyToTerminate;
                DisplayLog::info(appDisplayNameAndVersion(app),
                                 ": is now ready to terminate");
            }
        }
        break;
        case AppState::ReadyToTerminate:
        {
            auto& app = app_group_.currentHostedApplication();

            DisplayLog::info(appDisplayNameAndVersion(app),
                             ": started termination");
            app.app_state = AppState::Terminated;
            systemController()->terminate();
            system_loader_.destroy();
            return true;
        }
        break;
        case AppState::Terminated:
            return true;
            break;
        default:
            break;
    }
    return false;
}

bool Host::HostPrivate::addApplication(ManagedApp managed_app, htps::str name)
{
    return app_group_.try_add_app(std::move(managed_app), std::move(name));
}

bool Host::HostPrivate::loadApplication(htps::str const& app_name)
{
    ManagedApp managed_app = app_loader.loadApp(app_name);
    return managed_app.app != nullptr
        ? addApplication(std::move(managed_app), app_name)
        : false;
}

bool Host::HostPrivate::unloadApplication(htps::str const& app_name)
{
    if (app_group_.appExists(app_name))
    {
        // This is safe, given that app exists
        auto& app = app_group_.getAppByName(app_name)->managed_app_;
        app_loader.unloadApp(app);
        return app_group_.removeApp(app_name);
    }
    return false;
}

}  // namespace haf::host

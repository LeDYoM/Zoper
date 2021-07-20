#include "hosted_app_group.hpp"
#include "host_connector.hpp"

#include <hlog/include/hlog.hpp>

using namespace htps;

namespace haf::host
{
bool HostedAppGroup::empty() const noexcept
{
    return app_.empty();
}

size_type HostedAppGroup::size() const noexcept
{
    return app_.size();
}

HostedApplication& HostedAppGroup::currentHostedApplication()
{
    return app_[index_current_app];
}

HostedApplication const& HostedAppGroup::currentHostedApplication() const
{
    return app_[index_current_app];
}

rptr<IApp const> HostedAppGroup::currentApp() const
{
    return app_[index_current_app].managed_app_.app;
}

rptr<IApp> HostedAppGroup::currentApp()
{
    return app_[index_current_app].managed_app_.app;
}

void HostedAppGroup::setCurrentAppState(AppState const app_state) noexcept
{
    currentHostedApplication().app_state = app_state;
}

bool HostedAppGroup::try_add_app(ManagedApp managed_app,
                                 htps::str name)
{
    LogAsserter::log_assert(managed_app.app != nullptr,
                            "Received nullptr Application");

    // Store if the app is not already registered
    bool const is_new_app{!appExists(name)};

    DisplayLog::error_if(!is_new_app, "Application already registered");

    if (is_new_app)
    {
        DisplayLog::info("Starting Registering app...");
        add_app(std::move(managed_app), std::move(name));
        DisplayLog::verbose("Starting new app...");
        setCurrentAppState(AppState::ReadyToStart);
    }

    return is_new_app;
}

bool HostedAppGroup::removeApp(htps::str const& app_name)
{
    // First step, search the app in the array
    auto const app_iterator = getAppByName(app_name);

    // If the app is found, remove it from the group
    if (app_iterator != app_.end())
    {
        // Aplication found. Execute unload steps.
        auto const old_size = app_.size();

        // Remove the application from the list
        app_.erase_iterator(app_iterator, app_.end());

        auto const new_size{app_.size()};

        // Show logs informing the user
        DisplayLog::info_if(old_size != new_size, "Application ", app_name,
                            " unloaded");

        DisplayLog::info_if(old_size == new_size, "Application ", app_name,
                            " unloaded, but cannot be deleted");

        return true;
    }

    return false;
}

bool HostedAppGroup::appExists(htps::str const& name) noexcept
{
    // Search for a pointer to the same app
    return (app_.cfind(HostedApplication{ManagedApp{}, name}) !=
            app_.cend());
}

void HostedAppGroup::add_app(ManagedApp&& app,
                             htps::str name)
{
    app_.emplace_back(std::move(app), std::move(name));
}

}  // namespace haf::host

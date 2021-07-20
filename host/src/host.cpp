#include "host.hpp"

#include "host_private.hpp"
#include "systemcontroller_loader.hpp"

#include <hosted_app/include/iapp.hpp>

#include <exception>

using namespace htps;

namespace haf::host
{
Host::Host(int argc, char* argv[]) : p_{muptr<HostPrivate>(argc, argv)}
{
    DisplayLog::info("Starting HostController...");
    DisplayLog::info("Host version: ", host_VERSION, ".", host_SUBVERSION, ".",
                     host_PATCH, ".", host_TWEAK);
}

Host::~Host()
{
    DisplayLog::info("Terminating Host...");
    DisplayLog::verbose_if(!p_->app_group_.empty(),
                           p_->app_group_.size(),
                           " pending apps to be terminated");

    while (!p_->app_group_.empty())
    {
        HostedApplication const& last = p_->app_group_.back();
        p_->unloadApplication(last.app_name_);
    }

    DisplayLog::info("All applications unloaded");
}

bool Host::initialize()
{
    return p_->initialize();
}

int Host::run()
{
    try
    {
        while (!p_->exit)
        {
            p_->exit = p_->update();
        }

        return 0;
    }
    catch (const std::exception& e)
    {
        DisplayLog::error(e.what());
    }
    catch (...)
    {
        DisplayLog::error("Unknown object thrown");
    }
    return 1;
}

}  // namespace haf::host

#include <host/include/main.hpp>

#include <memmanager/include/memmanager.hpp>
#include <host/include/host.hpp>
#include <lib/include/liblog.hpp>
#include <logger/include/log.hpp>
#include <host/include/apploader.hpp>

#include "../zoper_game/app.hpp"

namespace lib
{
    int libMain(int argc, char *argv[])
    {
        int result = -1;

        try
        {
            logger::init_log();

            installMemManager();

            {
                core::Host host(argc, argv);
                core::AppLoader app_loader;
                core::ManagedApp managed_app = app_loader.loadApp("zoper_game");
//                core::ManagedApp managed_app = app_loader.loadApp(createApp, destroyApp);
                host.setApplication(managed_app.app);
                int run_result(host.run());
                app_loader.unloadApp(managed_app);
                result = run_result;
            }
        }
        catch (std::exception e)
        {
            DisplayLog::error("std::Exception: ", e.what());
        }
        catch (...)
        {
            DisplayLog::info("Unexpected exception");
        }
        finishMemManager();
        logger::finish_log();

        return result;
    }
}

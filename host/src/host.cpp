#include <host/include/host.hpp>

#include <exception>
#include <hlog/include/hlog.hpp>
#include <hosted_app/include/iapp.hpp>
#include <haf/system/include/isystemcontroller.hpp>
#include <loader/include/loader.hpp>
#include <mtypes/include/parpar.hpp>
#include <mtypes/include/object.hpp>

using namespace mtps;

namespace
{
constexpr static const char HostVersion[]    = "1";
constexpr static const char HostSubversion[] = "2";
constexpr static const char HostPatch[]      = "0";
}  // namespace

namespace haf::sys
{
class SystemControllerLoader
{
public:
    using CreateSystemController_t  = ISystemController* (*)();
    using DestroySystemController_t = void (*)(ISystemController*);

    ~SystemControllerLoader()
    {
        destroy();
        loader::destroyLoader();
    }

    enum class ResultType
    {
        Success = 0,
        CannotCreateLoader,
        ObjectNotFound,
        CreateNotFound,
        DestroyNotFound,
        NoFunctionsFound
    };

    ResultType loadFunctions()
    {
        ResultType result{ResultType::Success};

        loader_ = loader::createLoader();
        if (!loader_)
        {
            return ResultType::CannotCreateLoader;
        }

        constexpr const char haf_library[] = "haf";
        if (loader_->loadModule(haf_library))
        {
            fp_haf_create_system_controller_ =
                static_cast<CreateSystemController_t>(
                    loader_->loadMethod(haf_library, "createSystemController"));

            if (!fp_haf_create_system_controller_)
            {
                result = ResultType::CreateNotFound;
            }

            fp_haf_destroy_system_controller_ =
                static_cast<DestroySystemController_t>(loader_->loadMethod(
                    haf_library, "destroySystemController"));

            if (!fp_haf_destroy_system_controller_)
            {
                result = (result == ResultType::CreateNotFound)
                    ? ResultType::NoFunctionsFound
                    : ResultType::DestroyNotFound;
            }
        }
        else
        {
            result = ResultType::ObjectNotFound;
        }
        return result;
    }

    bool create()
    {
        if (fp_haf_create_system_controller_ != nullptr)
        {
            system_controller_ = (*fp_haf_create_system_controller_)();
        }

        return (system_controller_ != nullptr);
    }

    void destroy()
    {
        if (system_controller_ != nullptr)
        {
            if (fp_haf_destroy_system_controller_)
            {
                (*fp_haf_destroy_system_controller_)(system_controller_);
            }
            system_controller_ = nullptr;
        }
        fp_haf_destroy_system_controller_ = nullptr;
    }

    rptr<ISystemController> systemController() noexcept
    {
        return system_controller_;
    }

    rptr<ISystemController const> systemController() const noexcept
    {
        return system_controller_;
    }

private:
    rptr<loader::Loader> loader_{nullptr};
    rptr<ISystemController> system_controller_{nullptr};
    CreateSystemController_t fp_haf_create_system_controller_;
    DestroySystemController_t fp_haf_destroy_system_controller_;
};

class Host::HostPrivate final
{
public:
    HostPrivate(const int argc, char const* const argv[]) :
        argc_{argc},
        argv_{argv},
        params_{parpar::create(argc, argv)},
        // Hardcoded default configuration
        // TODO
        configuration_{{}}
    {}

    ~HostPrivate() = default;

    bool parseCommandLineParameters()
    {
        if (params_.hasParameters())
        {
        }
        return true;
    }
    parpar::ParametersParser params_;

    Dictionary<str> configuration_;
    rptr<IApp> iapp_{nullptr};
    SystemControllerLoader system_loader_;

    rptr<ISystemController> systemController() noexcept
    {
        return system_loader_.systemController();
    }

    rptr<ISystemController const> systemController() const noexcept
    {
        return system_loader_.systemController();
    }

    int const argc_;
    char const* const* const argv_;
};

enum class Host::AppState : u8
{
    NotInitialized,
    ReadyToStart,
    Executing,
    ReadyToTerminate,
    Terminated
};

Host::Host(int argc, char* argv[]) :
    p_{muptr<HostPrivate>(argc, argv)}, app_state_{AppState::NotInitialized}
{
    DisplayLog::info("Starting HostController...");
    DisplayLog::info("Host version: ", HostVersion, ".", HostSubversion, ".",
                     HostPatch);
    DisplayLog::info("Parsing parameters...");
    p_->parseCommandLineParameters();
}

Host::~Host() = default;

bool Host::setApplication(rptr<IApp> iapp)
{
    LogAsserter::log_assert(iapp != nullptr, "Received nullptr Application");
    LogAsserter::log_assert(!p_->iapp_, "Application already set");

    if (!p_->iapp_ && iapp)
    {
        DisplayLog::info("Starting Registering app...");
        p_->iapp_ = iapp;
        DisplayLog::verbose("Starting new app...");
        app_state_ = AppState::ReadyToStart;
        return true;
    }
    return false;
}

str appDisplayNameAndVersion(const IApp& app)
{
    return make_str(app.getName(), "(", app.getVersion(), ".",
                    app.getSubVersion(), ".", app.getPatch(), ")");
}

bool Host::update()
{
    switch (app_state_)
    {
        case AppState::NotInitialized:
            break;
        case AppState::ReadyToStart:
        {
            DisplayLog::info("Starting initialization of new App...");
            app_state_ = AppState::Executing;
            p_->system_loader_.loadFunctions();
            p_->system_loader_.create();
            p_->systemController()->init(p_->iapp_);

            DisplayLog::info(appDisplayNameAndVersion(*(p_->iapp_)),
                             ": Starting execution...");
        }
        break;
        case AppState::Executing:
        {
            if (loopStep())
            {
                app_state_ = AppState::ReadyToTerminate;
                DisplayLog::info(appDisplayNameAndVersion(*(p_->iapp_)), ": ",
                                 " is now ready to terminate");
            }
            else if (app_state_ == AppState::ReadyToTerminate)
            {
                DisplayLog::info(appDisplayNameAndVersion(*(p_->iapp_)), ": ",
                                 " requested to terminate");
            }
        }
        break;
        case AppState::ReadyToTerminate:
            DisplayLog::info(appDisplayNameAndVersion(*(p_->iapp_)),
                             ": started termination");
            app_state_ = AppState::Terminated;
            p_->systemController()->terminate();
            p_->system_loader_.destroy();
            return true;
            break;
        case AppState::Terminated:
            return true;
            break;
        default:
            break;
    }
    return false;
}

int Host::run()
{
    try
    {
        while (!exit)
        {
            if (update())
            {
                exit = true;
            }
        }

        return 0;
    }
    catch (const std::exception& e)
    {
        DisplayLog::error(e.what());
    }
    return 1;
}

bool Host::loopStep()
{
    return p_->systemController()->runStep();
}

void Host::exitProgram()
{
    LogAsserter::log_assert(
        app_state_ == AppState::Executing,
        "Cannot terminate a program that is not in the executing state");
    app_state_ = AppState::ReadyToTerminate;
}
}  // namespace haf::sys

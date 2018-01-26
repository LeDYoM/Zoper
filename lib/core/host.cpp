#include "host.hpp"
#include "window.hpp"
#include "resourcemanager.hpp"
#include "randomizer.hpp"

#include <lib/scene/scenemanager.hpp>
#include <lib/core/events/eventmanager.hpp>
#include <lib/core/inputsystem.hpp>
#include <lib/backend/backendfactory.hpp>
#include <lib/backend/iwindow.hpp>
#include <lib/core/debugsystem.hpp>

#include <mtypes/include/log.hpp>

#include <parpar/include/parpar.hpp>
#include <algorithm>

#define LIB_VERSION_MAJOR 1
#define LIB_VERSION_MINOR 4
#define LIB_VERSION_PATCH 2

namespace lib
{
    namespace core
    {
        auto transformParams(int argc, char *argv[])
        {
            vector<str> temp;

            for (int i = 1; i<argc; ++i) {
                temp.push_back(argv[i]);
            }
            return temp;
        }

        enum class Host::AppState : u8
        {
            NotInitialized,
            ReadyToStart,
            Executing,
            ReadyToTerminate,
            Terminated
        };

        Host *Host::m_instance = nullptr;

        bool Host::createHost(int argc, char * argv[])
        {
            if (!m_instance) {
                m_instance = new Host(argc, argv);
                backend::BackendFactory::initilialize("bsfml");
                return true;
            }
            return false;
        }

        bool Host::destroyHost() noexcept
        {
            if (m_instance) {
                backend::BackendFactory::destroy();
                delete m_instance;
                m_instance = nullptr;
                return true;
            }
            return false;
        }

        Host::Host(int argc, char *argv[])
            : m_state{ AppState::NotInitialized }
        {
            logConstruct_NOPARAMS;
            log_release_info("Starting HostController...");
            log_release_info("LIB version: ", LIB_VERSION_MAJOR,".", LIB_VERSION_MINOR,".", LIB_VERSION_PATCH);
            log_release_info("Parsing parameters...");
            m_params = transformParams(argc, argv);
        }

        Host::~Host()
        {
            logDestruct_NOPARAMS;
        }

        bool Host::setApplication(uptr<IApp> iapp)
        {
            if (!m_iapp && iapp) {
                std::swap(m_iapp, iapp);
                log_debug_info("Starting app ", appId(), "...");
                m_state = AppState::ReadyToStart;
                return true;
            }
            return false;
        }

        bool Host::update()
        {
            switch (m_state)
            {
            case AppState::NotInitialized:
                break;
            case AppState::ReadyToStart:
            {
                log_debug_info(appId(), ": Starting initialization...");
                m_state = AppState::Executing;

                m_inputSystem = muptr<input::InputSystem>();
                m_randomizer = muptr<Randomizer>();
                m_eventManager = muptr<EventManager>();
                m_window = muptr<Window>(m_iapp->getAppDescriptor().wcp);
                m_sceneManager = muptr<scene::SceneManager>(*m_window);
                m_resourceManager = muptr<core::ResourceManager>();
                m_debugSystem = muptr<DebugSystem>();

                m_iapp->onInit();
                log_debug_info(appId(), ": Starting execution...");
            }
            break;
            case AppState::Executing:
            {
                if (loopStep()) {
                    m_state = AppState::ReadyToTerminate;
                    log_debug_info(appId(), ": ", " is now ready to terminate");
                }
                else if (m_state == AppState::ReadyToTerminate) {
                    log_debug_info(appId(), ": ", " requested to terminate");
                }
            }
            break;
            case AppState::ReadyToTerminate:
                log_debug_info(appId(), ": started termination");
                m_sceneManager->finish();
                m_state = AppState::Terminated;
//				m_iapp->onFinish();
                m_debugSystem = nullptr;
                m_sceneManager = nullptr;
                m_window = nullptr;
                m_resourceManager = nullptr;
                m_eventManager = nullptr;
                m_randomizer = nullptr;
                m_inputSystem = nullptr;
                m_params.clear();
                log_debug_info(appId(), ": terminated");
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
            while (!exit) {
                const bool terminated{ update() };
                if (terminated) {
                    m_iapp.reset();
                    exit = true;
                }
            }

            if (!m_iapp) {
                log_release_info("App destroyed. Exiting normally");
            }
            return 0;
        }

        bool Host::loopStep()
        {
            const bool windowWants2Close{ m_window->preLoop() };
            m_eventManager->update();
            m_inputSystem->preUpdate();
            m_sceneManager->update();

            __ASSERT(m_currentScene || m_nextScene, "Current scene and nextscene cannot be nullptr at same time");

            m_window->postLoop();
            m_inputSystem->postUpdate();
            return windowWants2Close;
        }

        void Host::exitProgram()
        {
            __ASSERT(m_state == AppState::Executing, "Cannot terminate a program that is not in the executing state");
            m_state = AppState::ReadyToTerminate;
        }

        const str Host::appId() const
        {
            if (m_iapp) {
                const auto &cAppDescriptor(m_iapp->getAppDescriptor());
                return make_str(cAppDescriptor.Name, ":", cAppDescriptor.Version, ".", cAppDescriptor.SubVersion, ".", cAppDescriptor.Patch);
            }
            return "NoApp:0.0.0";
        }
    }
}

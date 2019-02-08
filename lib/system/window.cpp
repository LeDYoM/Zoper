#include "window.hpp"

#include <lib/include/core/log.hpp>
#include <lib/include/core/timer.hpp>
#include <lib/include/backend/irendertarget.hpp>
#include <lib/include/iapp.hpp>

#include <lib/system/inputsystem.hpp>
#include <lib/system/rendertarget.hpp>
#include <lib/core/backendfactory.hpp>

#include <lib/scene/texture.hpp>
#include <lib/scene/renderdata.hpp>
#include <lib/scene/vertexarray.hpp>

namespace lib::core
{
    struct Window::WindowPrivate final
    {
        Timer globalClock;
        u64 lastTimeFps{ 0 };
        s32 lastFps{ 0 };
        s32 currentFps{ 0 };
        backend::IWindow* m_backendWindow{nullptr};
        sptr<RenderTarget> m_renderTarget{nullptr};
    };

    Window::Window(const WindowCreationParams &wcp)
        : AppService{ },
        m_wPrivate{ muptr<WindowPrivate>() }, m_title{ wcp.windowTitle }
    {
        create(wcp);
    }

    Window::~Window() = default;

    sptr<RenderTarget> Window::renderTarget()
    {
        return m_wPrivate->m_renderTarget;
    }

    const sptr<RenderTarget> Window::renderTarget() const
    {
        return m_wPrivate->m_renderTarget;
    }

    backend::IInputDriver* Window::inputDriver()
    {
        return m_wPrivate->m_backendWindow->inputDriver();
    }

    const backend::IInputDriver* Window::inputDriver() const
    {
        return m_wPrivate->m_backendWindow->inputDriver();
    }

    void Window::create(const WindowCreationParams &wcp)
    {
        log_debug_info("Going to create Window");
        log_debug_info("Resolution:", wcp.width, "x", wcp.height ,"x", wcp.bpp);
        log_debug_info("Fullscreen:" , wcp.fullScreen);
        log_debug_info("Antialiasing:", wcp.antialiasing);

        assert_release(!m_wPrivate->m_backendWindow, "Cannot create window twice");
        log_debug_info("Creating window...");

        // Create window object
        m_wPrivate->m_backendWindow = backend::BackendFactory::instance()->getOrCreateWindow();
//            m_wPrivate->m_renderTarget = dynamic_cast<backend::IRenderTarget*>(m_wPrivate->m_backendWindow);
        log_debug_info("Window created");
        log_debug_info("Registering for view changes...");
        backend::IWindow &bw(*m_wPrivate->m_backendWindow);

        // Create physical window
        if (bw.createWindow(wcp))
        {
            log_debug_info("Hardware window created...");
            // If window created successfully, extract the render target
            // associated with the window.
            m_wPrivate->m_renderTarget = msptr<RenderTarget>(
                    m_wPrivate->m_backendWindow->renderTarget());

            // Also extract the input driver
//            m_wPrivate->input_driver_ = sptr<backend::IInputDriver>(
//                m_wPrivate->m_backendWindow->inputDriver());
        }
        log_debug_info("Window creation completed");
    }

    bool Window::preLoop()
    {
        backend::IWindow &bw(*m_wPrivate->m_backendWindow);
        auto eMs = m_wPrivate->globalClock.ellapsed().asMilliSeconds();
        if ((eMs - m_wPrivate->lastTimeFps) > 1000) {
            m_wPrivate->lastTimeFps = eMs;
            m_wPrivate->lastFps = m_wPrivate->currentFps;
            m_wPrivate->currentFps = 0;
            str wtitle(m_title + " FPS:" + str(m_wPrivate->lastFps));
            bw.setWindowTitle(wtitle);
        }
        ++(m_wPrivate->currentFps);
        m_wPrivate->m_renderTarget->clear();

        return bw.processEvents();
    }

    void Window::postLoop()
    {
        m_wPrivate->m_backendWindow->display();
    }

    void Window::onCreate()
    {
        log_debug_info("Window created");
    }

    void Window::onDestroy()
    {
        log_debug_info("Going to close Window");
        m_wPrivate->m_backendWindow->closeWindow();
        log_debug_info("Window closed");
    }
}
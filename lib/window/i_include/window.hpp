#pragma once

#ifndef LIB_WINDOW_WINDOW_INCLUDE_HPP
#define LIB_WINDOW_WINDOW_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <lib/system/include/appservice.hpp>

namespace lib::input
{
    class InputDriver;
}

namespace lib::sys
{
class RenderTarget;
class SystemProvider;

/**
 * @brief Class containing elements to manage a window.
 * This class system is intended to be used internally to provide
 * functionallity related to a window.
 */
class Window final : public AppService
{
public:
    /**
     * @brief Construct a new Window object
     * 
     * @param system_provider The system provider
     */
    Window(sys::SystemProvider &system_provider);

    /**
     * @brief Destroy the Window object
     * 
     */
    ~Window() override;

    /**
     * @brief Method to be executed before starting a cycle of the system
     * 
     * @return true The window has requested to exit
     * @return false The window did not request to exit
     */
    bool preLoop();

    /**
     * @brief Method to be executed after the cycle of a system
     */
    void postLoop();
    void onCreate();
    void onDestroy();

    sptr<RenderTarget> renderTarget();
    const sptr<RenderTarget> renderTarget() const;

    sptr<input::InputDriver> inputDriver();
    const sptr<input::InputDriver> inputDriver() const;

private:
    void create();
    struct WindowPrivate;
    uptr<WindowPrivate> priv_;
};
} // namespace lib::sys

#endif

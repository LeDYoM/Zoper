#include <system/i_include/systemcontroller.hpp>
#include <hlog/include/hlog.hpp>

#ifdef _WIN32
#define HAF_SYSTEM_API extern "C" __declspec(dllexport)
#else
#define HAF_SYSTEM_API extern "C" __attribute__((visibility("default")))
#endif

HAF_SYSTEM_API ISystemController* createSystemController()
{
    return new haf::sys::SystemController();
}

HAF_SYSTEM_API void destroySystemController(ISystemController* isp)
{
    haf::LogAsserter::log_assert(
        isp != nullptr, "nullptr parameter to destroySystemController");
    auto* system_controller = dynamic_cast<haf::sys::SystemController*>(isp);
    haf::LogAsserter::log_assert(system_controller != nullptr,
                                 "Invalid type of system controller");
    delete system_controller;
}

#ifndef HAF_CORE_SYSTEM_TIMESYSTEM_INCLUDE_HPP
#define HAF_CORE_SYSTEM_TIMESYSTEM_INCLUDE_HPP

#include "system/systembase.hpp"

#include <htypes/include/types.hpp>
#include <haf/include/time/timepoint.hpp>

namespace haf::sys
{
/**
 * @brief System class to manage everything related to time.
 */
class TimeSystem final : public SystemBase
{
public:
    /**
     * @brief Construct a new Time System object
     * @param system_provider The system provider instance
     */
    TimeSystem(sys::ISystemProvider& system_provider);
    ~TimeSystem();

    time::TimePoint timeSinceStart() const;
    time::TimePoint now() const;

    void startFrame();
    void endFrame();

    void setAcceleration(htps::f32 const acceleration);

private:
    struct TimeSystemPrivate;
    htps::uptr<TimeSystemPrivate> priv_;
};
}  // namespace haf::sys

#endif
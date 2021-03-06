#include <time/i_include/timesystem.hpp>

#include <chrono>

#include <hlog/include/hlog.hpp>
#include <haf/time/include/timepoint.hpp>

using namespace mtps;

namespace haf::sys
{
using namespace time;

namespace
{
using clock_t = std::chrono::high_resolution_clock;

TimePoint timepoint_global_now()
{
    return TimePoint{
        static_cast<u64>(clock_t::now().time_since_epoch().count())};
}
}  // namespace

struct TimeSystem::TimeSystemPrivate final
{
    TimeSystemPrivate() : globalStart_{timepoint_global_now()}
    {
        DisplayLog::info("TimeSystem started at: ", globalStart_.seconds());
    }

    ~TimeSystemPrivate() {}

    TimePoint timeSinceStart() const
    {
        return (timepoint_global_now() - globalStart_)
#ifdef HAF_ALLOW_ACCELERATION
            * acceleration_;
#endif
        ;
    }

    void updateStartFrameTime() { last_start_frame_ = timepoint_global_now(); }

    void updateEndFrameTime() { last_end_frame_ = timepoint_global_now(); }

#ifdef HAF_ALLOW_ACCELERATION
    void setAcceleration(const f32 acceleration) noexcept
    {
        acceleration_ = acceleration;
    }
#endif
private:
    TimePoint globalStart_;
    TimePoint last_start_frame_{0U};
    TimePoint last_end_frame_{0U};
#ifdef HAF_ALLOW_ACCELERATION
    f32 acceleration_ = 1.0f;
#endif
};

TimeSystem::TimeSystem(sys::ISystemProvider& system_provider) :
    SystemBase{system_provider}, priv_{muptr<TimeSystemPrivate>()}
{}

TimeSystem::~TimeSystem() = default;

TimePoint TimeSystem::timeSinceStart() const
{
    return priv_->timeSinceStart();
}

TimePoint TimeSystem::now() const
{
    return priv_->timeSinceStart();
}

void TimeSystem::setAcceleration(const f32 acceleration)
{
#ifdef HAF_ALLOW_ACCELERATION
    priv_->setAcceleration(acceleration);
#endif
}

void TimeSystem::startFrame()
{
    priv_->updateStartFrameTime();
}

void TimeSystem::endFrame()
{
    priv_->updateEndFrameTime();
}
}  // namespace haf::sys

#ifndef HAF_SCENE_IANIMATION_INCLUDE_HPP
#define HAF_SCENE_IANIMATION_INCLUDE_HPP

#include <haf/include/time/timepoint.hpp>
#include <haf/include/time/timeview.hpp>
#include <haf/include/time/timer.hpp>

#include <htypes/include/types.hpp>
#include <htypes/include/properties.hpp>
#include <htypes/include/function.hpp>

namespace haf::scene
{
/**
 * @brief Class representing an animation. The animation will be updated when
 *  the animate() method is called.
 */
class Animation
{
public:
    /**
     * @brief Type to represent an animation
     */
    using ActionFunc = htps::function<void()>;

    enum class AnimationDirection : htps::u8
    {
        Forward = 0U,
        Backward
    };

    /**
     * @brief Construct a new Animation object
     *
     * @param timer @b Timer to use to animate
     * @param duration @b time::TimePoint containing the time the animation is
     *  going to last
     * @param animation_direction @b AnimationDirection Direction of the
     *  animation
     * @param endAction Functor containing an action to perform when the
     *  animation finishes
     */
    Animation(htps::uptr<time::Timer> timer,
              time::TimePoint duration,
              const AnimationDirection animation_direction,
              ActionFunc endAction = {}) noexcept;

    /**
     * @brief Performs one step on the animation
     *
     * @return true If the animation should continue
     * @return false The animation should finish
     */
    virtual bool animate();

    /**
     * @brief Destroy the Animation object
     */
    virtual ~Animation();

    /**
     * @brief Execute an action at the end of the animation
     */
    void executeEndAction();

protected:
    htps::f32 delta() const noexcept { return delta_; }

private:
    htps::uptr<time::Timer> timer_;
    time::TimePoint m_duration;
    time::TimePoint m_currentTime;
    ActionFunc m_endAction;
    AnimationDirection animation_direction_;
    htps::f32 raw_delta_;
    htps::f32 delta_;

    htps::f32 postProcessDelta(const htps::f32 delta);
};

}  // namespace haf::scene

#endif
#ifndef HAF_TIME_TIMER_TYPE_INCLUDE_HPP
#define HAF_TIME_TIMER_TYPE_INCLUDE_HPP

#include <htypes/include/types.hpp>

namespace haf::time
{

/**
 * @brief Enum class containing the different types of timer that can be created
 *
 */
enum class TimerType : htps::u8
{
    /**
     * @brief The @b Timer will be trigger continuosly until stopped.
     *
     */
    Continuous,

    /**
     * @brief The @b Timer will be triggered once.
     *
     */
    OneShot
};
}  // namespace haf::time

#endif

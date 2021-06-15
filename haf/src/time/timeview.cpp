#include <haf/include/time/timeview.hpp>
#include "timesystem.hpp"
#include "system/get_system.hpp"

#include <haf/include/scene/scenenode.hpp>


namespace haf::time
{
TimePoint TimeView::now() const
{

    return sys::getSystem<sys::TimeSystem>(attachedNode()).now();
}

}  // namespace haf::time
#include <haf/include/system/interfaceaccess.hpp>
#include "system/get_system.hpp"

namespace haf::sys
{
#define GET_INTERFACE_IMP(INTCLASS, SYSTEMCLASS)    \
template <> \
INTCLASS& getInterface(SystemAccess& system_access) \
{   \
    return static_cast<INTCLASS&>(  \
        getSystem<SYSTEMCLASS>(&system_access));    \
}   \
    \
template <> \
INTCLASS const& getInterface(   \
    SystemAccess const& system_access)  \
{   \
    return static_cast<INTCLASS const&>(    \
        getSystem<SYSTEMCLASS>(&system_access));    \
}

}
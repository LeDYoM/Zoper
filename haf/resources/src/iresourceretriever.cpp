#include <system/i_include/interfaceaccess_imp.hpp>

#include <haf/system/include/interfaceaccess.hpp>
#include <system/i_include/get_system.hpp>

#include <haf/resources/include/iresourceretriever.hpp>
#include <resources/i_include/resourcemanager.hpp>

namespace haf::sys
{
GET_INTERFACE_IMP(res::IResourceRetriever, sys::ResourceManager)

}  // namespace haf::sys

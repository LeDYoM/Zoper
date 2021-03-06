#include <haf/resources/include/resourceview.hpp>
#include <haf/resources/i_include/resourcemanager.hpp>

#include <hlog/include/hlog.hpp>
#include <system/i_include/get_system.hpp>

using namespace mtps;

namespace haf::res
{
void ResourceView::onAttached()
{
    retriever_ = &(sys::getSystem<sys::ResourceManager>(attachedNode()));
    LogAsserter::log_assert(retriever_ != nullptr,
                            "Invalid IResourceRetrieved");
}

sptr<ITTFont> ResourceView::getTTFont(const str& rid) const
{
    return retriever_->getTTFont(rid);
}

sptr<ITexture> ResourceView::getTexture(const str& rid) const
{
    return retriever_->getTexture(rid);
}

sptr<IShader> ResourceView::getShader(const str& rid) const
{
    return retriever_->getShader(rid);
}

sptr<IFont> ResourceView::getBMPFont(const str& rid) const
{
    return retriever_->getBMPFont(rid);
}

}  // namespace haf::res

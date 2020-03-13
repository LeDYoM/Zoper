#pragma once

#ifndef LIB_RESOURCE_VIEW_DATA_WRAPPER_INCLUDE_HPP
#define LIB_RESOURCE_VIEW_DATA_WRAPPER_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <lib/scene/include/idatawrapper.hpp>
#include <lib/resources/include/iresourceretriever.hpp>

namespace lib::scene
{
/// Component to provide access to resources.
class ResourceView final : public sys::IDataWrapper, public IResourceRetriever
{
public:
    void onAttached() override;
    sptr<scene::ITTFont> getTTFont(const str &rid) override;
    sptr<scene::ITexture> getTexture(const str &rid) override;
    sptr<scene::IShader> getShader(const str &rid) override;
    sptr<scene::IFont> getBMPFont(const str &rid) override;
private:
    rptr<IResourceRetriever> retriever_;
};
} // namespace lib::scene

#endif

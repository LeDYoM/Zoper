#pragma once

#ifndef HAF_SYSTEM_SHARED_DATA_INCLUDE_HPP
#define HAF_SYSTEM_SHARED_DATA_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <system/i_include/appservice.hpp>
#include <haf/shareddata/include/ishareable.hpp>

namespace haf::sys
{
/**
 * @brief Thisis a system whose only purpose is to store data shared
 * between different objects on the system.
 */
class SharedDataSystem final : public AppService
{
public:
    using AppService::AppService;

    /**
     * @brief Destroy the Shared Data System object
     */
    ~SharedDataSystem() override;

    /**
     * @brief Store a shareable object. The shared data system should be
     * empty before this call. You can check the empty state with isEmpty
     * before calling it.
     * @see isEmpty
     * 
     * @param data Object derived from @b shdata::IShareable to store
     * @return true The object has been stored.
     * @return false Cannot store the object.
     */
    bool store(mtps::uptr<shdata::IShareable> data) noexcept;

    /**
     * @brief Retrieve the stored object in the SharedData system.
     * 
     * @return mtps::uptr<shdata::IShareable> Object stored in the system.
     * It will be nullptr if no object was stored.
     */
    [[nodiscard]] mtps::uptr<shdata::IShareable> retrieve() noexcept;

    /**
     * @brief Ask if the system has an object stored
     * 
     * @return true There is an object stored
     * @return false No object stored currently
     */
    bool isEmpty() const noexcept;

    /**
     * @brief Make the Shared object empty.
     * 
     * @return true It was not empty. Now it is.
     * @return false It was empty already, no actions performed.
     */
    bool makeEmpty();

    mtps::uptr<shdata::IShareable> const& view() const noexcept;

private:
    mtps::uptr<shdata::IShareable> data_;
};

}  // namespace haf::sys

#endif
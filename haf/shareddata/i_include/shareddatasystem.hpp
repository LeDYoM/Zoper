#ifndef HAF_SYSTEM_SHARED_DATA_INCLUDE_HPP
#define HAF_SYSTEM_SHARED_DATA_INCLUDE_HPP

#include <htypes/include/types.hpp>
#include <system/i_include/systembase.hpp>
#include <haf/shareddata/include/ishareable.hpp>
#include <haf/shareddata/include/shareddata.hpp>

namespace haf::shdata
{
class Address;
}

namespace haf::sys
{
/**
 * @brief Thisis a system whose only purpose is to store data shared
 * between different objects on the system.
 */
class SharedDataSystem final : public SystemBase, public shdata::SharedData
{
public:
    using SystemBase::SystemBase;

    /**
     * @brief Destroy the Shared Data System object
     */
    ~SharedDataSystem();

    /**
     * @brief Store a shareable object. T. If there is already an object
     * at this address, it will be overwritten.
     *
     * @param address Adress in the tree to store the data.
     * @param data Object derived from @b shdata::IShareable to store
     * @return true The object has been stored.
     * @return false if cannot store the object.
     */
    bool store(shdata::Address const& address, shdata::IShareable const& data) override;

    /**
     * @brief Retrieve the stored object in the SharedData system.
     *
     * @param address Adress in the tree where to retrieve the object from.
     * @param data Reference to where to store the result.
     * @return If successful.
     */
    [[nodiscard]] bool retrieve(shdata::Address const& address,
                                shdata::IShareable& data) override;

    /**
     * @brief Ask if the system has an object stored
     *
     * @return true There is an object stored
     * @return false No object stored currently
     */
    bool isEmpty() const noexcept override;

    /**
     * @brief Make the Shared object empty.
     *
     * @return true It was not empty. Now it is.
     * @return false It was empty already, no actions performed.
     */
    bool makeEmpty() override;

private:
    htps::Object data_object_;
};

}  // namespace haf::sys

#endif

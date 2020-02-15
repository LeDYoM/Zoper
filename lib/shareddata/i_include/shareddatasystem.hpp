#pragma once

#ifndef LIB_SYSTEM_SHARED_DATA_INCLUDE_HPP
#define LIB_SYSTEM_SHARED_DATA_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <lib/system/appservice.hpp>
#include <lib/shareddata/include/ishareable.hpp>

namespace lib::sys
{

/// SharedDataSystem is a system whose only purpose is to store
/// data shared between different objects.
class SharedDataSystem final : public AppService
{
public:
    SharedDataSystem();
    ~SharedDataSystem() override;

    void store(uptr<shdata::IShareable> data);
    [[nodiscard]] uptr<shdata::IShareable> retrieve();

    bool isEmpty() const noexcept
    {
        return data_ == nullptr;
    }

private:
    uptr<shdata::IShareable> data_;
};
} // namespace lib::sys

#endif

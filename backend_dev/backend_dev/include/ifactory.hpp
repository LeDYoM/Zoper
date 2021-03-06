#pragma once

#ifndef HAF_BACKEND_IFACTORY_INCLUDE_HPP
#define HAF_BACKEND_IFACTORY_INCLUDE_HPP

namespace haf::backend
{
template <typename T>
class IFactoryOf
{
public:
    using Interface = T;

    virtual T* const create() noexcept = 0;
    virtual bool destroy(T*) noexcept  = 0;
    virtual ~IFactoryOf() {}
};
}  // namespace haf::backend

#endif

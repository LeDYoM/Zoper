#pragma once

#ifndef HAF_BACKEND_CLIENT_DEFAULT_BACKEND_MANAGER_INLUDE_HPP
#define HAF_BACKEND_CLIENT_DEFAULT_BACKEND_MANAGER_INLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/vector.hpp>
#include <backend_client/include/ibackendmanager.hpp>
#include <backend_client/include/autoregisterfactory.hpp>
#include <backend_client/include/backend_client_exports.hpp>

namespace haf::backend::client
{
class IBackendRegister;

/**
 * @brief This class is a default (or an example if one wants) of how to
 * inherit from @b IBackendManager and create your new one. For normal
 * use cases, this one can be directoly used. Only create your own if you
 * need very special behavior.
 */
class BC_API DefaultBackendManager : public IBackendManager
{
public:
    /**
     * @brief Construct a new Default Backend Manager object
     */
    DefaultBackendManager();

    /**
     * @brief Destroy the Default Backend Manager object
     */
    ~DefaultBackendManager() override;

    void setFactories(
        mtps::rptr<IBackendRegister> const backend_register) override final;

    void resetFactories(
        mtps::rptr<IBackendRegister> const backend_register) override final;

    template <typename T>
    void createFactoryOfFactories()
    {
        auto f(mtps::muptr<AutoRegisterFactory<typename T::Interface>>());
        f.get()->create(mtps::muptr<T>());
        pushFactory(std::move(f));
    }

private:
    void destroy() override final;

    void pushFactory(mtps::uptr<IAutoRegisterFactory> f);
    struct DefaultBackendManagerPrivate;
    mtps::uptr<DefaultBackendManagerPrivate> priv_;
};
}  // namespace haf::backend::client

#endif

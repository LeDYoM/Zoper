#pragma once

#ifndef LIB_BACKEND_INTERFACES_INLUDE_HPP
#define LIB_BACKEND_INTERFACES_INLUDE_HPP

#include "../iwindow.hpp"
#include "../iresourcefactories.hpp"
#include "ibackendregister.hpp"
#include "autoregisterfactory.hpp"
#include "ibackendmanager.hpp"
#include "factory.hpp"

namespace lib::backend::client
{
    template <typename T>
    void createFactoryOfFactories(lib::vector<lib::uptr<IAutoRegisterFactory>> &factories)
    {
        auto f(lib::muptr<AutoRegisterFactory<T::Interface>>());
        f.get()->create(lib::muptr<T>());
        factories.push_back(std::move(f));
    }

    struct DefaultBackendManager : IBackendManager
    {
        vector<uptr<IAutoRegisterFactory>> factories;

        void setFactories(IBackendRegister*const backend_register) override final
        {
            for (const auto &factory : factories)
            {
                factory.get()->setFactory(backend_register);
            }
        }

        void resetFactories(lib::backend::IBackendRegister*const backend_register) override final
        {
            for (const auto &factory : factories)
            {
                factory.get()->resetFactory(backend_register);
            }
        }

        void destroy()  override final
        {
            for (const auto &factory : factories)
            {
                factory.get()->destroy();
            }
        }

        ~DefaultBackendManager() override
        {
            destroy();
        }
    };

    template <typename T>
    inline bool default_init_function(T** backend_manager, IBackendRegister*const ibackend_register)
    {
        if (!(*backend_manager))
        {
            *backend_manager = new BackendManager;
            (*backend_manager)->create();

            (*backend_manager)->setFactories(ibackend_register);
            return true;
        }
        return false;
    }

    template <typename T>
    inline bool default_finish_function(T** backend_manager,IBackendRegister*const ibackend_register)
    {
        if (*backend_manager)
        {
            (*backend_manager)->resetFactories(ibackend_register);
            (*backend_manager)->destroy();
            delete (*backend_manager);
            (*backend_manager) = nullptr;
            return true;
        }

        return false;
    }
}

using p_initLib = bool (*)(lib::backend::IBackendRegister* const);
using p_finishLib = bool (*)(lib::backend::IBackendRegister* const);

#endif

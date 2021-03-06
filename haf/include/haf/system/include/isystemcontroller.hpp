#pragma once

#ifndef HAF_ISYSTEMCONTROLLER_INTERFACE_INCLUDE_HPP
#define HAF_ISYSTEMCONTROLLER_INTERFACE_INCLUDE_HPP

#include <mtypes/include/types.hpp>

namespace haf
{
class IApp;
}

class ISystemController
{
public:
    virtual ~ISystemController() {}

    virtual void init(mtps::rptr<haf::IApp> iapp,
                      int const argc,
                      char const* const argv[]) = 0;
    virtual bool runStep()                      = 0;
    virtual void terminate()                    = 0;
};

#endif

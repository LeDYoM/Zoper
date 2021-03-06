#pragma once

#ifndef LOADER_INCLUDE_HPP
#define LOADER_INCLUDE_HPP

#include "loader_export.hpp"

namespace loader
{
class Loader
{
public:
    Loader();
    ~Loader();

    LOADER_API void* loadModule(const char* fileName);
    LOADER_API void* loadMethod(const char* fileName, const char* methodName);
    LOADER_API bool unloadModule(const char* fileName);

private:
    class LoaderPrivate;
    LoaderPrivate* m_private;
};

LOADER_API Loader* createLoader();
LOADER_API void destroyLoader();
}  // namespace loader

#endif

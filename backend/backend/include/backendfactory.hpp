#pragma once

#ifndef LIB_BACKEND_FACTORY_HPP
#define LIB_BACKEND_FACTORY_HPP

#include <backend_dev/include/iresourcefactories.hpp>

namespace lib::backend
{
class BackendRegister;
class IWindow;
class IWindowProviderInfo;

class BackendFactory final
{
public:
    BackendFactory();
    ~BackendFactory();

    IWindow *getOrCreateWindow();
    IWindowProviderInfo *getWindowProviderInfo() const noexcept;
    ITextureFactory *getTextureFactory() const noexcept;
    ITTFontFactory *getTTFontFactory() const noexcept;
    IShaderFactory *getShaderFactory() const noexcept;
    IBMPFontFactory *getBMPFontFactory() const noexcept;
    IWindowProviderInfo &windowProviderInfo() const;
    ITextureFactory &textureFactory() const;
    ITTFontFactory &ttfontFactory() const;
    IShaderFactory &shaderFactory() const;
    IBMPFontFactory &bmpFontFactory() const;

private:
    uptr<BackendRegister> backend_register_;
    IWindowProviderInfo *m_windowProviderInfo{nullptr};
    IWindow *m_window{nullptr};
    ITextureFactory *m_textureFactory{nullptr};
    ITTFontFactory *m_ttfontFactory{nullptr};
    IShaderFactory *m_shaderFactory{nullptr};
    IBMPFontFactory *m_bmpFontFactory{nullptr};
};
} // namespace lib::backend

#endif

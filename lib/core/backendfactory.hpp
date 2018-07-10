#pragma once

#ifndef LIB_BACKEND_FACTORY_HPP
#define LIB_BACKEND_FACTORY_HPP

#include <lib/include/backend/iwindow.hpp>
#include <lib/include/backend/itexturefactory.hpp>
#include <lib/include/backend/ittfontfactory.hpp>

namespace lib::backend
{
	class BackendFactory final
	{
	public:
		static bool initilialize(const str&file);
		static bool destroy();
		sptr<IWindowProviderInfo> getWindowProviderInfo();
		sptr<IWindow> getOrCreateWindow();
		sptr<ITextureFactory> getTextureFactory();
		sptr<ITTFontFactory> getTTFontFactory();
		static inline BackendFactory *const instance() noexcept { return m_instance; }
	private:
		BackendFactory();
		~BackendFactory();
		sptr<IWindowProviderInfo> m_windowProviderInfo;
		sptr<IWindow> m_window;
		sptr<ITextureFactory> m_textureFactory;
		sptr<ITTFontFactory> m_ttfontFactory;
		static BackendFactory *m_instance;
	};

	static inline BackendFactory &backendFactory() { return *(BackendFactory::instance()); }
	static inline ITextureFactory &textureFactory() { return *(BackendFactory::instance()->getTextureFactory()); }
	static inline ITTFontFactory &ttfontFactory() { return *(BackendFactory::instance()->getTTFontFactory()); }
}

#endif
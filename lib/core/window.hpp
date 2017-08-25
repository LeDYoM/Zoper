#ifndef __LIB_WINDOW_HPP__
#define __LIB_WINDOW_HPP__

#include <mtypes/include/types.hpp>
#include "appservice.hpp"
#include <lib/backend/iwindow.hpp>
#include <lib/include/key.hpp>
#include <mtypes/include/properties.hpp>

namespace lib
{
	struct WindowCreationParams;
	namespace scene
	{
		class VertexArray;
		class RenderData;
	}

	namespace core
	{
		class Window : public AppService
		{
		public:
			Window(const WindowCreationParams &wcp);
			~Window();
			bool preLoop();
			void postLoop();
			virtual void onCreate();
			virtual void onDestroy();

			Property<Rectf32> viewPort;
			Property<Rectf32> viewRect;

			void draw(const scene::RenderData &renderData);

			bool arePendingKeyPresses() const;
			bool arePendingKeyReleases() const;

			input::Key popKeyPress();
			input::Key popKeyRelease();

		private:
			void create(const WindowCreationParams &wcp);
			struct WindowPrivate;
			uptr<WindowPrivate> m_wPrivate;
			str m_title;
		};
	}
}

#endif

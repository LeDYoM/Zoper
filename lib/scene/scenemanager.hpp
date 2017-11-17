#pragma once

#ifndef LIB_SCENE_SCENEMANAGER_INCLUDE_HPP__
#define LIB_SCENE_SCENEMANAGER_INCLUDE_HPP__

#include <mtypes/include/types.hpp>
#include <mtypes/include/str.hpp>
#include <mtypes/include/properties.hpp>
#include <mtypes/include/function.hpp>
#include <lib/scene/components/componentcontainer.hpp>
#include <lib/scene/components/statescontroller.hpp>
#include <lib/core/appservice.hpp>
#include <lib/core/window.hpp>

namespace lib
{
	namespace core
	{
		class Host;
		class Window;
	}
	namespace scene
	{
		class Scene;
		using SceneDirectorType = function<sptr<Scene>(sptr<Scene>)>;

		class SceneManager : public AppService
		{
		public:
			SceneManager(core::Window &);
			~SceneManager();

			void start(sptr<Scene> scene);
			void terminateScene();
			void setSceneDirector(SceneDirectorType sceneDirector);

			void update();

			void finish();

			inline const Rectf32 &viewPort() const noexcept { return m_parentWindow.viewPort(); }
			inline void setViewPort(Rectf32 vp) noexcept { m_parentWindow.viewPort = std::move(vp); }
			inline const Rectf32 &viewRect() const noexcept { return m_parentWindow.viewRect(); }
			inline void setViewRect(Rectf32 vr) noexcept { m_parentWindow.viewRect = std::move(vr); }

		private:
			ComponentContainer m_componentContainer;
			sptr<StatesController<sptr<Scene>>> m_statesController;
			SceneDirectorType m_sceneDirector;
			core::Window &m_parentWindow;
		};
	}
}

#endif

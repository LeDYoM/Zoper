#include "scene.hpp"
#include "../log.hpp"
#include "scenemanager.hpp"
#include "../core/window.hpp"
#include "draw/renderizable.hpp"

namespace lib
{
	namespace scn
	{
		Scene::Scene(const std::string &_name) : RenderGroup{_name,nullptr}
		{
			LOG_CONSTRUCT("Name: " << name());
		}

		Scene::~Scene()
		{
			LOG_DESTRUCT("Name: "<<name());
		}

		void Scene::updateView()
		{
			p_scnManager->parentWindow()->setView(*p_view.get());
			LOG_DEBUG("Scene view set to: center: " << p_view->getCenter().x << "," << p_view->getCenter().y << " and size: " << p_view->getSize().x << "," << p_view->getSize().y);
		}

		sf::View *const Scene::getView() const
		{
			return p_view.get();
		}

		const vector2df Scene::getCoordinatesToCenter(const sf::FloatRect &coordinates) const
		{
			return{ getCenterCoordinates().x - (coordinates.width / 2.0f), getCenterCoordinates().y - (coordinates.height / 2.0f) };
		}

		const vector2df Scene::getCenterCoordinates() const
		{
			return{ p_view->getSize().x / 2.0f, p_view->getSize().y / 2.0f };
		}

		void Scene::privateOnInit()
		{
			LOG_DEBUG("Initializing scene " << name());
			p_view = uptr<sf::View>(new sf::View(p_scnManager->parentWindow()->getView()));

			onInit();
		}

		void Scene::privateOnDeinit()
		{
			LOG_DEBUG("Deinitializing scene " << name());
			onDeinit();
		}

		void Scene::privateOnEnterScene()
		{
			LOG_DEBUG("Entered in scene " << name());
			auto sceneSize = getDefaultSizeView();
			p_view->setSize(sceneSize.x, sceneSize.y);
			p_view->setCenter(sceneSize.x / 2, sceneSize.y / 2);
			updateView();

			LOG_DEBUG("Scene view set to: center: " << p_view->getCenter().x << "," << p_view->getCenter().y << " and size: " << p_view->getSize().x << "," << p_view->getSize().y);

			clock.restart();
			onEnterScene();
		}

		void Scene::privateOnExitScene()
		{
			onExitScene();
			LOG_DEBUG("Exited from scene " << name());
		}

		void Scene::onKeyPressed(sf::Event::KeyEvent kEvent)
		{
			LOG_DEBUG("Key pressed: " << int{ kEvent.code });
		}

		void Scene::onKeyReleased(sf::Event::KeyEvent kEvent)
		{
			LOG_DEBUG("Key released: " << int{ kEvent.code });
		}

		void Scene::setNextScene(const std::string &name)
		{
			__ASSERT(p_scnManager, "Null SceneManager on Scene");
			p_scnManager->setScene(name);
		}

		u32 Scene::getRandomNumer(u32 max /*= 1*/, u32 min /*= 0*/)
		{
			return p_scnManager->parentWindow()->getRandomNumer(max, min);
		}

		uptr<ResourceManager> & Scene::resourceManager()
		{
			return p_scnManager->resourceManager();
		}
	}
}

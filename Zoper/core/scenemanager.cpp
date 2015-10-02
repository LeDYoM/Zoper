#include "scenemanager.hpp"
#include "scene.hpp"
#include "log.h"

namespace lib
{
	namespace core
	{
		SceneManager::SceneManager()
		{
		}


		SceneManager::~SceneManager()
		{
		}

		void SceneManager::addScene(Scene *newScene)
		{
			_scenes.push_back(newScene);
		}

		void SceneManager::setScene(Scene *scene)
		{
			if (_currentScene)
			{
				_currentScene->onDeactivated();
			}
			_currentScene = scene;
			_currentScene->onActivated();
		}
		
		void SceneManager::setScene(const std::string &name)
		{
			Scene *scene = getSceneByName(name);
			
			if (scene)
			{
				setScene(scene);
			}
			else
			{
				LOG_ERROR("Scene "<<name<<" not found in SceneManager");
			}
		}
		
		Scene *SceneManager::getSceneByName(const std::string &name) const
		{
			for (int i=0;i<_scenes.size();++i)
			{
				if (_scenes[i]->name() == name)
				{
					return _scenes[i];
				}
			}
			return 0;
		}
	}
}

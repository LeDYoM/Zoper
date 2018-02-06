#include "scene.hpp"
#include "renderizable.hpp"
#include <mtypes/include/log.hpp>
#include <lib/core/window.hpp>
#include <lib/core/host.hpp>

#include <type_traits>

namespace lib::scene
	{
	Scene::Scene(str name) : SceneNode{ nullptr,std::move(name) } {}

	Scene::~Scene()
	{
	}

	void Scene::onCreated()
	{
        clock.restart();

        m_sceneStates = ensureComponentOfType<std::remove_reference_t<decltype(*m_sceneStates)>>();
		m_sceneStates->start(0);
		m_sceneStates->UseDeferred();
    }

    u32 Scene::state()
    {
        return m_sceneStates->currentState();
    }

    void Scene::setState(const size_type ns)
    {
        m_sceneStates->setState(ns);
    }
}

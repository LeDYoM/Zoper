#include "scene.hpp"
#include <lib/scene/renderizables/renderizable.hpp>

#include <lib/include/core/log.hpp>
#include <lib/core/window.hpp>
#include <lib/core/host.hpp>

#include <type_traits>

namespace lib::scene
{
	Scene::Scene(str name) : SceneNode{ nullptr,std::move(name) } {}

	Scene::~Scene() = default;

	void Scene::onCreated()
	{
        clock.restart();

        m_sceneStates = ensureComponentOfType<std::remove_reference_t<decltype(*m_sceneStates)>>();
		m_sceneStates->start(0);
    }

    size_type Scene::state()
    {
        return m_sceneStates->currentState();
    }

    void Scene::setState(const size_type ns)
    {
        m_sceneStates->setState(ns);
    }
}

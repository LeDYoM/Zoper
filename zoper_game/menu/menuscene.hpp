#pragma once

#ifndef ZOPER_MENUSCENE_INCLUDE_HPP
#define ZOPER_MENUSCENE_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/connection.hpp>
#include <lib/scene/renderizables/nodequad.hpp>
#include <lib/scene/scene.hpp>
#include <lib/include/scene/iscene.hpp>

namespace zoper
{
	using namespace lib;
	class MenuScene final : public scene::Scene
	{
	public:
        using BaseClass = scene::Scene;

		DECLARE_SCENE(MenuScene)

		MenuScene();
		~MenuScene() override;

		virtual void onCreated() override;

	private:
		ireceiver m_receiver;
		sptr<scene::nodes::NodeQuad> m_logo;
		sptr<scene::nodes::NodeQuad> m_background;
	};
}

#endif
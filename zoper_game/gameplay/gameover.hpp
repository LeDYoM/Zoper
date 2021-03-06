#pragma once

#ifndef ZOPER_GAMEOVER_SCENENODE_INCLUDE_HPP
#define ZOPER_GAMEOVER_SCENENODE_INCLUDE_HPP

#include <haf/scene/include/scenenode.hpp>

namespace zoper
{
    using namespace haf;

	class GameOverSceneNode : public scene::SceneNode
	{
	public:
        GameOverSceneNode(scene::SceneNode* const parent, mtps::str name);
        ~GameOverSceneNode() override;

	private:
        mtps::sptr<scene::SceneNode> m_gameOverrg;
    };
}

#endif
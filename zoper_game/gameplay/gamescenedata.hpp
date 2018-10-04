#pragma once

#ifndef ZOOPER_GAMESCENE_DATA_INCLUDE_HPP
#define ZOOPER_GAMESCENE_DATA_INCLUDE_HPP

#include <mtypes/include/types.hpp>

#include <lib/scene/scenenode.hpp>
#include <lib/scene/nodes/textquad.hpp>

#include "../gameshareddata.hpp"
#include "boardgroup.hpp"

namespace zoper
{
    class GameScene;

    using namespace lib;

    class GameSceneData
    {
    public:
        GameSceneData() {}
        ~GameSceneData() {}

        void createData(GameScene& gameScene, const GameMode gameMode);

//        sptr<scene::SceneNode> m_mainBoardrg;
        sptr<scene::SceneNode> m_gameOverrg;
        sptr<scene::SceneNode> m_levelrg;
        sptr<scene::SceneNode> m_pauseSceneNode;
        sptr<scene::SceneNode> m_gameOverText;
        sptr<scene::nodes::TextQuad> m_scoreQuad, m_goalQuad;
        sptr<scene::nodes::SceneNodeText> m_pauseText;
        sptr<BoardGroup> m_boardGroup;
    };
}

#endif
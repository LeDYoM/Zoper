#pragma once

#ifndef ZOPER_HIGHSCORETEXTCONTROLLER_INCLUDE_HPP
#define ZOPER_HIGHSCORETEXTCONTROLLER_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/connection.hpp>
#include <lib/scene/include/scenenode.hpp>
#include <lib/resources/include/ifont.hpp>
#include <lib/scene/include/scenenodetypes.hpp>
#include <lib/scene_nodes/include/tablenode.hpp>
#include <lib/scene_nodes/include/scenenodetext.hpp>
#include "../loaders/highscoresresources.hpp"
#include "highscoresdata.hpp"

namespace zoper
{
using namespace lib;
using namespace lib::scene;

namespace
{
}
class HighScoreTextController : public nodes::TableNode<nodes::SceneNodeText>
{
    using BaseClass = nodes::TableNode<nodes::SceneNodeText>;

public:
    HighScoreTextController(scene::SceneNode *parent, str name);
    virtual ~HighScoreTextController();

    virtual void onCreated() override;

    emitter<> Finished;

private:
    void standarizeText(const sptr<nodes::SceneNodeText> &ntext);
    void saveHighScores();
    HighScoresData m_hsData;
    sptr<IFont> m_normalFont;
    Color m_normalColor;
    Color m_selectedColor;
};
} // namespace zoper

#endif

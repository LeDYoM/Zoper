#ifndef ZOPER_HIGHSCORETEXTCONTROLLER_INCLUDE_HPP
#define ZOPER_HIGHSCORETEXTCONTROLLER_INCLUDE_HPP

#include <htypes/include/types.hpp>
#include <htypes/include/connection.hpp>
#include <haf/scene/include/scenenode.hpp>
#include <haf/resources/include/ifont.hpp>
#include <haf/scene_nodes/include/renderizable_scenenode.hpp>
#include <haf/scene_nodes/include/tablenode.hpp>
#include <haf/scene_nodes/include/scenenodetext.hpp>
#include <haf/scene_components/include/animationcomponent.hpp>
#include "../loaders/highscoresresources.hpp"
#include "highscoresdata.hpp"

namespace zoper
{
using namespace haf;
using namespace haf::scene;

class HighScoreTextController : public nodes::TableNode<nodes::SceneNodeText>
{
    using BaseClass = nodes::TableNode<nodes::SceneNodeText>;    
public:
    using BaseClass::prop;
    using TransformationProperties::prop;

    HighScoreTextController(scene::SceneNode* parent, htps::str name);
    virtual ~HighScoreTextController();

    virtual void onCreated() override;

    htps::emitter<> Finished;

private:
    void standarizeText(const htps::sptr<nodes::SceneNodeText>& ntext);
    void saveHighScores();
    void addHighScoresLine(const htps::size_type counter,
                           const HighScore& element,
                           const bool is_inserting);
    void addHighScoreEditor(const htps::sptr<nodes::SceneNodeText>& label,
                            const htps::size_type counter);
    void addEditAnimation(const htps::size_type line_index);
    HighScoresData m_hsData;
    htps::sptr<res::IFont> m_normalFont;
    htps::sptr<scene::AnimationComponent> animation_component_;

    Color m_normalColor;
    Color m_selectedColor;
};
}  // namespace zoper

#endif

#include "highscoretextcontroller.hpp"
#include "highscoresdata.hpp"
#include "highscorevalidator.hpp"
#include "../loaders/highscoresresources.hpp"
#include "../gameshareddata.hpp"
#include <haf/filesystem/include/fileserializer.hpp>
#include <haf/scene_components/include/texteditorcomponent.hpp>
#include <haf/scene_components/include/scenemetricsview.hpp>
#include <haf/scene_nodes/include/scenenodetext_properties.hpp>
#include <haf/resources/include/ittfont.hpp>
#include <haf/resources/include/resourceview.hpp>
#include <haf/shareddata/include/shareddata.hpp>
#include <haf/shareddata/include/shareddataviewer.hpp>

using namespace mtps;
using namespace haf;
using namespace haf::scene;

namespace zoper
{
static constexpr char HighScoresFileName[] = "high_scores.txt";

HighScoreTextController::HighScoreTextController(SceneNode* parent, str name) :
    BaseClass{parent, "HighScreTextController"}
{}

HighScoreTextController::~HighScoreTextController() = default;

void HighScoreTextController::onCreated()
{
    BaseClass::onCreated();

    auto resource_view = dataWrapper<res::ResourceView>();

    m_normalFont =
        resource_view->getTTFont(HighScoresResources::MenuFontId)->font(72);
    m_normalColor        = colors::Blue;
    m_selectedColor      = colors::Red;
    animation_component_ = addComponentOfType<scene::AnimationComponent>();

    // Request the high scores.
    dataWrapper<sys::FileSerializer>()->deserializeFromFile(HighScoresFileName,
                                                            m_hsData);

    // Request game score
    Score gameScore = dataWrapper<shdata::SharedDataViewer<GameSharedData>>()
                          ->view(GameSharedData::address())
                          ->score;
    Rectf32 textBox{
        rectFromSize(dataWrapper<SceneMetricsView>()->currentView().size())
            .setLeftTop({0, 250})
            .setSize({2000, 1500})};
    prop<haf::scene::Position>().set(textBox.leftTop());
    prop<TableNodeProperties>().set<SceneNodeSize>(textBox.size());
    prop<TableSize>().set({3U, NumHighScore});

    size_type positionInTable{0U};
    const bool isInserting{
        m_hsData.tryInsertHighScore(gameScore, positionInTable)};

    size_type counter{0};
    for (const auto& element : m_hsData.highScoresList())
    {
        addHighScoresLine(counter, element,
                          (isInserting && positionInTable == counter));
        ++counter;
    }

    if (!isInserting)
    {
        auto input_component(addComponentOfType<input::InputComponent>());
        input_component->KeyPressed.connect(
            [this](const auto&) { Finished(); });
    }
}

void HighScoreTextController::addHighScoresLine(const size_type counter,
                                                const HighScore& element,
                                                const bool is_inserting)
{
    using namespace nodes;

    auto label(
        createNodeAt(vector2dst{0, counter}, make_str("label", 0, counter)));
    standarizeText(label);
    label->prop<SceneNodeTextProperties>().set<Text>(make_str(counter, "."));

    label = createNodeAt(vector2dst{1, counter}, make_str("label", 1, counter));
    standarizeText(label);
    label->prop<SceneNodeTextProperties>().set<Text>(make_str(element.score));

    label = createNodeAt(vector2dst{2, counter}, make_str("label", 2, counter));
    standarizeText(label);

    if (is_inserting)
    {
        addHighScoreEditor(label, counter);
    }
    else
    {
        label->prop<SceneNodeTextProperties>().set<Text>(element.name);
    }
}

void HighScoreTextController::addHighScoreEditor(const sptr<SceneNode>& label,
                                                 const size_type counter)
{
    addEditAnimation(counter);
    auto editor(label->addComponentOfType<TextEditorComponent>());
    editor->setTextValidator(muptr<HighScoreValidator>());
    editor->Accepted.connect([this, counter](const str& entry) mutable {
        m_hsData.setHighScoreName(counter, entry);
        saveHighScores();
        Finished();
    });
    editor->Rejected.connect(
        [editor_ = mwptr(editor)]() { editor_.lock()->enabled = true; });
}

void HighScoreTextController::addEditAnimation(const size_type line_index)
{
    LogAsserter::log_assert(line_index < prop<TableSize>().get().y,
                            "Invalid line_index");

    for_each_tableSceneNode_in_y(
        line_index,
        [this](const auto, const sptr<nodes::SceneNodeText>& element) {
            animation_component_->addCircledPropertyAnimation(
                time::TimePoint_as_miliseconds(2000),
                element->prop<nodes::SceneNodeTextProperties>()
                    .get_property_reference<nodes::TextColor>(),
                colors::White, colors::Black);
        });
}

void HighScoreTextController::standarizeText(
    const sptr<nodes::SceneNodeText>& ntext)
{
    ntext->prop<nodes::SceneNodeTextProperties>()
        .put<nodes::TextColor>(m_normalColor)
        .put<nodes::Font>(m_normalFont);
}

void HighScoreTextController::saveHighScores()
{
    DisplayLog::info("Saving highscores...");

    dataWrapper<sys::FileSerializer>()->serializeToFileTemplate(HighScoresFileName,
                                                        m_hsData);
    DisplayLog::info("High Scores saved");
}
}  // namespace zoper

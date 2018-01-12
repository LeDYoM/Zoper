#include "highscoresscene.hpp"
#include <lib/scene/renderizables/nodeshape.hpp>
#include <lib/scene/renderizables/nodequad.hpp>
#include <lib/scene/renderizables/nodetext.hpp>
#include <lib/core/resourcemanager.hpp>
#include <lib/core/host.hpp>
#include "../highscoresdata.hpp"
#include "../highscoretextcontroller.hpp"
#include "../highscoresscenestates.hpp"

namespace zoper
{
	using namespace lib;
	using namespace lib::scene;
	using namespace lib::scene::nodes;

	HighScoresScene::HighScoresScene() : BaseClass{ "HighScoresScene" } {}

	void HighScoresScene::onCreated()
	{
		BaseClass::onCreated();

		m_normalFont = resourceManager().getResource<TTFont>("menu.mainFont", "resources/oldct.ttf");
		m_normalColor = colors::Blue;
		m_selectedColor = colors::Red;
		m_normalCharacterSize = 72;

		m_resources = msptr<HighScoresResources>();
		m_resources->loadResources(resourceManager());

		m_background = createRenderizable<NodeQuad>("background");
		m_background->box = rectFromSize(2000.0f, 2000.0f);
		m_background->texture = m_resources->background;
		m_background->color = colors::White;

		auto statesController = ensureComponentOfType<StatesController<HighScoresSceneStates>>();
		statesController->UseDeferred();
		createSceneNode<HighScoreTextController>("HishScoreTextController");
		statesController->start(HighScoresSceneStates::Show);
	}

	void HighScoresScene::updateScene()
	{
	}
}
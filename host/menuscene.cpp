#include "menuscene.hpp"
#include <lib/draw/nodeshape.hpp>
#include <lib/draw/nodequad.hpp>
#include <lib/draw/nodetext.hpp>
#include <lib/core/resourcemanager.hpp>
#include <lib/core/host.hpp>
#include <lib/core/events/eventmanager.hpp>
#include <lib/core/events/inputevent.hpp>

#include "mainmenu.hpp"
#include "optionsmenu.hpp"
#include "keyredefinitionmenu.hpp"
#include "startlevelmenu.hpp"

namespace zoper
{
	using namespace lib;
	using namespace lib::draw;
	using namespace zmenu;

	MenuScene::MenuScene()
		: lib::menu::MenuManager("MenuScene") {}

	void MenuScene::onInit()
	{
		MenuManager::onInit();
		m_background = createRenderizable<NodeQuad>("background", vector2df{ 2000.0f,2000.0f }, host().resourceManager().getTexture("game_menu.background"), draw::colors::White);

		addMenuStep(msptr<MainMenu>(this));
		addMenuStep(msptr<OptionsMenu>(this));
//		addMenuStep(msptr<KeyRedefinitionMenu>(this));
		addMenuStep(msptr<StartLevelMenu>(this));

		m_logo = createRenderizable<NodeQuad>("mainLogo", vector2df{ 800,400 }, host().resourceManager().getTexture("game_menu.logo"), draw::colors::White);
		m_logo->setPositionX(getCenterCoordinates().x/*, lib::draw::Alignment::Center*/);
		m_logo->setPositionY(100);
	}

	void MenuScene::onEnterScene()
	{
		MenuManager::onEnterScene();
		start(MainMenu::ClassName);
	}

	void MenuScene::onExitScene()
	{
		lib::menu::MenuManager::onExitScene();
	}

	void MenuScene::update()
	{

	}

	const lib::vector2df MenuScene::getDefaultSizeView()
	{
		return{ 2000.0f, 2000.0f };
	}
}

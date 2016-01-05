#include "mainmenu.hpp"
#include "lib/menu/menumanager.hpp"
#include "lib/menu/choosecontrol.hpp"
#include "lib/menu/menudescriptors.hpp"
#include "lib/scn/draw/renderizable.hpp"
#include "lib/scn/resourcemanager.hpp"
#include "lib/scn/resource.hpp"

#include "common.hpp"

namespace zoper
{
	namespace zmenu
	{
		MainMenu::MainMenu()
			: lib::menu::MenuStep{ "MainMenu" }, _gameConfig{ ":NextGame" }
		{
		}


		MainMenu::~MainMenu()
		{
		}

		void MainMenu::onCreate()
		{
			_logo = createSpriteShape("mainLogo");
			auto _logoSprite = _logo->getAsEllipseShape();
			auto rManager = menuManager()->resourceManager();
			_logoSprite->setTexture(rManager->getResource("game_menu.logo")->getAsTexture(),true,false);
			_logoSprite->setSize(sf::Vector2f(800, 400));
			_logo->setPositionX(menuManager()->getCenterCoordinates().x, lib::scn::draw::Alignment::Center);
			_logo->setPositionY(100);

			auto callBack = [this](lib::u32 index/*, lib::sptr<lib::scn::draw::Renderizable> node, 
				const lib::u32 subNodeIndexSelected, lib::sptr<lib::scn::draw::Renderizable> subNode*/)
			{
				switch (index)
				{
				case 0:
					_gameConfig.addConfigInt(GameModeStr, 0, true);
					_gameConfig.addConfigInt(StartLevelStr, 0, true);
					menuManager()->setNextScene("GameScene");
					break;
				case 1:
					_gameConfig.addConfigInt(GameModeStr, 1, true);
					_gameConfig.addConfigInt(StartLevelStr, 0, true);
					menuManager()->setNextScene("GameScene");
					break;
				case 2:
					menuManager()->changeStep("OptionsMenu");
					break;
				case 3:
				default:
					menuManager()->exitProgram();
					break;
				}
			};
			_chooseControl = lib::sptr<lib::menu::ChooseControl>(new lib::menu::ChooseControl("mainmenu_chooseControl",
				rManager->getResource("game_menu.mainFont"),
				lib::scn::draw::Alignment::Center,
				90, 1,
				callBack,
//				nullptr,
				lib::sptr<lib::menu::CursorDescriptor>(new lib::menu::CursorDescriptor(3, lib::vector2df{ 90.0f, 90.0f },sf::Color::Red)),
				std::vector<lib::sptr<lib::menu::OptionDescriptor>>{
				lib::sptr<lib::menu::OptionDescriptor>(new lib::menu::OptionDescriptor("Play token mode", sf::Color::Blue)),
					lib::sptr<lib::menu::OptionDescriptor>(new lib::menu::OptionDescriptor("Play time mode", sf::Color::Blue)),
				lib::sptr<lib::menu::OptionDescriptor>(new lib::menu::OptionDescriptor("Options", sf::Color::Blue)),
				lib::sptr<lib::menu::OptionDescriptor>(new lib::menu::OptionDescriptor("Exit", sf::Color::Blue))
			}));
			addMenuControl(_chooseControl);
			_chooseControl->setPosition(menuManager()->getCenterCoordinates().x, 700);
		}
	}
}

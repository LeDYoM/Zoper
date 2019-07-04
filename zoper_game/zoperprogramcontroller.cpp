#include "zoperprogramcontroller.hpp"
#include "menu/menuscene.hpp"
#include "gameplay/gamescene.hpp"
#include "highscores/highscoresscene.hpp"

#include <lib/core/serializer.hpp>
#include <lib/scene/scenemanager.hpp>
#include <lib/system/systemprovider.hpp>

namespace zoper
{
    using namespace lib;
    using namespace lib::scene;

    ZoperProgramController::ZoperProgramController() {}
    ZoperProgramController::~ZoperProgramController() {}

    void ZoperProgramController::onInit()
    {
        appContext().setApplicationName("Zoper");
        appContext().setApplicationVersion(1);
        appContext().setApplicationSubVersion(2);
        appContext().setApplicationPatch(2);
        gameSharedData = msptr<GameSharedData>();

        keyMapping = muptr<KeyMapping>();
        Serializer<KeyMapping> kmSerializer;
        kmSerializer.deserialize("keyboard.txt", *keyMapping);
        systemProvider().sceneManager().setViewRect({0,0,2000,2000});

        auto &scene_factory(systemProvider().sceneManager().sceneController()->sceneFactory());

        scene_factory.registerSceneType<MenuScene>();
        scene_factory.registerSceneType<GameScene>();
        scene_factory.registerSceneType<HighScoresScene>();
        
        systemProvider().sceneManager().sceneController()->setSceneDirector([this](const str& scene_name) -> str
        {
            if (scene_name == (MenuScene::StaticTypeName))
            {
                // Did the user selected exit?
                if (gameSharedData->exitGame) {
                    return str();
                }
                return GameScene::StaticTypeName;
            }
            return MenuScene::StaticTypeName;
        });

        // Hack to test high scores
        {
//            gameData->score = 10000;
//            systemProvider().sceneManager().startFirstScene<HighScoresScene>();
        }

        systemProvider().sceneManager().sceneController()->startScene<MenuScene>();
    }

    IAppDescriptor ZoperProgramController::getAppDescriptor() const
    {
        return IAppDescriptor
        {
            "config.cfg",
            "res.cfg",
            WindowCreationParams
            {
                "ZOPER",
                1024,
                768,
                32,
                0,
                false,
                false,
                false
            }
        };
    }
}

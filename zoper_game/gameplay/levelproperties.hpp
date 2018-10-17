#pragma once

#ifndef ZOPER_LEVELPROPERTIES_INCLUDE_HPP
#define ZOPER_LEVELPROPERTIES_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/vector2d.hpp>

#include <lib/scene/components/timercomponent.hpp>
#include <lib/scene/components/icomponent.hpp>
#include <lib/scene/color.hpp>

#include "../gameshareddata.hpp"
#include "gamehud.hpp"

namespace zoper
{
    using namespace lib;

    class GameSceneData;

    using LevelType = size_type;

	class LevelProperties : public scene::IComponent
	{
	public:
        void setUp(const size_type currentLevel, 
            const GameMode gameMode, sptr<GameSceneData> gameSceneData,
            sptr<scene::TimerComponent> m_sceneTimerComponent);

        inline size_t millisBetweenTokens() const { return m_millisBetweenTokens; }
        inline size_t baseScore() const { return m_baseScore; }
        inline size_t stayCounter() const { return m_stayCounter; }
        void increaseScore(const size_type scoreIncrement);

        static constexpr size_t maxLevelWithProperties{ 25U };

        void tokenConsumed();
        emitter<const LevelType> levelChanged;

	private:
        void updateGoals();
        void updateLevelData();
        void setLevel(const LevelType currentLevel);

        Timer m_levelTimer;
        scene::TimerConnectorSPtr m_updateLevelDataTimer;
        sptr<scene::TimerComponent> m_sceneTimerComponent;

        size_type m_consumedTokens{ 0U };
        LevelType m_currentLevel{ 0U };
        size_t m_millisBetweenTokens{ 0U };
        size_t m_baseScore{ 0U };
        size_t m_stayCounter{ 0U };
        size_t m_currentScore{ 0U };
        GameMode m_gameMode;
        sptr<GameSceneData> m_gameSceneData;
        sptr<GameHudSceneNode> m_gameHud;
    };
}

#endif

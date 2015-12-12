#include "gamescene.hpp"

#include "tile.hpp"
#include "player.hpp"

#include "lib/board/boardmodel.hpp"
#include "lib/board/itilescontroller.hpp"
#include "lib/scn/draw/renderizable.hpp"
#include "lib/log.hpp"
#include "lib/compileconfig.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <functional>

namespace zoper
{
	GameScene::GameScene()
		: Scene("GameScene"), Configuration("config.cfg")
	{
		_gameData.size.x = 18;
		_gameData.size.y = 12;
		_gameData.centerRect.begin.x = 7;
		_gameData.centerRect.begin.y = 4;
		_gameData.centerRect.size.x = 4;
		_gameData.centerRect.size.y = 4;
		_gameData.generateTokenZones();
	}

	GameScene::~GameScene()
	{
	}

	sf::Font font;

	void GameScene::onInit()
	{
		p_boardModel = lib::sptr<lib::board::BoardModel>(new lib::board::BoardModel(_gameData.size, this));

		font.loadFromFile("resources/score.ttf");

		getView()->setSize(2000, 2000);
		getView()->setCenter(1000, 1000);
		updateView();

		addPlayer();

		_scoreText = this->createText("scoretxt");
		_scoreDisplay = this->createText("scoredisplay");

		auto _scoreTextText = _scoreText->getAsText();
		auto _scoreDisplayText = _scoreDisplay->getAsText();

		_scoreTextText->setFont(font); // font is a sf::Font
		_scoreDisplayText->setFont(font); // font is a sf::Font

		_scoreTextText->setString("Score: ");
		_scoreDisplayText->setString("00000");

		_scoreTextText->setCharacterSize(90);
		_scoreDisplayText->setCharacterSize(90);

		_scoreTextText->setColor(sf::Color::Blue);
		_scoreDisplayText->setColor(sf::Color::White);

		_scoreTextText->setScale(1.0f, 2.0f);
		_scoreDisplayText->setScale(1.0f, 2.0f);

		//		text->setStyle(sf::Text::Bold | sf::Text::Underlined);

		_scoreTextText->setPosition(50, 50);
		auto rBounds = _scoreTextText->getLocalBounds();
		_scoreDisplayText->setPosition(50 + rBounds.width, 50);
	}

	void GameScene::onDeinit()
	{
	}

	void GameScene::onEnterScene()
	{
		clock.restart();
	}

	void GameScene::onExitScene()
	{
		p_boardModel = nullptr;
	}

	void GameScene::update()
	{
		if (clock.getElapsedTime().asMilliseconds() > _millisBetweenTokens)
		{
			// New token
			generateNextToken();
			clock.restart();
		}
	}

	void GameData::generateTokenZones()
	{
		// From left to right
		_tokenZones[0].zone.begin.x = 0;
		_tokenZones[0].zone.begin.y = centerRect.begin.y;
		_tokenZones[0].zone.size.x = centerRect.begin.x - 1;
		_tokenZones[0].zone.size.y = (centerRect.begin.y + centerRect.size.y) - 1;
		_tokenZones[0].direction = Direction::DirectionData::Right;

		// From top to bottom
		_tokenZones[1].zone.begin.x = centerRect.begin.x;
		_tokenZones[1].zone.begin.y = 0;
		_tokenZones[1].zone.size.x = (centerRect.begin.x + centerRect.size.x) - 1;
		_tokenZones[1].zone.size.y = centerRect.begin.y - 1;
		_tokenZones[1].direction = Direction::DirectionData::Down;

		// From right to left
		_tokenZones[2].zone.begin.x = size.x - 1;
		_tokenZones[2].zone.begin.y = centerRect.begin.y;
		_tokenZones[2].zone.size.x = (centerRect.begin.x + centerRect.size.x);
		_tokenZones[2].zone.size.y = (centerRect.begin.y + centerRect.size.y) - 1;
		_tokenZones[2].direction = Direction::DirectionData::Left;

		// From bottom to top
		_tokenZones[3].zone.begin.x = centerRect.begin.x;
		_tokenZones[3].zone.begin.y = size.y - 1;
		_tokenZones[3].zone.size.x = (centerRect.begin.x + centerRect.size.x) - 1;
		_tokenZones[3].zone.size.y = centerRect.begin.y + centerRect.size.y;
		_tokenZones[3].direction = Direction::DirectionData::Up;

		for (lib::u32 i = 0; i < NUMWAYS; ++i)
		{
			_tokenZones[i].size = _tokenZones[i].direction.isHorizontal() ? centerRect.size.y : centerRect.size.x;
		}
	}

	void GameScene::generateNextToken()
	{
		const GameData::TokenZone &currentTokenZone = _gameData._tokenZones[_nextTokenPart];

		LOG_DEBUG("NextTokenPart: " << std::to_string(_nextTokenPart));
		LOG_DEBUG("x1: " << currentTokenZone.zone.begin.x << " y1: " << currentTokenZone.zone.begin.y << 
			" x2: " << currentTokenZone.zone.size.x << " y2: " << currentTokenZone.zone.size.y);
//		LOG_DEBUG("distX: " << currentTokenZone.distX() << " distY: " << currentTokenZone.distY());
//		LOG_DEBUG("horizontal: " << currentTokenZone.direction.isHorizontal() << " increment: " << currentTokenZone.increment);

		lib::u32 newToken = getRandomNumer(NUMTOKENS);

		lib::u32 sizep = getRandomNumer(currentTokenZone.size);

		lib::u32 newX = currentTokenZone.zone.begin.x + (currentTokenZone.direction.isHorizontal() ? 0 : sizep);
		lib::u32 newY = currentTokenZone.zone.begin.y + (currentTokenZone.direction.isHorizontal() ? sizep : 0);
		LOG_DEBUG("New tile pos: " << newX << "," << newY);

		lib::vector2du32 loopPosition{ (currentTokenZone.direction.isHorizontal() ? currentTokenZone.zone.size.x : newX),
			(currentTokenZone.direction.isHorizontal() ? newY : currentTokenZone.zone.size.y) };
//		lib::vector2du32 destPosition;
		LOG_DEBUG("Starting at: " << loopPosition.x << "," << loopPosition.y);

		// Now, we have the data for the new token generated, but first, lets start to move the row or col.
		Direction loopDirection = currentTokenZone.direction.negate();
		for_each_token_in_line(loopPosition, loopDirection, [this](const lib::vector2du32 &loopPosition, const Direction &direction)
		{
			// TO DO: Check for game over
			if (!p_boardModel->tileEmpty(loopPosition))
			{
				lib::vector2du32 dest = direction.negate().applyToVector(loopPosition);
				p_boardModel->moveTile(loopPosition, dest);

				if (pointInCenter(dest))
				{
					//game over
				}
			}
			return true;
		});
		// Set the new token
		addNewToken(lib::vector2du32{ newX, newY }, newToken);
		_nextTokenPart = (_nextTokenPart + 1) % NUMWAYS;

		_debugDisplayBoard();
	}

	void GameScene::for_each_token_in_line(const lib::vector2du32 &startPosition, const Direction &direction,
		std::function<bool(const lib::vector2du32 &, const Direction &)> updatePredicate)
	{
		lib::vector2du32 loopPosition{ startPosition };
		// Now, we have the data for the new token generated, but first, lets start to move the row or col.
		bool stay{ true };
		do
		{
			stay &= updatePredicate(loopPosition, direction);
			loopPosition = direction.applyToVector(loopPosition);
			stay &= p_boardModel->validCoords(loopPosition);
		} while (stay);
	}

	void GameScene::addPlayer()
	{
		LOG_DEBUG("Adding player tile at " << _gameData.centerRect.begin.x << "," << _gameData.centerRect.begin.y);
		__ASSERT(!p_player, "Player already initialized");
		// Create the player instance
		p_player = lib::sptr<Player>(new Player(lib::vector2du32(_gameData.centerRect.begin),tileSize()));

		// Set the radius depending on the scene
//		p_player->getAsEllipseShape()->setSize(tileSize());

		// Add it to the board and to the scene nodes
		p_boardModel->setTile(p_player->boardPosition(), std::dynamic_pointer_cast<lib::board::ITile>(addRenderizable(p_player)));
	}

	void GameScene::addNewToken(const lib::vector2du32 &position, lib::u32 newToken)
	{
		LOG_DEBUG("Adding new tile at " << position.x << "," << position.y << " with value " << newToken);
		// Create a new Tile instance
		lib::sptr<Tile> newTileToken = lib::sptr<Tile>(new Tile(lib::board::BoardTileData(newToken),tileSize()));
		// Set the position in the scene depending on the board position
		newTileToken->setPosition(board2Scene(position));

		// Add it to the board and to the scene nodes
		p_boardModel->setTile(position, std::dynamic_pointer_cast<lib::board::ITile>(addRenderizable(newTileToken)));
	}

	void GameScene::onKeyPressed(sf::Event::KeyEvent kEvent)
	{
		Scene::onKeyPressed(kEvent);
		auto dir = p_player->getDirectionFromKey(kEvent.code);
		if (dir.isValid())
		{
			p_player->setCurrentDirection(dir);
			movePlayer(dir);
		}
		else if (p_player->isLaunchKey(kEvent.code))
		{
			launchPlayer();
		}
	}

	void GameScene::movePlayer(const Direction & dir)
	{
		__ASSERT(dir.isValid(), "Invalid direction passed to move");
		auto nPosition = dir.applyToVector(p_player->boardPosition());
		if (pointInCenter(nPosition))
		{
			p_boardModel->moveTile(p_player->boardPosition(), lib::vector2du32(nPosition.x,nPosition.y));
			p_player->setBoardPosition(lib::vector2du32(nPosition.x, nPosition.y));
		}
	}

	void GameScene::onKeyReleased(sf::Event::KeyEvent kEvent)
	{
		Scene::onKeyReleased(kEvent);
	}

	void GameScene::launchPlayer()
	{
		LOG_DEBUG("Launching player");
		const Direction loopDirection = p_player->currentDirection();
		lib::vector2du32 loopPosition{ p_player->boardPosition() };
		lib::board::BoardTileData tokenType = p_player->getData();
		for_each_token_in_line(loopPosition, loopDirection, [this,tokenType](const lib::vector2du32 &loopPosition, const Direction &direction)
		{
			if (!p_boardModel->tileEmpty(loopPosition) && !pointInCenter(loopPosition))
			{
				lib::board::BoardTileData currentTokenType = p_boardModel->getTile(loopPosition).lock()->getData();
				if (currentTokenType == tokenType)
				{
					p_boardModel->deleteTile(loopPosition);
				}
				else
				{
					p_boardModel->changeTileData(p_player->boardPosition(), currentTokenType);
					LOG_DEBUG("Player type changed to " << p_player->getData());
					return false;
				}
			}
			return true;
		});
	}

	bool GameScene::pointInCenter(const lib::vector2du32 &position) const
	{
		if (p_boardModel->validCoords(position))
		{
			if (position.x < _gameData.centerRect.begin.x || position.y < _gameData.centerRect.begin.y)
				return false;

			if (position.x >= (_gameData.centerRect.begin.x + _gameData.centerRect.size.x) || position.y >= (_gameData.centerRect.begin.y + _gameData.centerRect.size.y))
				return false;

			return true;
		}
		return false;
	}

	const lib::vector2df GameScene::board2Scene(const lib::vector2du32 &bPosition) const
	{
		return lib::vector2df{ (getView()->getSize().x * bPosition.x) / static_cast<float>(p_boardModel->size().x),
			(getView()->getSize().y * bPosition.y) / static_cast<float>(p_boardModel->size().y) };
	}

	const lib::vector2df GameScene::tileSize() const
	{
		return board2Scene(lib::vector2du32( 1, 1 ));
	}

	void GameScene::_debugDisplayBoard() const
	{
		for (lib::u32 y = 0; y < _gameData.size.y; ++y)
		{
			std::string temp;
			for (lib::u32 x = 0; x < _gameData.size.x; ++x)
			{
				std::string chTemp;
				auto lp_tile = p_boardModel->getTile(lib::vector2du32(x, y)).lock();
				if (lp_tile)
				{
					chTemp = std::to_string(lp_tile->getData());
				}
				else
				{
					chTemp = "*";
					if (pointInCenter(lib::vector2du32(x, y)))
						chTemp = "C";
				}

				temp += chTemp;
			}
			LOG_DEBUG(temp);
		}
	}

	void GameScene::tileAdded(const lib::vector2du32 &position, lib::board::WITilePointer nTile)
	{
		// Tile appeared
		if (auto ztile = std::dynamic_pointer_cast<Tile>(nTile.lock()))
		{
			tokenAppeared(position, ztile);
		}
		else if (auto ztile = std::dynamic_pointer_cast<Player>(nTile.lock()))
		{
			// Set the position in the scene depending on the board position
			playerAppeared(position, ztile);
		}
	}

	void GameScene::tileDeleted(const lib::vector2du32 &position, lib::board::WITilePointer nTile)
	{
		if (auto ztile = std::dynamic_pointer_cast<Tile>(nTile.lock()))
		{
			tokenDissapeared(position,ztile);
		}
		else if (auto ztile = std::dynamic_pointer_cast<Player>(nTile.lock()))
		{
			playerDissapeared(position,ztile);
		}
	}

	void GameScene::tileMoved(const lib::vector2du32 &source, const lib::vector2du32 &dest, lib::board::WITilePointer tile)
	{
		if (auto ztile = std::dynamic_pointer_cast<Tile>(tile.lock()))
		{
			tokenMoved(source, dest, ztile);
		}
		else if (auto ztile = std::dynamic_pointer_cast<Player>(tile.lock()))
		{
			playerMoved(source, dest, ztile);
		}
	}

	void GameScene::tileChanged(const lib::vector2du32 &position, lib::board::WITilePointer nTile, 
		const lib::board::BoardTileData &ov, const lib::board::BoardTileData &nv)
	{
		if (auto ztile = std::dynamic_pointer_cast<Tile>(nTile.lock()))
		{
			tokenChangedValue(position, ztile, ov, nv);
		}
		else if (auto ztile = std::dynamic_pointer_cast<Player>(nTile.lock()))
		{
			playerChangedValue(position, ztile, ov, nv);
		}
	}

	void GameScene::tokenMoved(const lib::vector2du32 &source, const lib::vector2du32 &dest, lib::sptr<Tile> tile)
	{
		tile->setPosition(board2Scene(dest));
	}

	void GameScene::tokenAppeared(const lib::vector2du32 &position, lib::sptr<Tile> tile)
	{

	}

	void GameScene::tokenDissapeared(const lib::vector2du32 &position, lib::sptr<Tile> tile)
	{
		LOG_DEBUG("Deleting token " << tile->name() << " from scene");
		removeRenderizable(tile);
	}

	void GameScene::tokenChangedValue(const lib::vector2du32 &position, lib::sptr<Tile> tile,
		const lib::board::BoardTileData &ov, const lib::board::BoardTileData &nv)
	{
		tile->getAsEllipseShape()->setFillColor(tile->getColorForToken());
	}

	void GameScene::playerMoved(const lib::vector2du32 &source, const lib::vector2du32 &dest, lib::sptr<Player> player_)
	{
		auto player = player_->getAsEllipseShape();
		player->setOrigin(tileSize() / 2.0f);
		player->setPosition(board2Scene(dest) + (tileSize() / 2.0f));
		player->setRotation(player_->currentDirection().angle());
	}

	void GameScene::playerAppeared(const lib::vector2du32 &position, lib::sptr<Player> player)
	{
		player->setPosition(board2Scene(position));
	}

	void GameScene::playerDissapeared(const lib::vector2du32 &position, lib::sptr<Player> player)
	{

	}

	void GameScene::playerChangedValue(const lib::vector2du32 &position, lib::sptr<Player> player,
		const lib::board::BoardTileData &ov, const lib::board::BoardTileData &nv)
	{
		player->getAsEllipseShape()->setFillColor(player->getColorForToken());
	}
}

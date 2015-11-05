#include "utils.hpp"

namespace zoper
{
	KeyMapping::KeyMapping() : lib::Configuration{ false }
	{
	}


	KeyMapping::~KeyMapping()
	{
	}

	void KeyMapping::load()
	{
		std::array<sf::Keyboard::Key, Direction::Total+1> defaults{ sf::Keyboard::Key::Left,sf::Keyboard::Key::Right,sf::Keyboard::Key::Up,sf::Keyboard::Key::Down,sf::Keyboard::Key::Space };

		for (auto i = 0u; i < (Direction)Total; ++i)
		{
			_keys[i] = static_cast<sf::Keyboard::Key>(addConfigInt("key"+std::to_string(i),defaults[i]));
		}
	}

	sf::Keyboard::Key KeyMapping::getKey(Direction d) const
	{
		return _keys[d];
	}
	Direction KeyMapping::getDirectionFromKey(sf::Keyboard::Key k) const
	{
		for (auto i = 0u; i < (Direction)Total; ++i)
		{
			if (_keys[i] == k)
			{
				return (Direction)i;
			}
		}
		return Direction::Total;
	}
}

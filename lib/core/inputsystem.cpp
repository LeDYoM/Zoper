#include "inputsystem.hpp"
#include "log.hpp"


namespace lib
{
	namespace input
	{
		InputSystem::InputSystem()
		{
		}

		InputSystem::~InputSystem()
		{
		}

		void InputSystem::keyPressed(const Key key)
		{
			__ASSERT(key < Key::KeyCount, "Incorrect key value");
			m_keyStates[key] = true;
			m_pressedKeys.push_back(key);
		}

		void InputSystem::keyReleased(const Key key)
		{
			__ASSERT(key < Key::KeyCount, "Incorrect key value");
			m_keyStates[key] = false;
			m_releasedKeys.push_back(key);
		}

		void InputSystem::preUpdate()
		{
		}

		void InputSystem::postUpdate()
		{
			m_pressedKeys.clear();
			m_releasedKeys.clear();
		}
	}
}

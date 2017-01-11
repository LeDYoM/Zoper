#include "menumanager.hpp"
#include "choosecontrol.hpp"

#include <lib/core/log.hpp>
#include <lib/core/host.hpp>
#include <lib/core/resourcemanager.hpp>

#include <lib/core/events/inputevent.hpp>

namespace lib
{
	namespace menu
	{
		MenuManager::MenuManager(const std::string &name, Theme theme)
			: draw::Scene{ name }, m_theme( theme )
		{
			m_theme.font = host().resourceManager().getFont("game_menu.mainFont");
			m_theme.textColor = draw::colors::Blue;
			m_theme.selectedTextColor = draw::colors::Red;
			m_theme.chSize = 70;
			m_theme.incY = 1;
			m_theme.cursorDescriptor = CursorDescriptor{ 3, vector2df{ 90.0f, 90.0f },draw::colors::Red };

			addSubscription(events::KeyPressedEvent::subscribe([this](const events::Event&ev) {
				logDebug("Key pressed toMenuManager");
				const auto &kEvent{ dynamic_cast<const events::KeyPressedEvent&>(ev) };
				if (kEvent.key == input::Key::Down || kEvent.key == input::Key::Numpad2) {
					m_activeMenuStep->goDown();
				}
				else if (kEvent.key == input::Key::Up || kEvent.key == input::Key::Numpad8) {
					m_activeMenuStep->goUp();
				}
				else if (kEvent.key == input::Key::Left || kEvent.key == input::Key::Numpad4) {
					m_activeMenuStep->goLeft();
				}
				else if (kEvent.key == input::Key::Right || kEvent.key == input::Key::Numpad8) {
					m_activeMenuStep->goRight();
				}
				else if (kEvent.key == input::Key::Return || kEvent.key == input::Key::Space) {
					if (m_activeMenuStep->m_onSelected) {
						m_activeMenuStep->m_onSelected(m_activeMenuStep->_cursorItemSelected);
					}
				}
			}));
		}

		MenuManager::~MenuManager() = default;

		void MenuManager::addMenuSteps(const std::vector<sptr<ChooseControl>> &steps)
		{
			for (auto& menuStep : steps)
			{
				addMenuStep(menuStep);
			}
		}

		void MenuManager::addMenuStep(sptr<ChooseControl> step)
		{
			addRenderGroup(step);
			m_steps.push_back(step);
		}

		void MenuManager::start(sptr<ChooseControl> &firstStep)
		{
			__ASSERT(firstStep, "Cannot start in nullptr step");
			changeStep(firstStep);
		}

		void MenuManager::start(const std::string &firstStep)
		{
			changeStep(firstStep);
		}

		void MenuManager::changeStep(const std::string &step)
		{
			for (auto& nstep : m_steps) {
				if (nstep->name() == step) {
					changeStep(nstep);
					break;
				}
			}
		}

		const Theme & MenuManager::currentTheme() const noexcept
		{
			return m_theme;
		}

		void MenuManager::changeStep(sptr<ChooseControl> &step)
		{
			setActiveStep(step);
		}

		void MenuManager::setActiveStep(sptr<ChooseControl> &step)
		{
			m_activeMenuStep = step;

			for (const auto &_step : m_steps) {
				_step->setVisible(_step == step);
			}
		}
	}
}
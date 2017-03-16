#include "choosecontrolgroup.hpp"
#include "choosecontrol.hpp"
#include "optionmodel.hpp"
#include <lib/core/log.hpp>
#include <lib/core/host.hpp>
#include <lib/core/resourcemanager.hpp>
#include <lib/core/events/inputevent.hpp>


namespace lib
{
	namespace gui
	{
		ChooseControlGroup::ChooseControlGroup(draw::SceneNode *parent, str_const name, Theme theme)
			: draw::SceneNode{ parent, std::move(name) }, m_theme( theme ) { }

		void ChooseControlGroup::create()
		{
			parentScene()->addSubscription(events::KeyPressedEvent::subscribe([this](const events::Event&ev) {
				logDebug("Key pressed toChooseControlGroup");
				const auto &kEvent{ dynamic_cast<const events::KeyPressedEvent&>(ev) };
				auto node(m_sController->activeNode()->snCast<ChooseControl>());
				if (kEvent.key == input::Key::Down || kEvent.key == input::Key::Numpad2) {
					node->goDown();
				}
				else if (kEvent.key == input::Key::Up || kEvent.key == input::Key::Numpad8) {
					node->goUp();
				}
				else if (kEvent.key == input::Key::Left || kEvent.key == input::Key::Numpad4) {
					node->goLeft();
				}
				else if (kEvent.key == input::Key::Right || kEvent.key == input::Key::Numpad8) {
					node->goRight();
				}
				else if (kEvent.key == input::Key::Return || kEvent.key == input::Key::Space) {
					logDebug("Calling onSelected with currentSelection(): ", currentSelection());
					OptionModelIndex currentSelected{ currentSelection() };
					__ASSERT(currentSelected.size() > 1, "currentSelected size must be > 1");
					OptionModelIndex predefinedPath{ options()[currentSelected[0]][currentSelected[1]].next };
					OptionModelIndex resultIndices(onSelected(OptionModelIndex(currentSelection())));
					logDebug("The onSelect returned ", resultIndices);
					if (resultIndices.empty()) {
						resultIndices = std::move(predefinedPath);
					}
					if (!resultIndices.empty()) {
						if (resultIndices[0] != m_sController->activeNodeIndex()) {
							m_sController->activeNodeIndex = resultIndices[0];
						}
						if (resultIndices.size() > 1) {
							m_sController->activeNode()->snCast<ChooseControl>()->selectedItem = resultIndices[1];
						}
					}
				}
			}));
		}

		void ChooseControlGroup::configure()
		{
			box.setCallback([this]() {
				for_each_group([this](const sptr<SceneNode>&sceneNode) {
					if (auto chControl = sceneNode->snCast<ChooseControl>()) {
						chControl->box = box();
					}
				});
			});
			m_theme.font = host().resourceManager().getFont("game_menu.mainFont");
			m_theme.textColor = draw::colors::Blue;
			m_theme.selectedTextColor = draw::colors::Red;
			m_theme.chSize = 70;
			m_theme.incY = 1;
			m_theme.cursorDescriptor = CursorDescriptor{ 3, vector2df{ 90.0f, 90.0f },draw::colors::Red };
			u32 count{ 0 };

			for (const auto& option : options()) {
				auto chooseControl = createSceneNode<ChooseControl>("chooseControl"+std::to_string(count));
				chooseControl->options = option;
				chooseControl->box = box();
				chooseControl->configure();
				++count;
			}

			// Use an state controller to manager which node is active
			m_sController = muptr<StatesController>();
			// Set the nodes to it
			m_sController->nodes = &sceneNodes();
			// Set the index 0 as start node
			m_sController->activeNodeIndex = 0;
			// Set it ready to work
			m_sController->configure();

			// Set the user visible property able to modify the internal state
			currentControlIndex.setCallback([this]() { m_sController->activeNodeIndex = currentControlIndex(); });
		}

		ChooseControlGroup::~ChooseControlGroup() = default;

		const OptionModelIndex ChooseControlGroup::currentSelection() const noexcept
		{
			std::vector<u32> temp{ m_sController->activeNodeIndex() };
			std::vector<u32> controlSelection{m_sController->activeNode()->snCast<ChooseControl>()->currentSelection() };
			temp.reserve(temp.size() + controlSelection.size());
			temp.insert(temp.end(), controlSelection.begin(), controlSelection.end());
			return temp;
		}

		const Theme & ChooseControlGroup::currentTheme() const noexcept
		{
			return m_theme;
		}
	}
}

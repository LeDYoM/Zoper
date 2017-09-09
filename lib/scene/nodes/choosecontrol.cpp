#include "choosecontrol.hpp"
#include "choosecontrolgroup.hpp"
#include <mtypes/include/log.hpp>
#include <lib/scene/scene.hpp>
#include <lib/scene/ianimation.hpp>
#include <lib/scene/nodes/choosecontrolline.hpp>
#include <lib/scene/renderizables/nodeshape.hpp>
#include <lib/scene/renderizables/nodetext.hpp>
#include <lib/scene/renderizables/discretetext.hpp>

namespace lib
{
	namespace scene
	{
		namespace nodes
		{

			ChooseControl::ChooseControl(lib::scene::SceneNode *parent, str name)
				: SceneNode{ parent, std::move(name) } {}

			ChooseControl::~ChooseControl() = default;

			void ChooseControl::create()
			{
				box.setCallback([this]() {
					for_each_node([this](const sptr<Renderizable>&node) {
						if (auto chControlLine = node->rnCast<ChooseControlLine>()) {
							chControlLine->setAlignmentBox(box());
						}
					});
				});
				const auto &cTheme(dynamic_cast<ChooseControlGroup*>(parent())->currentTheme());
				descriptorCursorSize = cTheme.cursorDescriptor.m_size;
				optionModel.setCallback([this]() {
					modelChanged();
				});
			}

			const OptionModelIndex ChooseControl::currentSelection() const noexcept
			{
				vector<u32> lineSelection{ lines[selectedItem()]->currentSelection() };
				vector<u32> temp{ selectedItem() };
				temp.reserve(temp.size() + lineSelection.size());
				if (!lineSelection.empty())
					temp.insert(std::move(lineSelection));
				return temp;
			}

			u32 ChooseControl::selectedSubLabel(const u32 index) const
			{
				__ASSERT(index < lines.size(), "Invalid index");
				return lines[index]->m_option->index();
			}

			void ChooseControl::goDown()
			{
				if (selectedItem() < (lines.size() - 1)) {
					selectedItem = selectedItem() + 1;
				}
				else {
					selectedItem = 0;
				}
			}

			void ChooseControl::goUp()
			{
				if (selectedItem() > 0) {
					selectedItem = selectedItem() - 1;
				}
				else {
					selectedItem = lines.size() - 1;
				}
			}

			void ChooseControl::goLeft()
			{
				currentLine()->m_option->decrementIndex();
			}

			void ChooseControl::goRight()
			{
				currentLine()->m_option->incrementIndex();
			}

			const sptr<ChooseControlLine> ChooseControl::currentLine() const
			{
				return lines[selectedItem()];
			}

			const sptr<ChooseControlLine> ChooseControl::previouscurrentLine() const
			{
				return lines[previouslySelectedItem];
			}

			void ChooseControl::modelChanged()
			{
				clearNodes();
				/*
				// Disable cursor
				m_cursorNode = createSceneNode("cursorNode");
				m_cursor = m_cursorNode->createRenderizable<nodes::NodeShape>("cursor", cTheme.cursorDescriptor.m_nVertex);
				m_cursor->box = { 1000, 100, descriptorCursorSize.x, descriptorCursorSize.y };
				m_cursor->color = cTheme.cursorDescriptor.m_color;
				*/
				u32 count{};
				vector2df currentPos{};
				const auto &cTheme(parent()->snCast<ChooseControlGroup>()->currentTheme());
				for (const auto& label : optionModel())
				{
					auto menuLine = createSceneNode<ChooseControlLine>("menuLineText" + str(count));
					menuLine->create();
					menuLine->mainText()->text = label.text;
					menuLine->setFont(cTheme.font);
					menuLine->setCharacterSize(cTheme.chSize);
					menuLine->setColor(cTheme.textColor);
					menuLine->setAlignmentBox(box().moved(currentPos));
					menuLine->option()->data = label.subOptionsLabels;
					menuLine->configure();

					currentPos.y += (cTheme.chSize + cTheme.incY);
					lines.push_back(std::move(menuLine));
					++count;
				}

				previouslySelectedItem = 0;
				selectedItem.setCallback([this]() {
					if (!lines.empty()) {
						__ASSERT(previouslySelectedItem < lines.size(), "Invalid previously selected index for cursor");
						__ASSERT(selectedItem() < lines.size(), "Invalid select index for cursor");

						const auto &cTheme(parent()->snCast<ChooseControlGroup>()->currentTheme());

						previouscurrentLine()->setColor(cTheme.textColor);
						currentLine()->setColor(cTheme.selectedTextColor);
						previouslySelectedItem = selectedItem();

						//			m_cursorNode->rotation.set(90);
						//			auto p(vector2df{ selectedText->position().x - descriptorCursorSize.x, selectedText->position().y });
						//			addAnimation(msptr<anim::IPropertyAnimation<vector2df>>(120, m_cursorNode->position,
						//				m_cursorNode->position(), vector2df{ selectedText->position().x - descriptorCursorSize.x, selectedText->position().y },
						//				anim::noAction, anim::noAction));
					}
				});
				selectedItem = 0;
			}
		}
	}
}
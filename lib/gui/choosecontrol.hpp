#ifndef LIB_GUI_CHOOSECONTROL_HPP__
#define LIB_GUI_CHOOSECONTROL_HPP__

#include <lib/include/types.hpp>
#include <lib/draw/scenenode.hpp>
#include <lib/draw/nodes/nodetext.hpp>
#include <lib/draw/nodes/nodeshape.hpp>
#include <vector>
#include <functional>

namespace lib
{
	namespace core
	{
		class Resource;
	}
	namespace gui
	{
		class OptionDescriptor
		{
		public:
			explicit OptionDescriptor(const std::string &text, bool createSubString = false, u32 startValueIndex = 0,
				const std::vector<std::string> &subOptionsLabels = std::vector<std::string>())
				: _text(text), _createSubString{ createSubString }, _startValueIndex{ startValueIndex },
				_subOptionsLabels(subOptionsLabels) {}

			std::string _text;
			bool _createSubString;
			u32 _startValueIndex;
			std::vector<std::string> _subOptionsLabels;
		};
		class MenuManager;
		class ChooseControl : public draw::SceneNode
		{
		public:
			ChooseControl(MenuManager *parent, std::string name, 
				std::function<void(const u32)> onSelected, const std::vector<sptr<OptionDescriptor>> labels);
			virtual ~ChooseControl();

			MenuManager *menuManager() const;

			u32 selectedSubLabel(const u32 index) const;

		private:
			void updateSubLabelText(const u32 index);
			void cursorSelectItem(const u32 nodeIndex);
			void goDown();
			void goUp();
			void goLeft();
			void goRight();

			vector2df descriptorCursorSize;
			u32 _cursorItemSelected{ 0 };

			struct LabelData
			{
				std::vector<std::string> textSubLabel;
				sptr<draw::nodes::NodeText> subLabel{ nullptr };
				sptr<draw::nodes::NodeText> label{ nullptr };
				u32 selectedSublabel{ 0 };
				LabelData(const std::vector<std::string> &textSubLevel_, sptr<draw::nodes::NodeText> subLabel_,
					sptr<draw::nodes::NodeText> label_, const u32 selectedSubLabel_)
					: textSubLabel{ textSubLevel_ }, subLabel{ subLabel_ }, label{ label_ }, selectedSublabel{ selectedSubLabel_ } {}
			};

			std::vector<LabelData> m_labelData;
			draw::SceneNodeSPtr m_cursorNode;
			sptr<draw::nodes::NodeShape> m_cursor;
			std::function<void(const u32)> m_onSelected;

			friend class MenuManager;
		};
	}
}

#endif

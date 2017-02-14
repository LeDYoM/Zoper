#include "nodealignedtext.hpp"

#include <lib/draw/font.hpp>
#include <lib/draw/texture.hpp>

#include <cmath>

namespace lib
{
	namespace draw
	{
		namespace nodes
		{
			NodeAlignedText::NodeAlignedText(str_const name, std::string txt, sptr<Font> font_, u32 characterSize_, const Color &color, Rectf32 alignBox, AlignmentX alignmentX_, AlignmentY alignmentY_) :
				NodeText{ std::move(name), std::move(txt), font_, characterSize_, color }, 
				alignmentX{ alignmentX_,[this]() { updateGeometry(); } }, alignmentY{ alignmentY_,[this]() {updateGeometry(); } }, alignmentBox{ std::move(alignBox),[this]() {updateGeometry(); } }
			{
				logConstruct("Name: ", name);

				text.setCallback([this]() { updateGeometry(); });

				updateGeometry();
			}

			NodeAlignedText::~NodeAlignedText()
			{
				logDestruct("Name: ", name());
			}

			void NodeAlignedText::updateGeometry()
			{
				NodeText::updateGeometry();
				updateAlignmentX();
				updateAlignmentY();
			}

			void NodeAlignedText::updateAlignmentX()
			{
				// To be called only after text set
				switch (alignmentX())
				{
				default:
				case AlignmentX::Left:
					m_vertices.moveX(alignmentBox().left);
					break;
				case AlignmentX::Center:
				{
					const auto &abox{ alignmentBox() };
					m_vertices.moveX(abox.left + (abox.width / 2.f) - (bounds().width / 2));
				}
					break;
				case AlignmentX::Right:
					m_vertices.moveX(alignmentBox().right() - bounds().right());
					break;
				}
			}

			void NodeAlignedText::updateAlignmentY()
			{
				// To be called only after text set
				switch (alignmentY())
				{
				default:
				case AlignmentY::Top:
					m_vertices.moveY(alignmentBox().top);
					break;
				case AlignmentY::Middle:
				{
					const auto &abox{ alignmentBox() };
					m_vertices.moveY(abox.top + (abox.height / 2.f) - (bounds().height / 2));
				}
					break;
				case AlignmentY::Bottom:
					m_vertices.moveY(alignmentBox().bottom() - bounds().bottom());
					break;
				}
			}
		}
	}
}
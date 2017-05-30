#include "isimplenode.hpp"
#include <lib/core/log.hpp>
#include <lib/scene/texture.hpp>

namespace lib
{
	namespace scene
	{
		namespace nodes
		{
			ISimpleNode::ISimpleNode(str &&name, const u32 pointCount)
				: Renderizable{ std::move(name), TriangleFan, pointCount + 2 } {}

			void ISimpleNode::configureBase()
			{
				if (texture()) {
					auto tSize(texture()->size());
					if ((texture().get() && textureRect() == Rects32{})) {
						textureRect = { 0, 0, static_cast<s32>(tSize.x), static_cast<s32>(tSize.y) };
					}
				}

				texture.setCallback([this]()
				{
					if (texture()) {
						auto tSize(texture()->size());
						if ((!texture().get() && textureRect() == Rects32{})) {
							textureRect = { 0, 0, static_cast<s32>(tSize.x), static_cast<s32>(tSize.y) };
						}
					}
				});
				textureRect.setCallback([this]() { updateTextureCoords(); });
				box.setCallback([this]() { updateGeometry(); });
				Renderizable::configureBase();
			}

			void ISimpleNode::updateGeometry()
			{
				updateGeometrySimpleNode();

				color.update();

				// Texture coordinates
				updateTextureCoords();
			}

			void ISimpleNode::updateTextureCoords()
			{
				const Rectf32 bounds(bounds());
				BasicVertexArray& vertices{ m_vertices.verticesArray() };
				if (!vertices.empty()) {
					for (auto &v : vertices) {
						const f32 xratio((v.position.x - bounds.left) / bounds.width);
						const f32 yratio((v.position.y - bounds.top) / bounds.height);
						v.texCoords.x = textureRect().left + (textureRect().width * xratio);
						v.texCoords.y = textureRect().top + (textureRect().height * yratio);
					}
				}
			}
		}
	}
}
#include "boardgroup.hpp"
#include <lib/scene/renderizables/nodeshape.hpp>

namespace zoper
{
	using namespace lib;
	using namespace lib::scene;
	using namespace lib::board;

	u32 Tile::m_tileCounter{ 0 };

	Tile::Tile(SceneNode* const parent, str name, BoardTileData data, const Rectf32 &box) :
		GameBaseTile{ parent, name + str(m_tileCounter) + str(m_tileCounter), data }
	{
		++m_tileCounter;
		m_node = createRenderizable<nodes::NodeShape>("Node", 30);
		m_node->box = box;
		m_node->color = getColorForToken();
	}

	Tile::~Tile() = default;

	void Tile::resetTileCounter()
	{
		m_tileCounter = 0;
	}
}
#ifndef __LIB_STATES_CONTROLLER_INCLUDE_HPP__
#define __LIB_STATES_CONTROLLER_INCLUDE_HPP__

#pragma once

#include <vector>
#include <lib/include/types.hpp>
#include <lib/draw/scene.hpp>

namespace lib
{
	namespace gui
	{
		class StatesController
		{
		public:
			void configure();

			Property<vector_shared_pointers<draw::SceneNode>> nodes;
			Property<u32> activeNodeIndex;

			const sptr<draw::SceneNode> &activeNode() const noexcept { 
				__ASSERT(nodes().size() < activeNodeIndex(), "Accessing invalid index");
				return nodes()[activeNodeIndex()]; 
			}
		private:
			u32 m_lastActiveNodeIndex{};
		};
	}
}

#endif

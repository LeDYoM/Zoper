#pragma once

#ifndef LIB_COMPONENT_DISCRETE_TEXT_INCLUDE_HPP__
#define LIB_COMPONENT_DISCRETE_TEXT_INCLUDE_HPP__

#include <mtypes/include/types.hpp>
#include "parentrendercomponent.hpp"
#include <lib/scene/scenenodetypes.hpp>
#include <lib/scene/renderizables/nodetext.hpp>

namespace lib::scene
{
	class DiscreteTextComponent : public RenderizableSceneNodeComponent<nodes::NodeText>
	{
	public:
		using BaseClass = RenderizableSceneNodeComponent<nodes::NodeText>;
		DiscreteTextComponent() {}
		virtual ~DiscreteTextComponent() {}

		virtual void onAttached() override {
			BaseClass::onAttached();
		}

		Property<bool> circleAroud{ true };
		Property<u32> index;
		Property<string_vector> data;
		void incrementIndex() noexcept;
		void decrementIndex() noexcept;
	private:
		void _setText(const str&nText);
	};
}

#endif
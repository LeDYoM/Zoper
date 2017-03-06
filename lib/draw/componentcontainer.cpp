#include "componentcontainer.hpp"
#include <algorithm>

namespace lib
{
	namespace draw
	{
		namespace
		{
			inline std::type_index tindexOf(const sptr<IComponent> &c)
			{
				return std::type_index(typeid(*c));
			}

			inline sptr<IComponent> getComponentFromTypeIndex(const std::type_index &tindex, const vector_shared_pointers<IComponent>&v)
			{
				auto iterator (std::find_if(v.cbegin(), v.cend(), [&tindex](const sptr<IComponent> &component) { return tindexOf(component) == tindex; }));
				return (iterator == v.cend()) ? nullptr : (*iterator);
			}

			inline bool containsComponentOfType(const std::type_index &tindex, const vector_shared_pointers<IComponent>&v)
			{
				return (std::find_if(v.cbegin(), v.cend(), [&tindex](const sptr<IComponent> &component) { return tindexOf(component) == tindex; })) != v.cend();
			}

			inline bool containsComponentOfType(const sptr<IComponent> &c, const vector_shared_pointers<IComponent>&v)
			{
				return containsComponentOfType(tindexOf(c),v);
			}
		}
		ComponentContainer::ComponentContainer(SceneNode *sceneNode) noexcept
			: m_sceneNode{ sceneNode } {}

		ComponentContainer::~ComponentContainer() 
		{ 
			m_sceneNode = nullptr; 
			m_components.clear();
		}

		bool ComponentContainer::addComponent(sptr<IComponent> nc)
		{
			__ASSERT(nc, "Trying to add a nullptr component");
			const auto& tinfo(typeid(*nc));
			const std::type_index tindex(tinfo);
			
			logDebug("Adding component of type ", tinfo.name()," wich has an index of ",tindex.hash_code());
			logDebug("Searching for another component of the same type...");
			
			sptr<IComponent> component(std::move(nc));
			const bool alreadyInConainer(containsComponentOfType(component,m_components));

			if (!alreadyInConainer) {
				component->m_sceneNode = m_sceneNode;
				m_components.push_back(std::move(component));
				return true;
			}
			return false;
		}

		void ComponentContainer::updateComponents()
		{
			for (auto &component : m_components) {
				component->update();
			}
		}
		const sptr<IComponent> ComponentContainer::componentOfType(const std::type_index & ti)
		{
			return getComponentFromTypeIndex(ti, m_components);
		}
	}
}
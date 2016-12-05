#include "renderstates.hpp"
#include "transformation.hpp"
#include "texture.hpp"

namespace lib
{
	namespace draw
	{
		RenderStates::RenderStates() = default;
		RenderStates::~RenderStates() = default;

		void RenderStates::newFrame()
		{
			m_renderStates = sf::RenderStates{};
		}
		sf::RenderStates RenderStates::prepareNewElement(const Transform &transform, sptr<Texture> texture)
		{
			return prepareNewElement(transform, texture.get());
		}
		sf::RenderStates RenderStates::prepareNewElement(const Transform & transform, const sf::Texture * texture)
		{
			return sf::RenderStates(m_renderStates.blendMode, m_renderStates.transform*transform, texture, m_renderStates.shader);
		}
	}
}
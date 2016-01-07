#ifndef __LIB_RENDERGROUP_HPP__
#define __LIB_RENDERGROUP_HPP__

#include "../../types.hpp"
#include "../../core/HasName.hpp"
#include "idrawable.hpp"
#include "anim/animationmanager.hpp"
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>

namespace lib
{
	namespace core
	{
		class Window;
	}
	namespace scn
	{
		namespace draw
		{
			class Renderizable;
			class RenderGroup : public lib::core::HasName, public IDrawable, public anim::AnimationManager, public sf::Transformable
			{
			public:
				RenderGroup(const std::string &name, RenderGroup *parent=nullptr);
				virtual ~RenderGroup();

				sptr<draw::Renderizable> createText(const std::string &name);
				sptr<draw::Renderizable> createShape(const std::string &name,const sf::Vector2f &radius=sf::Vector2f(),u32 pointCount=30);
				sptr<draw::Renderizable> createSpriteShape(const std::string &name, const sf::Vector2f &radius = sf::Vector2f());
				sptr<draw::Renderizable> addRenderizable(sptr<Renderizable> newElement);
				bool removeRenderizable(sptr<Renderizable> element);
				void clear();

				sptr<RenderGroup> createNewRenderGroup(const std::string &name);
				bool removeRenderGroup(sptr<RenderGroup> element);

				bool putOnTop(sptr<IDrawable> node);
				bool putonBottom(sptr<IDrawable> node);

				u32 draw(lib::core::Window *window, sf::RenderStates &states) override;

			protected:
				void addRenderGroup(sptr<RenderGroup> node);

				RenderGroup *parent() const { return _parent; }
				std::vector<sptr<IDrawable>> _renderNodes;

			private:
				RenderGroup *_parent{ nullptr };
			};
		}
	}
}

#endif

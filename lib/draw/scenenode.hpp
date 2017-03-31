#ifndef LIB_DRAW_SCENENODE_HPP__
#define LIB_DRAW_SCENENODE_HPP__

#pragma once

#include <lib/include/types.hpp>
#include <lib/include/vector2d.hpp>
#include <lib/draw/transformable.hpp>
#include <lib/draw/color.hpp>
#include <lib/draw/hasname.hpp>
#include "componentcontainer.hpp"

namespace lib
{
	namespace draw
	{
		class IComponent;
		class Scene;
		class Renderizable;

		/** \brief Main class representing all SceneNodes from a Scene.
		* This class is that serves as main entry point in the hierarchy of the scene
		*/
		class SceneNode : public core::HasName, public Transformable, public ComponentContainer
		{
		public:
			SceneNode(const SceneNode&) = delete;
			SceneNode(SceneNode *const parent, str_const name);
			virtual ~SceneNode();

			virtual void create() {}
			virtual void configure() {}

			/**
			* Method to add a user defined renderizable
			* @params args Arguments to be passed to the constructor
			* @returns The created renderizable
			*/
			template <typename T, typename... Args>
			sptr<T> createRenderizable(Args&&... args)
			{
				auto result(msptr<T>(std::forward<Args>(args)...));
				addRenderizable(result);
				return result;
			}

			bool removeRenderizable(const sptr<Renderizable> &element);
			void clear();

			template <typename T = SceneNode, typename... Args>
			sptr<T> createSceneNode(str_const name, Args&&... args)
			{
				auto result(msptr<T>(this, std::move(name), std::forward<Args>(args)...));
				addSceneNode(result);
				return result;
			}

			sptr<SceneNode> createSceneNode(str_const name)
			{
				return createSceneNode<SceneNode>(std::move(name));
			}

			bool moveLastBeforeNode(const sptr<SceneNode> &beforeNode);
			bool removeSceneNode(const sptr<SceneNode> &element);

			void draw();

			Property<bool> visible;

			virtual Scene *const parentScene() { return m_parent->parentScene(); }
			const Rectf32 &scenePerspective();

			/**
			* Set a color for all vertex for all child nodes of this node
			* @param [in] color to be set
			* @param [in] apply the function to scenenodes too
			* @
			*/
			void setColor(const Color &color, const bool applySceneNodes = true);
			inline SceneNode *parent() noexcept { return m_parent; }
			inline const SceneNode *parent() const noexcept { return m_parent; }

			template <typename T>
			constexpr T *const snCast() { return dynamic_cast<T *const>(this); }

			template <typename T>
			constexpr const T *const snCast() const { return dynamic_cast<const T *const>(this); }

			void for_each_node(std::function<void(const sptr<Renderizable> &)> action);
			void for_each_group(std::function<void(const sptr<SceneNode> &)> action);

		protected:

			const vector_shared_pointers<Renderizable> &renderNodes() const noexcept { return m_renderNodes; }
			const vector_shared_pointers<SceneNode> &sceneNodes() const noexcept { return m_groups; }

			void addRenderizable(const sptr<Renderizable> &newElement);
			void addSceneNode(sptr<SceneNode> node);

		private:

			vector_shared_pointers<Renderizable> m_renderNodes;
			vector_shared_pointers<SceneNode> m_groups;

			SceneNode *m_parent{ nullptr };
			bool m_visible{ true };

		};

		using SceneNodeSPtr = sptr<SceneNode>;
	}
}

#endif

#ifndef HAF_SCENE_TEXTQUAD_INCLUDE_HPP
#define HAF_SCENE_TEXTQUAD_INCLUDE_HPP

#include <haf/scene_nodes/include/tablenode.hpp>
#include <haf/scene_nodes/include/scenenodetext.hpp>
#include <haf/scene_nodes/include/text_table_node.hpp>

namespace haf::scene::nodes
{
class TextQuad : public TextTableNode
{
    using BaseClass = TextTableNode;
public:
    using BaseClass::BaseClass;
    using BaseClass::prop;

    void onCreated() override;

    inline htps::sptr<SceneNodeText> text(htps::vector2dst index) noexcept
    {
        return nodeAt(std::move(index));
    }
};
}  // namespace haf::scene::nodes

#endif
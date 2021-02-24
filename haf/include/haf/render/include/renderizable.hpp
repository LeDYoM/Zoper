#ifndef HAF_SCENE_RENDERIZABLE_INCLUDE_HPP
#define HAF_SCENE_RENDERIZABLE_INCLUDE_HPP

#include <htypes/include/types.hpp>
#include <htypes/include/propertystate.hpp>
#include <htypes/include/function.hpp>
#include <htypes/include/p_impl_pointer.hpp>

#include <haf/render/include/renderizable_modifier_context.hpp>
#include <haf/scene/include/scenenodeparent.hpp>
#include <haf/scene/include/color.hpp>
#include <haf/scene/include/hasname.hpp>
#include <haf/render/include/fig_type.hpp>
#include <haf/render/include/vertexarray.hpp>
#include <haf/resources/include/itexture.hpp>
#include <haf/resources/include/ishader.hpp>
#include <haf/render/include/renderdata.hpp>

namespace haf::scene
{
class SceneNode;

class Renderizable final : public sys::HasName, SceneNodeParent
{
public:
    Renderizable(htps::rptr<SceneNode> parent,
                 htps::str name,
                 FigType_t figure_type,
                 htps::size_type initial_point_count,
                 htps::Rectf32 _box,
                 Color color,
                 htps::sptr<res::ITexture> _texture,
                 htps::sptr<res::IShader> _shader);

    ~Renderizable();

    void render();

    htps::PropertyState<FigType_t> figType;
    htps::PropertyState<htps::Rectf32> box;
    htps::PropertyState<Color> color;
    htps::PropertyState<htps::size_type> pointCount;
    htps::PropertyState<htps::sptr<res::IShader>> shader;
    htps::PropertyState<
        htps::function<Color(const RenderizableModifierContext&)>>
        color_modifier;

    htps::BasicProperty<bool> visible{true};

    void setTextureAndTextureRect(htps::sptr<res::ITexture> texture_,
                                  const htps::Rectf32& textRect);

    void setTextureFill(htps::sptr<res::ITexture> texture_);

    struct RenderizableInternalData
    {
        FigType_t const& figType;
        htps::Rectf32 const& box;
        Color const& color;
        htps::size_type const& pointCount;
        htps::sptr<res::IShader> const& shader;
        htps::Rects32 const& textureRect;
        htps::sptr<res::ITexture> const& texture;
        htps::function<Color(const RenderizableModifierContext&)> const& color_modifier;
    };

private:
    struct RenderizablePrivate;
    htps::PImplPointer<RenderizablePrivate> p_;

    htps::PropertyState<htps::Rects32> textureRect;
    htps::PropertyState<htps::sptr<res::ITexture>> texture;

    RenderizableInternalData getMomentumInternalData() const;
    VertexArray m_vertices;
    RenderData render_data_;

    void update();
};
}  // namespace haf::scene

#endif

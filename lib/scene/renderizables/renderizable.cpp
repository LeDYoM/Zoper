#include "renderizable.hpp"
#include <lib/system/rendersystem.hpp>

#include <lib/system/scenemanager.hpp>
#include <lib/scene/renderdata.hpp>
#include <lib/scene/scenenode.hpp>
#include <lib/resources/texture.hpp>
#include <lib/resources/shader.hpp>
#include <lib/system/systemprovider.hpp>

#include "geometry_math.hpp"

namespace lib::scene
{
namespace
{
template <typename T>
constexpr int sgn(const T val) noexcept
{
    return (T(0) < val) - (val < T(0));
}

template <typename T>
constexpr int sgn_cos(T angle)
{
    return sgn(std::cos(angle));
}

template <typename T>
constexpr int sgn_sin(T angle)
{
    return sgn(std::sin(angle));
}

constexpr vector2dd getPositionFromAngleAndRadius(
    const FigType_t fig_type,
    const f64 angle, const vector2df &radius)
{
    switch (fig_type)
    {
    default:
    case FigType_t::Quad:
    {
        return {(sgn_cos(angle) * radius.x),
                sgn_sin(angle) * radius.y};
    }
    break;
    case FigType_t::Shape:
    {
        return {std::cos(angle) * radius.x,
                std::sin(angle) * radius.y};
    }
    }
}

constexpr size_type vertexPerFigure(
    const FigType_t fig_type, const size_type num_points)
{
    switch (fig_type)
    {
    case FigType_t::Quad:
    case FigType_t::Shape:
    {
        return num_points + 2U;
    }
    break;
    default:
    {
        return num_points;
    }
    }
}

constexpr size_type primitivePerFigure(
    const FigType_t fig_type, const size_type num_points)
{
    switch (fig_type)
    {
    case FigType_t::Quad:
    case FigType_t::Shape:
    {
        return num_points + 2U;
    }
    break;
    default:
    {
        return num_points;
    }
    }
}

} // namespace

Renderizable::Renderizable(
    rptr<SceneNode> parent, str name, FigType_t figure_type,
    size_type initial_point_count, Rectf32 _box, Color _color,
    sptr<ITexture> _texture, sptr<IShader> _shader)
    : sys::HasName{std::move(name)},
      parent_{std::move(parent)},
      m_vertices{
          PrimitiveType::TriangleFan,
          vertexPerFigure(FigType_t::Quad, initial_point_count)},
      pointCount{initial_point_count},
      box{std::move(_box)},
      color{std::move(_color)},
      texture{std::move(_texture)},
      shader{std::move(_shader)},
      render_data_{m_vertices, parent->globalTransform(), 
        texture().get() != nullptr ? dynamic_cast<Texture *>(texture().get()) : nullptr, 
        shader().get() != nullptr ? dynamic_cast<Shader *>(shader().get()) : nullptr}
{
}

Renderizable::~Renderizable() = default;

void Renderizable::render()
{
    if (visible())
    {
        update();

        if (!m_vertices.empty())
        {
            parent_->sceneManager().systemProvider().renderSystem().draw(render_data_);
        }
    }
}

void Renderizable::setTextureAndTextureRect(sptr<ITexture> texture_, const Rectf32 &textRect)
{
    texture.set(texture_);
    if (texture_)
    {
        textureRect = static_cast<Rects32>(textRect);
    }
}

void Renderizable::setTextureFill(sptr<ITexture> texture_)
{
    if (texture_)
    {
        setTextureAndTextureRect(std::move(texture_),
                                 {0.0f, 0.0f, static_cast<vector2df>(texture_->size())});
    }
}

vector2df Renderizable::normalizeInBox(
    const vector2df &position,
    const Rectf32 box, const Rectf32 &rect) const
{
    const f32 xratio{(position.x - box.left) / box.width};
    const f32 yratio{(position.y - box.top) / box.height};
    return {(rect.left + (rect.width * xratio)),
            (rect.top + (rect.height * yratio))};
}

void Renderizable::updateTextureCoordsAndColorForVertex(
    const BasicVertexArray::iterator v_iterator,
    const Rectf32 &cbox, const Rects32 &ctexture_rect)
{
    auto &dest_vertex = *v_iterator;
    dest_vertex.texCoords = normalizeInBox(dest_vertex.position, cbox, ctexture_rect);
    updateColorForVertex(v_iterator, cbox, ctexture_rect);
}

void Renderizable::updateColorForVertex(
    const BasicVertexArray::iterator v_iterator,
    const Rectf32 &cbox, const Rects32 &ctexture_rect)
{
    Color dest_color{color()};
    if (color_modifier())
    {
        RenderizableModifierContext context{
            box(),
            ctexture_rect,
            texture() ? texture()->size() : vector2du32{0U, 0U},
            *v_iterator};
        dest_color *= color_modifier()(context);
    }
    v_iterator->color = dest_color;
}

void Renderizable::updateTextureCoordsAndColor()
{
    const auto &cbox(box());
    const auto ctexture_rect{textureRect()};

    BasicVertexArray &vertices{m_vertices.verticesArray()};

    for (auto v_iterator = vertices.begin();
         v_iterator != vertices.end(); ++v_iterator)
    {
        updateTextureCoordsAndColorForVertex(v_iterator, cbox, ctexture_rect);
    }
}

void Renderizable::updateColors()
{
    const auto &cbox(box());
    const auto ctexture_rect{textureRect()};

    BasicVertexArray &vertices{m_vertices.verticesArray()};

    for (auto v_iterator = vertices.begin();
         v_iterator != vertices.end(); ++v_iterator)
    {
        updateColorForVertex(v_iterator, cbox, ctexture_rect);
    }
}

void Renderizable::update()
{
    if (ps_readResetHasAnyChanged(box, figType, pointCount))
    {
        updateGeometry();
        textureRect.resetHasChanged();
        color.resetHasChanged();
        color_modifier.resetHasChanged();
    }

    if (ps_readResetHasAnyChanged(textureRect))
    {
        updateTextureCoordsAndColor();
        color.resetHasChanged();
        color_modifier.resetHasChanged();
    }

    if (ps_readResetHasAnyChanged(color, color_modifier))
    {
        updateColors();
    }

    if (ps_readResetHasChanged(texture))
    {
        render_data_.texture = (texture().get() != nullptr) ?
            (dynamic_cast<Texture *>(texture().get())) : nullptr;
    }

    if (ps_readResetHasChanged(shader))
    {
        render_data_.shader = (shader().get() != nullptr) ?
            (dynamic_cast<Shader *>(shader().get())) : nullptr;

    }
}

void Renderizable::updateGeometry()
{
    if (pointCount())
    {
        const Rectf32 &cBox{box()};
        auto &vertices(m_vertices.verticesArray());

        const auto fig_type{figType()};
        const size_type nPoints{pointCount()};
        const size_type nVertex{vertexPerFigure(fig_type, nPoints)};
        const vector2df radius{cBox.size() / 2.0F};

        vertices.resize(nVertex); // + 2 for center and repeated first point
        const f64 baseAngle(PiM2Constant<f64> / static_cast<f64>(nPoints));
        const auto leftTop(cBox.leftTop());
        const auto base_position{leftTop + radius};

        const auto vertices_iterator_begin = m_vertices.verticesArray().begin();
        auto vertices_iterator_second = vertices_iterator_begin;
        auto vertices_iterator{++vertices_iterator_second};
        auto angle{0.0};

        for (size_type i{0U}; i < nPoints; ++i, ++vertices_iterator)
        {
            angle += baseAngle;
            const vector2dd r{getPositionFromAngleAndRadius(fig_type, angle, radius)};
            vertices_iterator->position = base_position + static_cast<vector2df>(r);
            updateTextureCoordsAndColorForVertex(vertices_iterator, cBox, textureRect());
        }

        vertices_iterator->position = vertices_iterator_second->position;
        updateTextureCoordsAndColorForVertex(vertices_iterator, cBox, textureRect());
        vertices_iterator_begin->position = radius + leftTop;
        updateTextureCoordsAndColorForVertex(vertices_iterator_begin, cBox, textureRect());
    }
}
} // namespace lib::scene

#include "renderizable.hpp"
#include <lib/core/host.hpp>
#include <lib/system/window.hpp>
#include <lib/system/rendertarget.hpp>

#include <lib/scene/scenemanager.hpp>
#include <lib/scene/renderdata.hpp>
#include <lib/scene/scenenode.hpp>
#include <lib/resources/texture.hpp>

#include "geometry_math.hpp"

namespace lib::scene
{
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }

    template <typename T> int sgn_cos(T angle) 
    {
        return sgn(std::cos(angle));
    }

    template <typename T> int sgn_sin(T angle) 
    {
        return sgn(std::sin(angle));
    }

    Renderizable::Renderizable(SceneNode * const parent, const str & name, const u32 vertexCount)
        : core::HasName{ name }, m_parent{ parent }, m_vertices{ TriangleFan, vertexCount }
    {
    }

    Renderizable::~Renderizable() = default;

    void Renderizable::render()
    {
        if (visible())
        {
            updateGeometry();

            if (!m_vertices.empty()) 
            {
                m_parent->parentScene()->sceneManager().systemProvider().parentWindow().renderTarget()->
                        draw({
                    m_vertices,
                    m_parent->globalTransform(),
                    dynamic_cast<Texture*>(texture().get())
                });
            }
        }
    }

    void Renderizable::updateColor()
    {
        const Color c{color()};
        if (!m_vertices.empty()) 
        {
            for (auto& v : m_vertices.verticesArray()) 
            {
                v.color = c;
            }
        }
    }

    void Renderizable::updateTextureCoords()
    {
        const auto& localbounds(box());
        BasicVertexArray& vertices{ m_vertices.verticesArray() };
        if (!vertices.empty()) {
            for (auto &v : vertices) {
                const f32 xratio((v.position.x - localbounds.left) / localbounds.width);
                const f32 yratio((v.position.y - localbounds.top) / localbounds.height);
                v.texCoords.x = textureRect().left + (textureRect().width * xratio);
                v.texCoords.y = textureRect().top + (textureRect().height * yratio);
            }
        }
    }

    void Renderizable::setTextureAndTextureRect
        (sptr<ITexture> texture_, const Rectf32& textRect)
    {
        texture.set(texture_);
        if (texture_) {
            auto tSize(texture_->size());
            textureRect = {
                static_cast<s32>(textRect.left),
                static_cast<s32>(textRect.top),
                static_cast<s32>(textRect.width),
                static_cast<s32>(textRect.height)
            };
        }
    }

    vector2dd Renderizable::getPositionFromAngleAndRadius(const f64 angle, const vector2df& radius) const
    {
        switch (figType())
        {
            default:
            case FigType_t::Quad:
            {
                return { (sgn_cos(angle) * radius.x),
                sgn_sin(angle) * radius.y };
            }
            break;
            case FigType_t::Shape:
            {
                return { std::cos(angle) * radius.x,
                    std::sin(angle) * radius.y };
            }
        }
    }

    void Renderizable::setTextureFill(sptr<ITexture> texture_)
    {
        if (texture_)
        {
            setTextureAndTextureRect(std::move(texture_),
                { 0.0f, 0.0f, static_cast<vector2df>(texture_->size()) } );
        }
    }

    void Renderizable::updateGeometry()
    {
        if (ps_readResetHasChanged(box, figType))
        {
            updateGeometrySimpleNode();
            color.setChanged();
            textureRect.setChanged();
        }

        if (textureRect.readResetHasChanged()) 
        {
            updateTextureCoords();
            color.setChanged();
        }

        if (color.readResetHasChanged()) 
        {
            updateColor();
        }
    }

    void Renderizable::updateGeometrySimpleNode()
    {
        if (pointCount()) 
        {
            const Rectf32 &cBox{ box() };
            auto& vertices(m_vertices.verticesArray());

            const size_type nPoints{pointCount()};
            const size_type nVertex{nPoints + 2};

            const vector2df size{ cBox.size() };
            const vector2df radius{ size / 2.0f };

            vertices.resize(nVertex); // + 2 for center and repeated first point
            const f64 baseAngle((2 * PiConstant<f64>) / static_cast<f64>(nPoints));
            const auto leftTop(cBox.leftTop());

            for (size_type i{ 0U }; i < nPoints; ++i) 
            {
                const f64 angle{ (i*baseAngle)  + (PiConstant<f64> / 2.0) };
                const vector2dd r{ getPositionFromAngleAndRadius(angle, radius)  };
                vertices[i + 1].position = leftTop + radius + static_cast<vector2df>(r);
            }

            vertices[nPoints + 1].position = vertices[1].position;
            vertices[0].position = radius + leftTop;
        }
    }
}

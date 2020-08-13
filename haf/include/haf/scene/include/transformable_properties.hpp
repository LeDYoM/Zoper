#pragma once

#ifndef HAF_TRANSFORMABLE_PROPERTIES_INCLUDE_HPP
#define HAF_TRANSFORMABLE_PROPERTIES_INCLUDE_HPP

#include <mtypes/include/grouping_property.hpp>
#include <mtypes/include/vector2d.hpp>
#include <haf/scene/include/matrix4x4.hpp>

namespace haf::scene
{
//    mtps::PropertyState<VectorScalar> origin;
//    mtps::PropertyState<Scalar> rotation;
//    mtps::PropertyState<VectorScalar> scale;
//    mtps::PropertyState<VectorScalar> position;

template <typename Scalar>
struct OriginScalar
{
    using value_type = mtps::vector2d<Scalar>;
};

template <typename Scalar>
struct RotationScalar
{
    using value_type = Scalar;
};

template <typename Scalar>
struct ScaleScalar
{
    using value_type = mtps::vector2d<Scalar>;
};

template <typename Scalar>
struct PositionScalar
{
    using value_type = mtps::vector2d<Scalar>;
};

using Origin   = OriginScalar<Matrix4x4::Scalar>;
using Rotation = RotationScalar<Matrix4x4::Scalar>;
using Scale    = ScaleScalar<Matrix4x4::Scalar>;
using Position = PositionScalar<Matrix4x4::Scalar>;

using TransformableProperties = mtps::PropertyGroup<Origin,
                                                    Rotation,
                                                    Scale,
                                                    Position>;

}  // namespace haf::scene

#endif

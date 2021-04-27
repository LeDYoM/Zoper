#ifndef HTYPES_GROW_POLICY_INCLUDE_HPP
#define HTYPES_GROW_POLICY_INCLUDE_HPP

#include "types.hpp"

namespace htps
{
    class GrowPolicyUnary
    {
    public:
        static constexpr size_type growSize(const size_type size) noexcept
        {
            return size + 1U;
        }
    };

    class GrowPolicyDouble
    {
    public:
        static constexpr size_type growSize(const size_type size) noexcept
        {
            return size > 0U ? (size * 2U) : (size + 1);
        }
    };
}

#endif

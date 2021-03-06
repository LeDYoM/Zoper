#pragma once

#ifndef MTYPES_ALLOCATOR_FUNC_1_INCLUDE_HPP
#define MTYPES_ALLOCATOR_FUNC_1_INCLUDE_HPP

#include "types.hpp"
#include <cassert>

namespace mtps
{

template <typename T, auto M, auto F>
class AllocatorFunc1
{
public:
    using pointer = T*;

    static pointer allocate(const size_type size)
    {
        assert(size > 0U);
        return (pointer)M(sizeof(T) * size);
    }

    static void deallocate(pointer element)
    {
        assert(element != nullptr);
        F(element);
    }
};

}  // namespace mtps

#endif

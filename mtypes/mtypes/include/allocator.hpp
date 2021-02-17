#ifndef MTYPES_ALLOCATOR_INCLUDE_HPP
#define MTYPES_ALLOCATOR_INCLUDE_HPP

#include "placement.hpp"
#include "alloc_func.hpp"

namespace mtps
{

template <typename T, typename ReserveDestroy = AllocatorMallocFree<T>>
class AllocatorType : public ReserveDestroy,
                      public BasicConstructDestruct<T>
{};

}  // namespace mtps

#endif

#pragma once

#ifndef LIB_INPUT_COMPONENT_BASE_INCLUDE_HPP
#define LIB_INPUT_COMPONENT_BASE_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <lib/scene/include/icomponent.hpp>
#include <lib/input/include/key.hpp>

namespace lib::input
{
class InputComponentBase : public scene::IComponent
{
public:
    bool isPressed(const Key key);

    static inline bool isAscii(const Key key) noexcept
    {
        return key >= Key::A && key <= Key::Z;
    }

    char toAscii(const Key key) noexcept;
    bool isShiftPressed() const;
};
} // namespace lib::scene

#endif

#pragma once

#ifndef LIB_COMPONENT_INPUT_INCLUDE_HPP
#define LIB_COMPONENT_INPUT_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/connection.hpp>
#include <lib/input/include/input_component_base.hpp>
#include <lib/input/include/key.hpp>

namespace lib::input
{
class InputComponent : public InputComponentBase
{
public:
    InputComponent();
    ~InputComponent() override;

    void update() override;

    mtps::emitter<const Key &> KeyPressed;
    mtps::emitter<const Key &> KeyReleased;
};
} // namespace lib::input

#endif

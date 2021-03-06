#include <haf/input/include/virtual_input_component.hpp>
#include <haf/input/i_include/inputsystem.hpp>
#include <haf/input/include/key.hpp>

#include <system/i_include/get_system.hpp>

#include <hlog/include/hlog.hpp>

namespace haf::input
{
VirtualInputComponent::VirtualInputComponent()  = default;
VirtualInputComponent::~VirtualInputComponent() = default;

void VirtualInputComponent::update()
{
    if (attachedNode())
    {
        const sys::InputSystem& input_system{
            sys::getSystem<sys::InputSystem>(attachedNode())};

        for (const auto& pressedKey : input_system.pressedKeys())
        {
            onKeyPressed(pressedKey);
        }

        for (const auto& releasedKey : input_system.releasedKeys())
        {
            onKeyReleased(releasedKey);
        }
    }
}
}  // namespace haf::input

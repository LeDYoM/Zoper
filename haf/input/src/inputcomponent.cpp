#include <haf/input/include/inputcomponent.hpp>
#include <input/i_include/inputsystem.hpp>

#include <hlog/include/hlog.hpp>
#include <haf/input/include/key.hpp>
#include <system/i_include/get_system.hpp>

namespace haf::input
{

InputComponent::InputComponent()  = default;
InputComponent::~InputComponent() = default;

void InputComponent::update()
{
    if (attachedNode())
    {
        const sys::InputSystem& input_system{
            sys::getSystem<sys::InputSystem>(attachedNode())};

        for (const auto& pressedKey : input_system.pressedKeys())
        {
            KeyPressed(pressedKey);
        }

        for (const auto& releasedKey : input_system.releasedKeys())
        {
            KeyReleased(releasedKey);
        }
    }
}
}  // namespace haf::input

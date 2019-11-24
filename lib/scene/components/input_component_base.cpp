#include "input_component_base.hpp"

#include <lib/include/liblog.hpp>
#include <lib/include/key.hpp>
#include <lib/system/inputsystem.hpp>
#include <lib/scene/scenenode.hpp>
#include <lib/scene/scene.hpp>
#include <lib/scene/scenemanager.hpp>

namespace lib::scene
{
using namespace input;

bool InputComponentBase::isPressed(const Key key)
{
    if (attachedNode())
    {
        return attachedNode()->inputSystem().keyStates()[KeyIndex(key)];
    }
    return false;
}

bool InputComponentBase::isShiftPressed() const
{
    return false;
}

char InputComponentBase::toAscii(const Key key) noexcept
{
    return static_cast<char>(KeyIndex(key) + (isShiftPressed() ? 127 : 65));
}
} // namespace lib::scene
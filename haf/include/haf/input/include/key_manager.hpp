#ifndef HAF_INPUT_KEY_MANAGER_INCLUDE_HPP
#define HAF_INPUT_KEY_MANAGER_INCLUDE_HPP

#include <mtypes/include/types.hpp>
#include <mtypes/include/vector.hpp>
#include <haf/system/include/idatawrapper.hpp>

#include <haf/input/include/key.hpp>

namespace haf::input
{
class KeyManager : public sys::IDataWrapper
{
public:
    using KeyIndex = mtps::u16;

    bool registerKey(KeyIndex const key_index, Key const key) noexcept;
    Key keyForIndex(KeyIndex const key_index) const noexcept;
private:
    mtps::vector<Key> keys_;
};
}  // namespace haf::input

#endif
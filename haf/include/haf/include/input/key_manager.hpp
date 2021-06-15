#ifndef HAF_INPUT_KEY_MANAGER_INCLUDE_HPP
#define HAF_INPUT_KEY_MANAGER_INCLUDE_HPP

#include <htypes/include/types.hpp>
#include <htypes/include/vector.hpp>
#include <haf/include/system/idatawrapper.hpp>

#include <haf/include/input/key.hpp>

namespace haf::input
{
class KeyManager : public sys::IDataWrapper
{
public:
    using KeyIndex = htps::u16;

    bool registerKey(KeyIndex const key_index, Key const key) noexcept;
    Key keyForIndex(KeyIndex const key_index) const noexcept;
private:
    htps::vector<Key> keys_;
};
}  // namespace haf::input

#endif
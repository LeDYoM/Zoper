#ifndef HEF_WINDOW_WINDOW_PROPERTIES_INCLUDE_HPP
#define HEF_WINDOW_WINDOW_PROPERTIES_INCLUDE_HPP

#include <htypes/include/types.hpp>
#include <htypes/include/basic_property.hpp>
#include <haf/include/system/idatawrapper.hpp>

namespace haf::win
{

class WindowProperties : public sys::IDataWrapper
{
public:
    htps::BasicProperty<htps::u32> width;
    htps::BasicProperty<htps::u32> height;
    htps::BasicProperty<htps::u8> bits_per_red;
    htps::BasicProperty<htps::u8> bits_per_green;
    htps::BasicProperty<htps::u8> bits_per_blue;
    htps::BasicProperty<htps::u8> bits_per_alpha;

    htps::u32 bitsPerPixel() const;

    void onAttached() override;
};

}  // namespace haf::win

#endif
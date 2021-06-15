#ifndef HAF_RESOURCES_RESOURCES_CONFIG_DATA_INCLUDE_HPP
#define HAF_RESOURCES_RESOURCES_CONFIG_DATA_INCLUDE_HPP

#include <htypes/include/types.hpp>
#include <htypes/include/vector.hpp>
#include <htypes/include/str.hpp>
#include <htypes/include/object.hpp>

namespace haf::res
{
struct ResourcesConfigDataElement
{
    htps::str type;
    htps::str file_name;

    static constexpr char TypeStr[]     = "type";
    static constexpr char FileNameStr[] = "filename";
};

inline const htps::Object& operator>>(const htps::Object& obj,
                                      ResourcesConfigDataElement& element)
{
    element.type      = obj[ResourcesConfigDataElement::TypeStr].getValue();
    element.file_name = obj[ResourcesConfigDataElement::FileNameStr].getValue();

    return obj;
}

struct ResourcesConfigData
{
    htps::Object elements_;
};

inline const htps::Object& operator>>(const htps::Object& obj,
                                      ResourcesConfigData& element)
{
    element.elements_ = obj;
    return obj;
}

}  // namespace haf::res

#endif
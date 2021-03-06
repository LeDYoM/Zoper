#include <haf/filesystem/include/fileserializer.hpp>
#include <filesystem/i_include/filesystem.hpp>
#include <system/i_include/systemprovider.hpp>
#include <system/i_include/get_system.hpp>

using namespace mtps;

namespace haf::sys
{
str FileSerializer::loadTextFile(const Path& file_name)
{
    return sys::getSystem<sys::FileSystem>(attachedNode())
        .loadTextFile(file_name);
}

bool FileSerializer::saveFile(const Path& file_name, const str& data)
{
    return sys::getSystem<sys::FileSystem>(attachedNode())
        .saveFile(file_name, data);
}

FileSerializer::Result FileSerializer::deserializeFromFile(
    const Path& file_name,
    shdata::IShareable& data)
{
    const mtps::str text_data{loadTextFile(file_name)};
    if (!text_data.empty())
    {
        mtps::ObjectCompiler obj_compiler(text_data);
        if (obj_compiler.compile())
        {
            // The compilation was correct so, at least we
            // have a valid Object.
            return ((data.deserialize(obj_compiler.result()))
                        ? Result::Success
                        : Result::ParsingError);
        }
        else
        {
            return Result::ParsingError;
        }
    }
    else
    {
        return Result::FileIOError;
    }
}

FileSerializer::Result FileSerializer::serializeToFile(
    const Path& file_name,
    const shdata::IShareable& data)
{
    //        auto temp{mtps::Serializer<T>::serialize(data)};
    mtps::Object obj;
    auto temp(data.serialize(obj));

    if (temp)
    {
        mtps::str data_str;
        data_str << obj;

        return ((saveFile(file_name, std::move(data_str)))
                    ? Result::Success
                    : Result::FileIOError);
    }
    return Result::ParsingError;
}

}  // namespace haf::sys

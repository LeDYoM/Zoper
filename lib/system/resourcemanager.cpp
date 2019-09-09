#include "resourcemanager.hpp"
#include "systemprovider.hpp"
#include "filesystem/filesystem.hpp"
#include "filesystem/file.hpp"

#include <lib/include/liblog.hpp>

#include <lib/resources/ttfont.hpp>
#include <lib/resources/texture.hpp>
#include <lib/resources/shader.hpp>
#include <lib/resources/bmpfont.hpp>

#include <backend/include/backendfactory.hpp>
#include <algorithm>
#include <list>

namespace lib::core
{
    template <typename T>
    using NamedIndex = pair<const str, T>;

    template <typename T>
    using ResourceList = std::list<NamedIndex<T>>;

    namespace
    {
        template <bool UseInternalFileSystem, typename T, typename V>
        inline sptr<T> loadResource(backend::IResourceFactory<V>& factory, 
            FileSystem& fileSystem, const str &fileName)
        {
            if constexpr (UseInternalFileSystem)
            {
                RawMemory data(fileSystem.loadBinaryFile(fileName));

                // Prototype / check
                return msptr<T>(factory.loadFromRawMemory(&data));
            }
            else
            {
                return msptr<T>(factory.loadFromFile(fileName));
            }
        }

        template <bool UseInternalFileSystem, typename V, typename T>
        inline sptr<T> get_or_add(backend::IResourceFactory<V>& factory, ResourceList<sptr<T>> &container, 
            FileSystem& fileSystem, const str &rid, const str &fileName)
        {
            auto iterator(std::find_if(container.begin(), container.end(),
                [rid](const auto &node) { return node.first == rid; }));

            if (iterator != container.end())
            {
                log_debug_info(rid, " found on resource list. Returning instance.");

                return (*iterator).second;
            }
            else
            {
                // Not found, try to load it.
                log_debug_info(rid, " not found on resource list.");

                log_debug_info("Going to load file: ", fileName);
                sptr<T> resource(loadResource<UseInternalFileSystem,T>(factory, fileSystem, fileName));
                container.push_back(NamedIndex<sptr<T>>(rid, resource));
                return resource;
            }
        }

        template <typename T>
        inline auto get_or_default(ResourceList<sptr<T>> &container, const str &rid)
        {
            auto iterator(std::find_if(container.begin(), container.end(),
                [rid](const auto &node) { return node.first == rid; }));

            return (iterator != container.end()) ?
                (*iterator).second
                :
                msptr<T>(nullptr);
        }
    }

    struct ResourceManager::ResourceManagerPrivate
    {
        ResourceList<sptr<scene::TTFont>> ttf_fonts_;
        ResourceList<sptr<scene::Texture>> textures_;
        ResourceList<sptr<scene::Shader>> shaders_;
        ResourceList<sptr<scene::BMPFont>> bmp_fonts_;
    };

    ResourceManager::ResourceManager(core::SystemProvider &system_provider) 
        : HostedAppService{ system_provider },
        m_private{ muptr<ResourceManagerPrivate>() } {}

    ResourceManager::~ResourceManager() = default;

    sptr<scene::ITTFont> ResourceManager::getTTFont(const str &rid)
    {
        return get_or_default(m_private->ttf_fonts_, rid);
    }

    sptr<scene::ITexture> ResourceManager::getTexture(const str &rid)
    {
        return get_or_default(m_private->textures_, rid);
    }

    sptr<scene::IShader> ResourceManager::getShader(const str &rid)
    {
        return get_or_default(m_private->shaders_, rid);
    }

    sptr<scene::IFont> ResourceManager::getBMPFont(const str &rid)
    {
        return get_or_default(m_private->bmp_fonts_, rid);
    }

    sptr<scene::TTFont> ResourceManager::loadTTFont(const str & rid, const str & fileName)
    {
        return get_or_add<true>(systemProvider().backendFactory().ttfontFactory(), m_private->ttf_fonts_, systemProvider().fileSystem(),  rid, fileName);
    }

    sptr<scene::Texture> ResourceManager::loadTexture(const str & rid, const str & fileName)
    {
        return get_or_add<true>(systemProvider().backendFactory().textureFactory(), m_private->textures_, systemProvider().fileSystem(), rid, fileName);
    }
    sptr<scene::Shader> ResourceManager::loadShader(const str & rid, const str & fileName)
    {
        return get_or_add<true>(systemProvider().backendFactory().shaderFactory(), m_private->shaders_, systemProvider().fileSystem(), rid, fileName);
    }

    sptr<scene::BMPFont> ResourceManager::loadBMPFont(const str &rid, const str &fileName)
    {
        auto font(get_or_add<true>(systemProvider().backendFactory().bmpFontFactory(), 
            m_private->bmp_fonts_, systemProvider().fileSystem(), rid, fileName));

        if (font)
        {
            auto glyph_file_names(font->glyphFileNames());
            vector<sptr<scene::Texture>> textures(glyph_file_names.size());
            std::transform(glyph_file_names.cbegin(), glyph_file_names.cend(),
            textures.begin(),
                [this](const auto& glyph_file_name)
                {
                    return this->loadTexture(glyph_file_name);
                }
            );
        }

        return font;
    }
}

#include "mainmenuresources.hpp"

#include <lib/resources/include/iresourcehandler.hpp>

namespace zoper
{
using namespace haf;
using namespace haf::scene;

void MainMenuResources::loadResources(IResourceHandler &resourceHandler)
{
    constexpr static char BackgroundTextureFile[] = "resources/gr_ms.png";
    constexpr static char LogoFile[] = "resources/zoperbl.png";
    constexpr static char MenuFontFile[] = "resources/oldct.ttf";
    constexpr static char ShaderFile[] = "resources/wave";

    resourceHandler.loadTexture(LogoId, LogoFile);
    resourceHandler.loadTTFont(MenuFontId, MenuFontFile);
    resourceHandler.loadShader(Shader, ShaderFile);

#ifdef TEST_BMP_FONT
    constexpr static char TestBMPFontFile[] = "lucida";
    resourceHandler.loadBMPFont(TestFontId, TestBMPFontFile);
#endif
}
} // namespace zoper

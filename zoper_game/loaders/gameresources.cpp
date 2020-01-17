#include "gameresources.hpp"

#include <lib/resources/include/iresourcehandler.hpp>
#include <lib/resources/include/itexture.hpp>
#include <lib/resources/include/ittfont.hpp>

namespace zoper
{
using namespace lib;
using namespace lib::scene;

constexpr char ScoreFontFile[] = "resources/score.ttf";

void GameResources::loadResources(IResourceHandler &resourceHandler)
{
    resourceHandler.loadTTFont(ScoreFontId, ScoreFontFile);
}
} // namespace zoper

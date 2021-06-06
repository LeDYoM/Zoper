#include "console.hpp"
#include <haf/include/scene_nodes/scenenodetext.hpp>
#include "../loaders/mainmenuresources.hpp"

#include <haf/include/scene/scene.hpp>
#include <haf/include/scene_nodes/scenenodetext.hpp>

#include <hlog/include/hlog.hpp>
#include <haf/include/resources/iresourceretriever.hpp>
#include <haf/include/resources/ittfont.hpp>

using namespace htps;
using namespace haf;
using namespace haf::scene;
using namespace haf::scene::nodes;

namespace zoper
{

void Console::onCreated()
{
    SceneNode::onCreated();

    auto a = createSceneNode<SceneNodeText>("scnText");
    a->prop<Font>().set(systemInterface<res::IResourceRetriever>()
                .getBMPFont("console_font"));

    prop<Position>().set(vector2df{100.0F, 0.0F});
    a->prop<Text>().set("1234 ABC abc");
    a->prop<TextColor>().set(colors::Red);
//    a->prop<Scale>().set({1, 1});
}

}  // namespace zoper

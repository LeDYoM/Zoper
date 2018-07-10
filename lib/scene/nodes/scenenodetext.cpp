#include "scenenodetext.hpp"

#include <lib/scene/ttfont.hpp>
#include <lib/scene/texture.hpp>
#include <lib/include/core/log.hpp>
#include <lib/scene/vertexarray.hpp>
#include <lib/scene/scenenodetypes.hpp>

// Temp
#include <cmath>

namespace lib::scene::nodes
{
    SceneNodeText::SceneNodeText(SceneNode * const parent, const str & name)
        : SceneNode{ parent, name } //, Triangles, 0 }
    {
    }

    nodes::SceneNodeText::~SceneNodeText() = default;

    void SceneNodeText::update()
    {
        BaseClass::update();

        if (font.hasChanged() || characterSize.hasChanged()
                || text.hasChanged())
        {
            clearSceneNodes();

            if (font() && characterSize() && !(text()().empty()))
            {
                auto texture(font()->getTexture(characterSize()));
                if (ps_readResetHasChanged(font, characterSize))
                {
                        font()->ensureLoadASCIIGlyps(characterSize());
                        texture = font()->getTexture(characterSize());
                        text.setChanged();
                }

                if (ps_readResetHasChanged(text))
                {
                    const u32 currentCharacterSize{ characterSize() };
                    const f32 vspace{ font()->getLineSpacing(currentCharacterSize) };

                    f32 x{ 0.f };
                    f32 y{ static_cast<f32>(currentCharacterSize) };

                    // Create one quad for each character
                    f32 minX{ y };
                    f32 minY{ y };
                    f32 maxX{ 0.f };
                    f32 maxY{ 0.f };
                    u32 prevChar{ 0 };
                    for (auto&& curChar : text()())
                    {
                        // Apply the kerning offset
                        x += font()->getKerning(prevChar, curChar, currentCharacterSize);
                        prevChar = curChar;

                        // Handle special characters
                        if ((curChar == ' ') || (curChar == '\t') || (curChar == '\n')) {
                            using namespace std;
                            // Update the current bounds (min coordinates)
                            minX = min(minX, x);
                            minY = min(minY, y);
                            const f32 hspace{ font()->getGlyph(L' ', currentCharacterSize).advance };

                            switch (curChar)
                            {
                            case ' ':  x += hspace;        break;
                            case '\t': x += hspace * 4;    break;
                            case '\n': y += vspace; x = 0; break;
                            }

                            // Update the current bounds (max coordinates)
                            maxX = max(maxX, x);
                            maxY = max(maxY, y);
                        }
                        else
                        {
                            const TTGlyph glyph{ font()->getGlyph(curChar, currentCharacterSize) };
                            const Rectf32 textureUV{ glyph.textureBounds};
                            const Rectf32 letterBox{ glyph.bounds + vector2df{ x,y } };

                            auto letterNode(createSceneNode
                                            <QuadSceneNode>("text_"+str(curChar)));
                            letterNode->node()->box.set(letterBox);
                            letterNode->node()->setTextureAndTextureRect(texture,
                                        textureUV);

                            // Update the current bounds
                            {
                                using namespace std;
                                minX = min(minX, letterBox.left);
                                maxX = max(maxX, letterBox.right());
                                minY = min(minY, letterBox.top);
                                maxY = max(maxY, letterBox.bottom());
                            }

                            // Advance to the next character
                            x += glyph.advance;
                        }
                    }

                    // Force update color
                    textColor.setChanged();
                }
            }
        }

        if (textColor.readResetHasChanged()) {
            const Color &tc{textColor()()};
            sceneNodes().for_each([&tc](const SceneNodeSPtr& sNode) {
                sNode->snCast<QuadSceneNode>()->node()->color.set(tc);
            });
        }
    }
}
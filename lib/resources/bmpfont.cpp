#include "bmpfont.hpp"
#include "bmpfont_private.hpp"
#include <lib/include/liblog.hpp>

#include <fstream>

namespace lib::scene
{
    using namespace backend;

    BMPFont::BMPFont(const str& id, const str& file_name, IResourceHandler& resource_handler)
    {
        fontPrivate = new BMFontPrivate;
        fontPrivate->chars_.resize(256U);

        str fontfile(file_name + ".fnt");
        str texturefile(file_name + ".png");
        log_debug_info("Starting to Parse Font ", fontfile);
        ParseFont(fontfile);
        log_debug_info("Finished Parsing Font ", fontfile);
        log_debug_info("Calculating some metrics");
        fontPrivate->adv = 1.0f / (f32)size().x;

        for (u32 i{0U}; i < fontPrivate->chars_.size(); ++i)
        {
            fontPrivate->chars_[i].offsetedPosition = Rectf32(
                static_cast<vector2d<f32>>(fontPrivate->chars_[i].offset), 
                static_cast<vector2d<f32>>(fontPrivate->chars_[i].position.size())
            );
        }
        log_debug_info("Finished Parsing Font ", fontfile);
        log_debug_info("Loading pages. Number of pages: ", fontPrivate->pagesData_.size());

        for (u32 i = 0; i < fontPrivate->pagesData_.size(); ++i)
        {
            fontPrivate->pagesData_[i].it = resource_handler.loadTexture(
                fontPrivate->pagesData_[i].file, 
                fontPrivate->pagesData_[i].file);
        }
        log_debug_info("Page(s) loaded");
    }

    BMPFont::~BMPFont()
    {
        delete fontPrivate;
    }

    const vector<str>& BMPFont::glyphFileNames() const
    {
        vector<str> result(fontPrivate->pagesData_.size());

        for (const auto& file : fontPrivate->pagesData_)
        {
            result.push_back(file.file);
        }

        return result;
    }

    void BMPFont::setGlyphTextures(const vector<sptr<Texture>>& textures)
    {
        assert_debug(textures.size() == fontPrivate->pagesData_.size(), 
        "The size of the vector passed to loadGlyphTextures is different from the size of pages");

        for (u32 i(0U); i < fontPrivate->pagesData_.size(); ++i)
        {
            fontPrivate->pagesData_[i].it = textures[i];
        }
    }

    void filterStr(std::stringstream &LineStream, std::string &Value)
    {
        bool doNext = true;
        do
        {
            int first_ = Value.find_first_of('\"');
            int last_ = Value.find_last_of('\"');
            doNext = (first_ > -1 && first_ == last_);
            if (doNext)
            {
                std::string readTemp;
                LineStream >> readTemp;
                Value += " " + readTemp;
            }
        } while (doNext);
    }

    const vector2du32 &BMPFont::size() const
    {
        return fontPrivate->size_;
    }

    std::string getStr(const std::string &read)
    {
        std::string result;

        for (char ch : read)
        {
            if (ch != '"')
            {
                result.push_back(ch);
            }
        }
        return result;
    }

    bool BMPFont::ParseFont(const str&fontfile)
    {
        std::ifstream Stream(fontfile.c_str());
        std::string Line, Read, Key, Value;
        std::size_t i{0U};
        u32 first{0U};
        u32 second{0U};
        s32 amount{0};

        KearningInfo K;
        CharDescriptor C;

        while (Stream && !Stream.eof())
        {
            std::stringstream LineStream;
            std::getline(Stream, Line);
            LineStream << Line;

            //read the line's type
            LineStream >> Read;
            if (Read == "common")
            {
                //this holds common data
                while (!LineStream.eof())
                {
                    std::stringstream Converter;
                    LineStream >> Read;
                    i = Read.find('=');
                    Key = Read.substr(0, i);
                    Value = Read.substr(i + 1).c_str();

                    //assign the correct value
                    Converter << Value;
                    if (Key == "lineHeight")
                    {
                        Converter >> fontPrivate->lineHeight;
                    }
                    else if (Key == "base")
                    {
                        Converter >> fontPrivate->base;
                    }
                    else if (Key == "scaleW")
                    {
                        Converter >> fontPrivate->size_.x;
                    }
                    else if (Key == "scaleH")
                    {
                        Converter >> fontPrivate->size_.y;
                    }
                    else if (Key == "pages")
                    {
                        Converter >> fontPrivate->pages;
                    }
                    else if (Key == "outline")
                    {
                        Converter >> fontPrivate->outline;
                    }
                }
            }
            else if (Read == "page")
            {
                //this holds common data
                while (!LineStream.eof())
                {
                    std::stringstream Converter;
                    LineStream >> Read;
                    i = Read.find('=');
                    Key = Read.substr(0, i);
                    Value = Read.substr(i + 1);
                    u32 id(0);

                    filterStr(LineStream, Value);

                    //assign the correct value
                    Converter << Value;
                    if (Key == "id")
                    {
                        Converter >> id;
                        fontPrivate->pagesData_.resize(id + 1);
                    }
                    else if (Key == "file")
                    {
                        fontPrivate->pagesData_[id].file = str(getStr(Value).c_str());
                    }
                }
            }
            else if (Read == "info")
            {
                //this holds info font data
                while (!LineStream.eof())
                {
                    std::stringstream Converter;
                    LineStream >> Read;
                    i = Read.find('=');
                    Key = Read.substr(0, i);
                    Value = Read.substr(i + 1);

                    filterStr(LineStream, Value);
                    Converter << Value;
                    if (Key == "face")
                    {
                        fontPrivate->fInfo.face = getStr(Value).c_str();
                    }
                    else if (Key == "size")
                    {
                        Converter >> fontPrivate->fInfo.size;
                    }
                    else if (Key == "bold")
                    {
                        Converter >> fontPrivate->fInfo.bold;
                    }
                    else if (Key == "italic")
                    {
                        Converter >> fontPrivate->fInfo.italic;
                    }
                    else if (Key == "charset")
                    {
                        fontPrivate->fInfo.charset = getStr(Value).c_str();
                    }
                    else if (Key == "unicode")
                    {
                        Converter >> fontPrivate->fInfo.unicode;
                    }
                    else if (Key == "stretchH")
                    {
                        Converter >> fontPrivate->fInfo.stretchH;
                    }
                    else if (Key == "smooth")
                    {
                        Converter >> fontPrivate->fInfo.smooth;
                    }
                    else if (Key == "aa")
                    {
                        Converter >> fontPrivate->fInfo.aa;
                    }
                    else if (Key == "padding")
                    {
                        std::string temp;
                        std::getline(Converter, temp, ',');
                        fontPrivate->fInfo.padding.left = static_cast<s16>(std::atoi(temp.c_str()));
                        std::getline(Converter, temp, ',');
                        fontPrivate->fInfo.padding.top = static_cast<s16>(std::atoi(temp.c_str()));
                        std::getline(Converter, temp, ',');
                        fontPrivate->fInfo.padding.setRight(static_cast<s16>(std::atoi(temp.c_str())));
                        std::getline(Converter, temp, ',');
                        fontPrivate->fInfo.padding.setBottom(static_cast<s16>(std::atoi(temp.c_str())));
                    }
                    else if (Key == "spacing")
                    {
                        std::string temp;
                        std::getline(Converter, temp, ',');
                        fontPrivate->fInfo.spacing.x = static_cast<s16>(std::atoi(temp.c_str()));
                        std::getline(Converter, temp, ',');
                        fontPrivate->fInfo.spacing.y = static_cast<s16>(std::atoi(temp.c_str()));
                    }
                    else if (Key == "outline")
                    {
                        Converter >> fontPrivate->fInfo.outline;
                    }
                }
            }
            else if (Read == "char")
            {
                //This is data for each specific character.
                int CharID = 0;

                while (!LineStream.eof())
                {
                    std::stringstream Converter;
                    LineStream >> Read;
                    i = Read.find('=');
                    Key = Read.substr(0, i);
                    Value = Read.substr(i + 1);

                    //Assign the correct value
                    Converter << Value;
                    if (Key == "id")
                    {
                        Converter >> CharID;
                    }
                    else if (Key == "x")
                    {
                        Converter >> C.position.left;
                    }
                    else if (Key == "y")
                    {
                        Converter >> C.position.top;
                    }
                    else if (Key == "width")
                    {
                        u16 temp;
                        Converter >> temp;
                        C.position.width = temp;
                    }
                    else if (Key == "height")
                    {
                        u16 temp;
                        Converter >> temp;
                        C.position.height = temp;
                    }
                    else if (Key == "xoffset")
                    {
                        Converter >> C.offset.x;
                    }
                    else if (Key == "yoffset")
                    {
                        Converter >> C.offset.y;
                    }
                    else if (Key == "xadvance")
                    {
                        Converter >> C.xadvance;
                    }
                    else if (Key == "page")
                    {
                        Converter >> C.page;
                    }
                }
                if (CharID < 256)
                    fontPrivate->chars_[CharID] = C;

            }
            else if (Read == "kernings")
            {
                while (!LineStream.eof())
                {
                    std::stringstream Converter;
                    LineStream >> Read;
                    i = Read.find('=');
                    Key = Read.substr(0, i);
                    Value = Read.substr(i + 1);

                    //assign the correct value
                    Converter << Value;
                    //      if( Key == "count" )
                    //          {Converter >> KernCount; }
                }
            }

            else if (Read == "kerning")
            {
                while (!LineStream.eof())
                {
                    std::stringstream Converter;
                    LineStream >> Read;
                    i = Read.find('=');
                    Key = Read.substr(0, i);
                    Value = Read.substr(i + 1);

                    Converter << Value;
                    if (Key == "first")
                    {
                        Converter >> first;
                    }

                    else if (Key == "second")
                    {
                        Converter >> second;
                    }

                    else if (Key == "amount")
                    {
                        Converter >> amount;
                    }
                }
                CharDescriptor &ch = fontPrivate->chars_[first];
                ch.kearn.push_back(KearningInfo(second, amount));
            }
        }

        Stream.close();
        return true;
    }

    Rectf32 BMPFont::getBounds(const u32 codePoint) const
    {
        return fontPrivate->chars_[codePoint].offsetedPosition;
    }

    Rectf32 BMPFont::getTextureBounds(const u32 codePoint) const
    {
        return static_cast<Rectf32>(fontPrivate->chars_[codePoint].position);
    }

    f32 BMPFont::getAdvance(const u32 codePoint) const
    {
        return static_cast<f32>(fontPrivate->chars_[codePoint].xadvance);
    }

    f32 BMPFont::getLineSpacing() const
    {
        return 0.F;
    }

    f32 BMPFont::getKerning(const u32 first, const u32 second) const
    {
        return static_cast<f32>(fontPrivate->chars_[first].GetKerningPair(second));
    }

    sptr<ITexture> BMPFont::getTexture() const
    {
        return fontPrivate->pagesData_[0U].it;
    }

    vector2df BMPFont::textSize(const str&text) const
    {
        return vector2df{};
    }
}

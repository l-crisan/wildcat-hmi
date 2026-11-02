/*
   Copyright (C) 2017 Marc Boris Duerner
   Copyright (C) 2010-2026 by Laurentiu-Gheorghe Crisan
   Copyright (C) 2010-2026 by ATESiON GmbH

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  As a special exception, you may use this file as part of a free
  software library without restriction. Specifically, if other files
  instantiate templates or use macros or inline functions from this
  file, or you compile this file and link it with other files to
  produce an executable, this file does not by itself cause the
  resulting executable to be covered by the GNU General Public
  License. This exception does not however invalidate any other
  reasons why the executable file might be covered by the GNU Library
  General Public License.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the:
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301 USA
*/
#ifndef WILDCAT_GUI_GFX_FREETYPE_H
#define WILDCAT_GUI_GFX_FREETYPE_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_CACHE_H

#include <Pt/Types.h>
#include <wildcat/gui/gfx/Font.h>
#include <wildcat/gui/gfx/FontInfo.h>
#include <wildcat/gui/gfx/Rect.h>
#include <Pt/System/Path.h>
#include <Pt/Singleton.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

namespace wildcat {
namespace gui {
namespace gfx {

class ArgbColor;
class Pixmap;
class FontMetrics;
class CompositionMode;
class Transform;

class FreeType : public Pt::Singleton<FreeType>
{
    friend class Pt::Singleton<FreeType>;

public:
    struct Init
    {
        Init()
        {
            FreeType::instance();
        }
    };

    ~FreeType();

    const gfx::Font& defaultFont() const;

    void setDefaultFont(const gfx::Font& font);

    const std::vector<FontInfo>& fonts() const;

    void setFontDir(const Pt::System::Path& path);

    FontMetrics fontMetrics(const Pt::String& text,
        FTC_FaceID faceId,
        std::size_t fontSize);

    static FT_Error fontRequest(FTC_FaceID face_id, FT_Library library,
        FT_Pointer request_data, FT_Face* face);

    FTC_FaceID findFaceId(const Font& font);

    void draw(Pixmap& image, const ArgbColor& color,
        const Point& pos, const Pt::String& text,
        const Rect& clip, const CompositionMode& mode,
        const Transform& transform, FTC_FaceID faceId,
        std::size_t fontSize);

protected:
    FreeType();

    FT_Error onFontRequest(FTC_FaceID face_id, FT_Face* face);

    void drawGlyph(Pixmap& image, const ArgbColor& color, int xpos, int ypos,
        int bmPitch, int height, int width,
        const unsigned char* buffer, const Rect& clip,
        const CompositionMode& mode);

    void draw_Pixmap(FT_Bitmap* Pixmap, FT_Int x, FT_Int y);

private:

    typedef std::vector<FontInfo> Fonts;

private:
    FT_Library     _ft;
    FTC_Manager    _manager;
    FTC_ImageCache _imageCache;
    FTC_CMapCache  _charMapCache;
    FTC_SBitCache  _PixmapCache;


    FTC_FaceID     _fallbackRegularFaceId;
    FTC_FaceID     _fallbackItalicFaceId;
    FTC_FaceID     _fallbackBoldFaceId;

    Pt::System::Path   _fontDir;
    Fonts          _fonts;

    gfx::Font    _defaultFont;
    FTC_FaceID   _defaultFontId;
};

static FreeType::Init initFreeType;

}}}

#endif

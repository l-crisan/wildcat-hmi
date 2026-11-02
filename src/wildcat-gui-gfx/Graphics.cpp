/*
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
#include <wildcat/gui/gfx/Graphics.h>
#include <wildcat/gui/gfx/Algorithm.h>
#include <wildcat/gui/gfx/Shape.h>
#include <wildcat/gui/base/String.h>
#include "Rasterizer.h"
#include <algorithm>

namespace wildcat {
namespace gui {
namespace gfx {


Graphics::Graphics()
: _scaleFactor(1)
{
}


Graphics::~Graphics()
{
    
}

void Graphics::drawFrame(const Rect& rect, int corner)
{
    if (corner != 0)
    {
        gfx::Polygon polygon = gfx::Graphics::toPolygon(rect, corner);
        drawPolyline(&polygon[0], polygon.size());
    }
    else
    {
        drawRect(rect);
    }
}

void Graphics::fillFrame(const Rect& rect, int corner)
{
    if (corner != 0)
    {
        gfx::Polygon polygon = gfx::Graphics::toPolygon(rect, corner);
       fillPolygon(&polygon[0], polygon.size());
    }
    else
    {
        fillRect(rect);
    }
}

void Graphics::drawShape(const Shape& shape)
{
    shape.draw(*this);
}

const Size& Graphics::size() const
{
    return onSize();
}

FontMetrics Graphics::fontMetrics(const Font& font, const Pt::String& text)
{
    return Rasterizer::fontMetrics(font, text);
}


void Graphics::setFontDir(const Pt::System::Path& path)
{
    Rasterizer::setFontDir(path);
}


const gfx::Font& Graphics::defaultFont()
{
    return Rasterizer::defaultFont();
}


void Graphics::setDefaultFont(const gfx::Font& f)
{
    Rasterizer::setDefaultFont(f);
}


const std::vector<FontInfo>& Graphics::fonts()
{
    return Rasterizer::fonts();
}

Size Graphics::fontMetricsBox(const Pt::String& text, const Font& font)
{
    Size box;

    std::vector<Pt::String> lines =  base::String::split(text, Pt::String("\n"));
    int width = 0;

    for (size_t i = 0; i < lines.size(); i++)
    {
        FontMetrics m = fontMetrics(font, lines[i]);    
        width = std::max(width, m.width());
        box.setHeight(box.height() + m.height());
    }

    box.setWidth(width);    
    return box;
}

gfx::Polygon Graphics::toPolygon(const gfx::Rect& rect, int corner)
{
    gfx::Polygon polygon;
    gfx::Point outline[9] = {};

    //1
    outline[0].setX(rect.x());
    outline[0].setY(rect.y() + corner);

    //2
    outline[1].setX(rect.x() + corner);
    outline[1].setY(rect.y());

    //3
    outline[2].setX(rect.right() - corner);
    outline[2].setY(rect.y());

    //4
    outline[3].setX(rect.right());
    outline[3].setY(rect.y() + corner);

    //5
    outline[4].setX(rect.right());
    outline[4].setY(rect.bottom() - corner);

    //6
    outline[5].setX(rect.right() - corner);
    outline[5].setY(rect.bottom());

    //7
    outline[6].setX(rect.x() + corner);
    outline[6].setY(rect.bottom());

    //8
    outline[7].setX(rect.x());
    outline[7].setY(rect.bottom() - corner);

    outline[8] = outline[0];

    for (size_t i = 0; i < 9; ++i)
        polygon.push_back(outline[i]);

    return polygon;
}

}}}

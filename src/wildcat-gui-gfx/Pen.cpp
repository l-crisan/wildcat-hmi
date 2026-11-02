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
#include <wildcat/gui/gfx/Pen.h>

namespace wildcat {
namespace gui {
namespace gfx {

Pen::Pen()
: _color()
, _size(1)
, _style(Solid)
, _dashPattern()
, _capStyle(RoundCap)
, _joinStyle(RoundJoin)
{
}


Pen::Pen(const ArgbColor& color)
: _color(color)
, _size(1)
, _style(Solid)
, _dashPattern()
, _capStyle(RoundCap)
, _joinStyle(RoundJoin)
{

}

Pen::Pen(const ArgbColor& color, std::size_t size,
         Style style, CapStyle cap, JoinStyle join)
: _color(color)
, _size(size)
, _style(style)
, _dashPattern()
, _capStyle(cap)
, _joinStyle(join)
{
}


Pen::Pen(const ArgbColor& color, std::size_t size,
         const std::vector<Pt::uint8_t>& dashPattern, 
         CapStyle cap, JoinStyle join)
: _color(color)
, _size(size)
, _style(Style::DashPattern)
, _dashPattern(dashPattern)
, _capStyle(cap)
, _joinStyle(join)

{
}

WILDCAT_GUI_GFX_API  std::ostream& operator<<(std::ostream& os, const Pen& obj)
{
    os << '(' << obj.size() << ',' << obj.color() << ')';
    return os;
}


WILDCAT_GUI_GFX_API std::istream& operator>>(std::istream& is, Pen& obj)
{
    char ch;
    size_t size;
    ArgbColor color;
    is >> ch;
    is >> size;
    is >> ch;
    is >> color;
    is >> ch;

    obj.setSize(size);
    obj.setColor(color);
    return is;
}

}}}

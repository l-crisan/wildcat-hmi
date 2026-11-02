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
#include <wildcat/gui/gfx/Arrow.h>

namespace wildcat {
namespace gui {
namespace gfx {

Arrow::Arrow()
{
    
}

Arrow::Arrow(Direction d)
{
    reset(d);
}

void Arrow::reset(Direction d)
{
    switch (d)
    {
        case Arrow::Left:
            _triangle[0] = Point(4, 0);
            _triangle[1] = Point(4, 8);
            _triangle[2] = Point(0, 4);
            _triangle[3] = Point(4, 0);
            _size.set(4, 8);
        break;

        case Arrow::Right:
            _triangle[0] = Point(0, 0);
            _triangle[1] = Point(4, 4);
            _triangle[2] = Point(0, 8);
            _triangle[3] = Point(0, 0);
            _size.set(4, 8);
        break;

        case Arrow::Top:
            _triangle[0] = Point(4, 0);
            _triangle[1] = Point(8, 4);
            _triangle[2] = Point(0, 4);
            _triangle[3] = Point(4, 0);
            _size.set(8, 4);
        break;

        case Arrow::Buttom:   
            _triangle[0] = Point(0, 0);
            _triangle[1] = Point(8, 0);
            _triangle[2] = Point(4, 4);
            _triangle[3] = Point(0, 0);
            _size.set(8, 4);
        break;        
    }    

}

Arrow::~Arrow()
{
}

void Arrow::draw(Graphics& gf) const
{    
    gf.drawLine(_triangle[0], _triangle[1]);
    gf.drawLine(_triangle[1], _triangle[2]);
    gf.drawLine(_triangle[2], _triangle[0]);

    gf.fillPolygon(_triangle, 4);
}


void Arrow::translate(const Point& v)
{
    for (size_t i = 0; i < 4; ++i)
        _triangle[i] += v;
}

void Arrow::scale(double factor)
{
    for(size_t i = 0 ; i < 4; ++i)
        _triangle[i] *= factor;
}

}}}

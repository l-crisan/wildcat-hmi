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
#include <wildcat/gui/gfx/Cross.h>

namespace wildcat {
namespace gui {
namespace gfx {


Cross::Cross()
{
    _size.set(11, 11);
    init();
}

Cross::~Cross()
{
}

void Cross::init()
{
    _cross.setOrigin(gfx::Point(1,1));
    _cross.setSize(_size);
}

void Cross::onReset()
{
    init(); 
}

void Cross::draw(Graphics& gf) const
{
    int margin = 0;
    int offset = 1;
    int inset = 1;

    const gfx::Rect& buttonRect = _cross;

    gfx::Point line1[] = { gfx::Point(buttonRect.topLeft().x() + margin + inset,
                                         buttonRect.topLeft().y() + offset + margin + inset),

                            gfx::Point(buttonRect.topLeft().x() + offset + margin + inset,
                                         buttonRect.topLeft().y() + margin + inset),

                            gfx::Point(buttonRect.bottomRight().x() - margin - inset,
                                         buttonRect.bottomRight().y() - offset - margin - inset),

                            gfx::Point(buttonRect.bottomRight().x() - offset - margin - inset,
                                         buttonRect.bottomRight().y() - margin - inset) };

    gfx::Point line2[] = { gfx::Point(buttonRect.topRight().x() - offset - margin - inset,
                                         buttonRect.topRight().y() + margin + inset),

                            gfx::Point(buttonRect.topRight().x() - margin - inset,
                                         buttonRect.topRight().y() + offset + margin + inset),

                            gfx::Point(buttonRect.bottomLeft().x() + offset + margin + inset,
                                         buttonRect.bottomLeft().y() - margin - inset),

                            gfx::Point(buttonRect.bottomLeft().x() + margin + inset,
                                         buttonRect.bottomLeft().y() - offset - margin - inset) };

    gf.fillPolygon(line1, 4);
    gf.fillPolygon(line2, 4);
}


void Cross::translate(const gfx::Point& v)
{
    _cross.setOrigin( _cross.topLeft() + v);
}

void Cross::scale(double factor)
{ 
    _cross.setSize(_cross.size() * factor);
}

}}}

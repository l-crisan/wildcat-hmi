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
#include <wildcat/gui/gfx/Pin.h>

namespace wildcat {
namespace gui {
namespace gfx {


Pin::Pin()
: _state(Off)
{    
    init();    
    _size.set(9, 9);
}

Pin::~Pin()
{
}

void Pin::onReset()
{
    init();
}

void Pin::init()
{   
    
    if (_state == On)
    {
        _head.setOrigin(gfx::Point(2, 0));
        _head.setSize(gfx::Size(5, 5));

        _shadow[0].set(5, 0);
        _shadow[1].set(5, 5);

        _base[0].set(0, 4);
        _base[1].set(size().width(), 4);

        _pin[0].set(4, 4);
        _pin[1].set(4, size().height());
    }
    else
    {
        const int base = size().width() - 4;  

        _head.setOrigin(gfx::Point(base, 2));
        _head.setSize(gfx::Size(5, 5));
        
        _shadow[0].set(base, 3);
        _shadow[1].set(size().width(), 3);

        _base[0].set(base, 0);
        _base[1].set(base, size().height()-1);
        
        _pin[0].set(0, 4);
        _pin[1].set(base, 4);        
    }    
}

void Pin::draw(Graphics& gf) const
{        
    gf.drawRect(_head);

    gf.drawLine(_shadow[0], _shadow[1]);

    gf.drawLine(_base[0], _base[1]);    
    gf.drawLine(_pin[0], _pin[1]);      
}

void Pin::translate(const gfx::Point& v)
{
    _head.setOrigin(_head.topLeft() + v);

    _shadow[0] += v;
    _shadow[1] += v;

    _base[0] += v;
    _base[1] += v;

    _pin[0] += v;
    _pin[1] += v;
}

void Pin::scale(double factor)
{
    _head.setSize(_head.size() * factor);
    _shadow[0] *= factor;
    _shadow[1] *= factor;

    _base[0] *= factor;
    _base[1] *= factor;
            
    _pin[0] *= factor;
    _pin[1] *= factor;

}

}}}


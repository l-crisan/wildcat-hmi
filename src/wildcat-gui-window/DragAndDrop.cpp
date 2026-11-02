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
#include <wildcat/gui/window/DragAndDrop.h>

namespace wildcat {
namespace gui {
namespace window {

DragAndDrop::DragAndDrop()
: _drag(false)
, _press(false)
, _released(false)
, _begin(0,0)
, _current(0,0)
, _delta(2)
{
}

DragAndDrop::~DragAndDrop()
{

}

void DragAndDrop::process(const gfx::Point& position, bool press, bool release)
{    
    if ((_press || _drag) && release)
    {
        if (_drag)
            _released = true;

        _press = false;
        _drag  = false;
        return;
    }

    if (!_press && !_drag && press)
    {
        _begin = position;
        _press = true;            
        return;
    }

    if (_press && !_drag)
    {
        const gfx::Point dt = _begin.deltaAbs(position);

        if( std::abs(dt.x()) > _delta || std::abs(dt.y()) > _delta)
        {//Drag detection after _delta pixel 
            _press = false;
            _drag = true;
        }
    }

    if(_drag)
    {
        _current = position;
    }
}

void DragAndDrop::reset()
{
    _press = false;
    _drag = false;
    _released = false;
    _begin.set(0, 0);
    _current.set(0, 0); 
}

}}}

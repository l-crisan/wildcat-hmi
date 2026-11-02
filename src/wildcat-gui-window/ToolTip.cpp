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
#include  <wildcat/gui/window/ToolTip.h>

namespace wildcat {
namespace gui {
namespace window {

ToolTip::ToolTip()
: _autoPopDelay(500)
, _initialDelay(500)
, _reshowDelay(500)
, _state(Idle)
, _timer()
, _ctrl(nullptr)
{
    _timer.timeout() += Pt::slot(*this, &ToolTip::onTimeout);   
}


ToolTip::~ToolTip()
{
    
}

void ToolTip::start(Control& ctrl)
{
    _timer.stop();

    if (_ctrl == &ctrl)
    {
        _ctrl = &ctrl;  
        _state = PopDelay;
        _timer.start(_reshowDelay); 
        return;

    }
    
    _ctrl = &ctrl;
    _state = PopDelay;
    _timer.start(_initialDelay);    
}



void ToolTip::stop(Control& ctrl)
{
    if(_ctrl != &ctrl)
        return;

    _timer.stop();
    _state = Idle;
    closeToolTip();
    _ctrl == nullptr;   
}

void ToolTip::showToolTip(const gfx::Point& pos)
{
    if (_ctrl)
        _ctrl->showToolTip(pos);
}

void ToolTip::closeToolTip()
{
    if(_ctrl)
        _ctrl->closeToolTip();
}

void ToolTip::onTimeout()
{
    _timer.stop();

    switch (_state)
    {   
        case PopDelay:
        {
            _state = Showing;
            _timer.start(_autoPopDelay);    
        }
        break;

        case Showing:
        {
            showToolTip(_mousePos);
            _state = Idle;
        }
        break;
    }
}

}}}
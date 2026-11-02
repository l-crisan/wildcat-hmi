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
#ifndef WILDCAT_GUI_WINDOW_WINDOWHOST_H
#define WILDCAT_GUI_WINDOW_WINDOWHOST_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/Element.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Rect.h>
#include <Pt/Signal.h>
#include <Pt/Connectable.h>

namespace wildcat {
namespace gui {
namespace window {

class WindowType
{
    public:
        enum Type
        {
            Default = 0,      // default window style
            Borderless = 1,   // no decoration
            Fixed = 2
        };

        WindowType(Type t = Default)
        : _type(t)
        {}

        WindowType& operator=(Type t)
        {
            _type = t;
            return *this;
        }

        operator Pt::uint32_t() const
        { 
            return _type; 
        }

    private:
        Pt::uint32_t _type;
};

class WindowState
{
    public:
        enum State
        {
            Normal = 0,
            Minimized = 1,
            Maximized = 2
        };

        WindowState(State t = Normal)
        : _state(t)
        {}

        WindowState& operator=(State t)
        {
            _state = t;
            return *this;
        }

        operator Pt::uint32_t() const
        { 
            return _state; 
        }

    private:
        Pt::uint32_t _state;
};

class WindowPositon
{
    public:
        enum Position
        {
            None = 0,
            CenterParent,
            CenterScreen
        };

        WindowPositon(Position t = None)
            : _state(t)
        {}

        WindowPositon& operator=(Position t)
        {
            _state = t;
            return *this;
        }

        operator Position() const
        {
            return _state;
        }

    private:
        Position _state;
};

}}}

#endif

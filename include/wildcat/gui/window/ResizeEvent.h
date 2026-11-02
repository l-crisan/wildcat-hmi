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
#ifndef WILDCAT_GUI_WINDOW_RESIZEEVENT_H
#define WILDCAT_GUI_WINDOW_RESIZEEVENT_H

#include <wildcat/gui/window/Event.h>
#include <wildcat/gui/gfx/Size.h>

namespace wildcat {
namespace gui {
namespace window {

class ResizeEvent : public Event<ResizeEvent>
{
    public:
        ResizeEvent(Element& v, const gfx::Size& size)
        : Event( v)
        , _size(size)
        {
        }

        virtual ~ResizeEvent()
        {
        }
        
        const gfx::Size& size() const
        {
            return _size;
        }

        void setSize(const gfx::Size& size)
        {
            _size = size;
        }

    private:
        gfx::Size   _size;
};

}}}

#endif

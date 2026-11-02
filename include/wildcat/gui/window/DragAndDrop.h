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
#ifndef WILDCAT_GUI_WINDOW_DRAGANDDROP_H
#define WILDCAT_GUI_WINDOW_DRAGANDDROP_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/gfx/Point.h>

namespace wildcat {
namespace gui {
namespace window {

class WILDCAT_GUI_WINDOW_API DragAndDrop
{
    public:
        DragAndDrop();

        virtual ~DragAndDrop();

        int deltaPixel() const
        {
            return _delta;
        }

        void setDeltaPixel(int v)
        {
            _delta = v;

            if( _delta <  1)
                _delta = 1;
        }


        void process(const gfx::Point& position, bool pressed, bool release);

        bool isDrag() const
        {
            return _drag;
        }


        bool isPress() const
        {
            return _press;
        }

        

        bool isReleased() const
        {
            return _released;
        }

        const gfx::Point& begin() const
        {
            return _begin;
        }

        const gfx::Point& current() const
        {
            return _current;
        }


        gfx::Point delta() const
        {
            return _current - _begin;
        }


        const void* tag() const
        {
            return _tag;
        }

        void* tag()
        {
            return _tag;
        }

        void setTag(void* tag)
        {
            _tag = tag;
        }

        void reset();

    private:
        bool _drag;
        bool _press;
        bool _released;
        gfx::Point _begin;
        gfx::Point _current;
        void* _tag;
        int _delta;
        
};

}}}

#endif

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
#ifndef WILDCAT_GUI_WINDOW_TOUSCHEVENT_H
#define WILDCAT_GUI_WINDOW_TOUSCHEVENT_H

#include <wildcat/gui/window/Event.h>
#include <wildcat/gui/gfx/Point.h>
#include <Pt/Event.h>

namespace wildcat {
namespace gui {
namespace window {

class TouchEvent : public Event<TouchEvent>
{
    private:    
        enum Action
        {
            Move = 0,
            Press = 1,
            Release = 2
        };

    public:
        explicit TouchEvent()
        : Event(0)
        , _pos(0, 0)
        , _action(Move)
        , _trackingId(0)
        , _pressure(1.0)
        { }

        TouchEvent(Element& v)
        : Event( v)
        , _pos(0, 0)
        , _action(Move)
        , _trackingId(0)
        , _pressure(1.0)
        { }
        
        void clear()
        {
            _pos.set(0, 0);
            _action = Move;
            _trackingId = 0;
            _pressure = 1.0;
        }              

        const gfx::Point& position() const
        {
            return _pos;
        }

        void setPosition(const gfx::Point& pos)
        {
            _pos = pos;
        }

        int x() const
        {
            return _pos.x();
        }
        
        void setX(int x)
        {
            _pos.setX(x);
        }
        
        int y() const
        {
            return _pos.y();
        }

        void setY(int y)
        {
            _pos.setY(y);
        }
        
        Pt::uint32_t trackingId() const
        {
            return _trackingId;
        }
        
        void setTrackingId(Pt::uint32_t tid)
        {
            _trackingId = tid;
        }
        
        double pressure() const
        {
            return _pressure;
        }

        void setPressure(double p)
        {
            _pressure = p;
        }

        bool isMove() const
        {
            return _action == Move;
        }

        void setMove()
        {
            _action = Move;
        }

        /** @brief Returns true if touch device is in pressed state.
        */
        bool isPressed() const
        {
             return _action == Move || _action == Press;
        }

        /** @brief Returns true if touch device was just pressed.
        */
        bool isPress() const
        {
            return _action == Press;
        }

        void setPress()
        {
            _action = Press;
        }

        /** @brief Returns true if touch device was released.
        */
        bool isRelease() const
        {
            return _action == Release;
        }

        void setRelease()
        {
            _action = Release;
        }

    private:
        gfx::Point   _pos;
        Action        _action;
        Pt::uint32_t  _trackingId;
        double        _pressure;
};

}}}

#endif

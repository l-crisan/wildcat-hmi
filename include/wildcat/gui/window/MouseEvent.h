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
#ifndef WILDCAT_GUI_WINDOW_MOUSEEVENT_H
#define WILDCAT_GUI_WINDOW_MOUSEEVENT_H

#include <wildcat/gui/window/Event.h>
#include <wildcat/gui/gfx/Point.h>

namespace wildcat {
namespace gui {
namespace window {

class MouseButton
{
    public:
        enum Type
        {
            Left = 0,
            Right = 1,
            Middle = 2,
        };

        MouseButton(Type type = Left)
        : _type(type)
        { }

        operator Pt::uint32_t() const
        { 
            return _type; 
        }

    private:
        Pt::uint32_t _type;
};


class MouseState
{
    public:
        MouseState()
        : _buttonState(0)
        { }

        /** @brief Returns true if the button is in pressed state.
        */
        bool isPressed(MouseButton button) const
        {
             Pt::uint32_t mask = 0x1 << button;
             return (_buttonState & mask) == mask;
        }

        void setPressed(MouseButton button)
        {
            Pt::uint32_t mask = 0x1 << button;
            _buttonState |= mask;
        }

        /** @brief Returns true if the button is in released state.
        */
        bool isReleased(MouseButton button) const
        {
             Pt::uint32_t mask = 0x1 << button;
             return (_buttonState & mask) != mask;
        }

        void setReleased(MouseButton button)
        {
            Pt::uint32_t mask = 0x1 << button;
            _buttonState &= (~mask);
        }
    private:
        Pt::uint32_t _buttonState;
};


class MouseEvent : public Event<MouseEvent>
{
    public:    
        enum Action
        {
            Move = 0,
            Press = 1,
            Release = 2
        };

        enum Button
        {
            Left = 0,
            Right = 1,
            Middle = 2,
        };

        explicit MouseEvent()
        : Event(0)
        , _pos(0, 0)
        , _action(Move)
        , _buttonState(0)
        , _button(0)
        { }

        explicit MouseEvent(Element& v)
        : Event( v)
        , _pos(0, 0)
        , _action(Move)
        , _buttonState(0)
        , _button(0)
        { }

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

        bool isMove() const
        {
            return _action == Move;
        }

        void setMove()
        {
            _action = Move;
            _button = 0;
        }

        /** @brief Returns true if the button is in pressed state.
        */
        bool isPressed(Pt::uint32_t button = Left) const
        {
             Pt::uint32_t mask = 0x1 << button;
             return (_buttonState & mask) == mask;
        }

        /** @brief Returns true if the button was just pressed.
        */
        bool isPress(Pt::uint32_t button = Left) const
        {
             Pt::uint32_t mask = 0x1 << button;
             return (_button & mask) == mask && _action == Press;
        }

        void setPress(Pt::uint32_t button = Left)
        {
            Pt::uint32_t mask = 0x1 << button;

            _action = Press;
            _button = mask;
            _buttonState |= mask;
        }

        /** @brief Returns true if the button is in released state.
        */
        bool isReleased(Pt::uint32_t button = Left) const
        {
             Pt::uint32_t mask = 0x1 << button;
             return (_buttonState & mask) != mask;
        }

        /** @brief Returns true if the button was just released.
        */
        bool isRelease(Pt::uint32_t button = Left) const
        {
             Pt::uint32_t mask = 0x1 << button;
             return (_button & mask) == mask && _action == Release;
        }

        void setRelease(Pt::uint32_t button = Left)
        {
            Pt::uint32_t mask = 0x1 << button;

            _action = Release;
            _button = mask;
            _buttonState &= (~mask);
        }

        bool isAnyPress() const
        {
            return (_button != 0 && _action == Press);
        }

    private:
        gfx::Point   _pos;
        Action        _action;
        Pt::uint32_t  _buttonState;
        Pt::uint32_t  _button;
};

}}}

#endif

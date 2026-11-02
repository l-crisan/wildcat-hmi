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
#ifndef WILDCAT_GUI_WINDOW_DESK_FLOWLAYOUT_H
#define WILDCAT_GUI_WINDOW_DESK_FLOWLAYOUT_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/Layout.h>
#include <wildcat/gui/base/TypeFactory.h>

namespace wildcat {
namespace gui {
namespace window {

class WILDCAT_GUI_WINDOW_API FlowLayout : public Layout
{
    typedef Layout Base;

    public:
        // Horizontal          use all space in row, same size for elements
        // HorizontalCenter    place elements accoring to size
        // Vertical            use all space in row, same size for elements
        // VerticalCenter      place elements accoring to size
        
        enum Direction
        {
            Left,
            Right,
            Top,
            Bottom
        };

    public:
        FlowLayout(Direction d = Left);

        virtual ~FlowLayout();

        void setDirection(Direction d);

        void setCenter(bool b);

        void setReverse(bool b);

        void setAutoSize(bool v)
        {
            _autoSize = v;
        }

        bool autoSize() const
        {
            return _autoSize;
        }

    protected:
        virtual gfx::Size onMeasure(const gfx::Size& parentSize);

        virtual void onLayout(const gfx::Rect& rect);

    private:
        gfx::Size onMeasureHorizontal(const gfx::Size& parentSize);

        gfx::Size onMeasureVertical(const gfx::Size& parentSize);

        void onLayoutLeft(const gfx::Rect& rect, bool center);

        void onLayoutRight(const gfx::Rect& rect, bool center);

        void onLayoutTop(const gfx::Rect& rect, bool center);

        void onLayoutBottom(const gfx::Rect& rect, bool center);

    private:
        int directionI() const
        {
            return (int) _direction;
        }


        void setDirectionI(int v)
        {
            setDirection((Direction)v);
        }

    private:
        Direction _direction;
        bool      _center;
        bool      _reverse;
        bool      _autoSize;

    private:
        static base::TypeInit<FlowLayout> _typeInit;
};

}}}

#endif

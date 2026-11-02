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
#ifndef WILDCAT_GUI_WINDOW_SCROLLLAYOUT_H
#define WILDCAT_GUI_WINDOW_SCROLLLAYOUT_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/Layout.h>

namespace wildcat {
namespace gui {
namespace window {

typedef Layout ScrollLayoutBase;

class WILDCAT_GUI_WINDOW_API ScrollLayout : public Layout
{    
    public:
        ScrollLayout();

        virtual ~ScrollLayout();

        void enableScrolling(bool scrollX, bool scrollY);

        int maximumX() const;

        int maximumY() const;

        void scrollX(int xpos);

        void scrollY(int ypos);

        int scrollPosX() const;

        int scrollPosY() const;

        Pt::Signal<int>& scrolledX();

        Pt::Signal<int>& scrolledY();

        void reset();

        void setScrollYDelta(int delta)
        {
            _scrollDtY = delta;
        }   

        int  scrollYDelta() const
        {
            return _scrollDtY;
        }
        
        void setScrollXDelta(int delta)
        {
            _scrollDtX = delta;
        }

        int scrollXDelta() const
        {
            return _scrollDtX;
        }   

    protected:
        virtual gfx::Size onMeasure(const gfx::Size& parentSize);

        virtual void onLayout(const gfx::Rect& rect);

    protected:
        virtual bool onScrollEvent(const gui::window::ScrollEvent& ev);
       
        virtual void onProcessPaintEvent(const PaintEvent& ev);

    private:
        Pt::Signal<int> _scrolledX;
        Pt::Signal<int> _scrolledY;
        gfx::Point _scrollPos;
        int _scrollByX;
        int _scrollByY;
        bool _enableX;
        bool _enableY;
        int _maxX;
        int _maxY;
        int _scrollDtX;
        int _scrollDtY;

    private:
        static base::TypeInit<ScrollLayout> _typeInit;
};

}}}

#endif

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
#include <wildcat/gui/window/ScrollLayout.h>

namespace wildcat {
namespace gui {
namespace window {

base::TypeInit<ScrollLayout> ScrollLayout::_typeInit("wildcat.gui.window.ScrollLayout");

ScrollLayout::ScrollLayout()
: _scrollByX(0)
, _scrollByY(0)
, _enableX(true)
, _enableY(true)
, _maxX(0)
, _maxY(0)
, _scrollDtX(0)
, _scrollDtY(0)
{
    rs(200, 100);

    setName("ScrollLayout");
    setTitle("ScrollLayout");

    setAcceptScrollEvent(true);    
}

ScrollLayout::~ScrollLayout()
{
}

void ScrollLayout::reset()
{
    _scrollByX = 0;
    _scrollByY = 0;
    measure(size());
    invalidate();
}

void ScrollLayout::enableScrolling(bool scrollX, bool scrollY)
{
    _enableX = scrollX;
    _enableY = scrollY;
}

int ScrollLayout::maximumX() const
{
    return _maxX;
}

int ScrollLayout::maximumY() const
{
    return _maxY;
}

void ScrollLayout::scrollX(int posX)
{
    int xpos = posX;

    int maxPosX = _maxX - size().width();

    if( xpos > maxPosX  )
        xpos = maxPosX;

    if(xpos < 0)
        xpos = 0;

    int delta = xpos - _scrollPos.x();


    _scrollByX += delta;

    _scrollPos.setX(xpos);
    _scrolledX.send(xpos);

    invalidate();

}

void ScrollLayout::scrollY(int posY)
{
    int ypos = posY;

    int maxPosY = _maxY - size().height();

    if( ypos > maxPosY  )
        ypos = maxPosY;

    if(ypos < 0)
        ypos = 0;

    int delta = ypos - _scrollPos.y();
    _scrollByY += delta;

    _scrollPos.setY(ypos);
    _scrolledY.send(ypos);

    invalidate();

}

int ScrollLayout::scrollPosX() const
{
    return _scrollPos.x();
}

int ScrollLayout::scrollPosY() const
{
    return _scrollPos.y();
}

Pt::Signal<int>& ScrollLayout::scrolledX()
{
    return _scrolledX;
}

Pt::Signal<int>& ScrollLayout::scrolledY()
{
    return _scrolledY;
}

void ScrollLayout::onProcessPaintEvent(const PaintEvent& ev)
{
    const gfx::Rect& r = ev.rect();
    if (r.isNull())
        return;

    if (!isVisible())
        return;

    View::onProcessPaintEvent(ev);

    gfx::Rect myClip(gfx::Point(1, 1), gfx::Size(size().width()-2, size().height() -2));

    // paint child widgets    
    std::vector<Control*>::const_iterator it;

    for (it = children().begin(); it != children().end(); ++it)
    {
        Control* w = (*it);

        // clip widget update rect
        const gfx::Rect cr = w->geometry();

        gfx::Rect updateRect = cr.intersect(r);

        if (updateRect.isNull())
            continue;

        updateRect = updateRect.intersect(myClip);  

        if (updateRect.isNull())
            continue;

        // paint widget rect
        gfx::Point updatePos = toControl(*w, updateRect.topLeft());
        updateRect.setOrigin(updatePos);

        PaintEvent pev(*w, updateRect);
        w->processEvent(pev);
    }
}

gfx::Size ScrollLayout::onMeasure(const gfx::Size& parentSize)
{
    ScrollLayoutBase::onMeasure(parentSize);

    int maxWidth = 0;
    int maxHeight = 0;

    for(size_t i = 0; i < children().size(); ++i)
    {
        Control* w = children().at(i);

        const gfx::Point& wpos = w->position();
        const gfx::Size& wsize = w->measure(size());

        maxWidth = std::max( maxWidth, wpos.x() +
                                       wsize.width() +
                                       _scrollPos.x() - _scrollByX);

        maxHeight = std::max( maxHeight, wpos.y() +
                                         wsize.height() +
                                         _scrollPos.y() - _scrollByY);
    }

    _maxX = maxWidth;
    _maxY = maxHeight;

    return size();
}

void ScrollLayout::onLayout(const gfx::Rect& rect)
{   
    std::vector<Control*>::const_iterator it;

    for(it = children().begin() ; it != children().end(); ++it)
    {
        Control* w = *it;

        gfx::Point pos = w->position();
        pos.subX(_scrollByX);
        pos.subY(_scrollByY);

        w->setPosition(pos);
        w->setSize(w->preferredSize());
    }

    _scrollByX = 0;
    _scrollByY = 0;    
}

bool ScrollLayout::onScrollEvent(const gui::window::ScrollEvent& ev)
{
    if(ev.wheel() == gui::window::ScrollEvent::Horizontal)
    {
        if(_enableX)
        {
            if(_scrollDtX != 0)
            {
                if(ev.delta() < 0)
                    scrollX(_scrollPos.x() - _scrollDtX);
                else
                    scrollX(_scrollPos.x() + _scrollDtX);
            }
            else
            {   
                int deltaX = _scrollPos.x() - ev.delta();
                scrollX(deltaX);
            }
        }
    }

    if(ev.wheel() == gui::window::ScrollEvent::Vertical)
    {
        if(_enableY)
        {            
            if(_scrollDtY != 0)
            {
                const int deltaY = (ev.delta() < 0) ? (_scrollPos.y() + _scrollDtY) : (_scrollPos.y() - _scrollDtY);

                scrollY(deltaY);
            }
            else
            {
                const int deltaY = _scrollPos.y() - ev.delta();
                scrollY(deltaY);    
            }
        }
    }

    return true;
}

}}}

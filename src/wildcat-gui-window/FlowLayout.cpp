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
#include <wildcat/gui/window/FlowLayout.h>

namespace wildcat {
namespace gui {
namespace window {

base::TypeInit<FlowLayout> FlowLayout::_typeInit("wildcat.gui.window.FlowLayout");

FlowLayout::FlowLayout(Direction d)
: _direction(d)
, _center(false)
, _reverse(false)
, _autoSize(false)
{
    rs(200, 100);

    setName("FlowLayout");
    setTitle("FlowLayout");

    registerProperty("Direction", *this, &FlowLayout::directionI, &FlowLayout::setDirectionI,"Cat=Behavoir;Editor=Combo;Param=[Left,Right,Top,Bottom]");       
}

FlowLayout::~FlowLayout()
{
}


void FlowLayout::setDirection(Direction d)
{ 
    _direction = d;
    
    relayout();
}

void FlowLayout::setCenter(bool b)
{
    _center = b;
}

void FlowLayout::setReverse(bool b)
{
    _reverse = b;
}

gfx::Size FlowLayout::onMeasure(const gfx::Size& parentSize)
{
    Base::onMeasure(parentSize);
    gfx::Size contentSize;

    switch(_direction)
    {
        default:
        case Left:
            contentSize =  onMeasureHorizontal(parentSize);
            break;

        case Right:
            contentSize = onMeasureHorizontal(parentSize);
            break;

        case Top:
            contentSize = onMeasureVertical(parentSize);
            break;

        case Bottom:
            contentSize = onMeasureVertical(parentSize);
        break;
    }

    return _autoSize ? contentSize : size();
}

void FlowLayout::onLayout(const gfx::Rect& rect)
{
    switch(_direction)
    {
        default:
        case Left:
            onLayoutLeft(rect, _center);
            break;

        case Right:
            onLayoutRight(rect, _center);
            break;

        case Top:
            onLayoutTop(rect, _center);
            break;

        case Bottom:
            onLayoutBottom(rect, _center);
            break;
    }
}

gfx::Size FlowLayout::onMeasureHorizontal(const gfx::Size& parentSize)
{
    std::vector<Control*>::const_iterator it = children().begin();
    std::vector<Control*>::const_iterator end = children().end();

    gfx::Size contentSize;

    for( ; it != end; ++it)
    {
        Control* item = *it;
        
        if( ! item->isVisible() )
            continue;  

        gfx::Size itemSize( size().width(), 
                             size().height() - 
                             padding().topBottom() - 
                             item->margin().topBottom() );

        gfx::Size prefSize = item->measure(itemSize);
        
        prefSize.addWidth( item->margin().leftRight() );
        prefSize.addHeight( padding().topBottom() + item->margin().topBottom() );

        contentSize.addWidth( prefSize.width() );
        contentSize.setHeight( std::max(contentSize.height(), size().height()) );
    }

    contentSize.addWidth( padding().leftRight() );
    contentSize.addHeight( padding().topBottom() );
    return contentSize;
}

gfx::Size FlowLayout::onMeasureVertical(const gfx::Size& parentSize)
{
    std::vector<Control*>::const_iterator it = children().begin();
    std::vector<Control*>::const_iterator end = children().end();

    gfx::Size contentSize;

    for( ; it != end; ++it)
    {
        Control* item = *it; 

        if( ! item->isVisible() )
            continue; 

        gfx::Size itemSize( size().width() - 
                             padding().leftRight() -
                             item->margin().leftRight(), 
                             size().height() );

        gfx::Size prefSize =  item->measure(itemSize);
  
        prefSize.addWidth( padding().leftRight() + item->margin().leftRight() );
        prefSize.addHeight( item->margin().topBottom() );

        contentSize.setWidth( std::max(contentSize.width(), 
                                       prefSize.width())  );
        contentSize.addHeight( prefSize.height() );
    }

    contentSize.addWidth( padding().leftRight() );
    contentSize.addHeight( padding().topBottom() );
    return contentSize;
}

void FlowLayout::onLayoutLeft(const gfx::Rect& rect, bool center)
{
    int posX = padding().left();
    
    if(center)
    {
        int itemsWidth = 0;

        std::vector<gui::window::Control*>::const_iterator it;
        for(it = children().begin(); it != children().end(); ++it)
        {
          Control* item = *it;
          itemsWidth += item->preferredSize().width();
          itemsWidth += item->margin().leftRight();
        }

        posX = (rect.width() - itemsWidth) / 2;
    }

    if (!_reverse)
    {
        std::vector<Control*>::const_iterator it = children().begin();
        std::vector<Control*>::const_iterator end = children().end();

        for (; it != end; ++it)
        {
            Control* item = *it;

            if (!item->isVisible())
                continue;

            int x = posX + item->margin().left();
            int y = padding().top() + item->margin().top();

            posX += item->preferredSize().width() +
                    item->margin().leftRight();

            gfx::Size itemSize(item->preferredSize().width(),
                                rect.size().height() -
                                padding().top() -
                                padding().bottom() -
                                item->margin().top() -
                                item->margin().bottom());

            gfx::Point pos(x, y);

            item->setPosition(pos);
            item->setSize(itemSize);
        }
    }
    else
    {
        std::vector<Control*>::const_reverse_iterator it = children().rbegin();
        std::vector<Control*>::const_reverse_iterator end = children().rend();

        for (; it != end; ++it)
        {
            Control* item = *it;

            if (!item->isVisible())
                continue;

            int x = posX + item->margin().left();
            int y = padding().top() + item->margin().top();

            posX += item->preferredSize().width() +
                    item->margin().leftRight();

            gfx::Size itemSize(item->preferredSize().width(),
                                rect.size().height() -
                                padding().top() -
                                padding().bottom() -
                                item->margin().top() -
                                item->margin().bottom());

            gfx::Point pos(x, y);

            item->setPosition(pos);
            item->setSize(itemSize);
        }
    }
}

void FlowLayout::onLayoutRight(const gfx::Rect& rect, bool center)
{
    int posRight  = rect.width() - padding().right();

    if(center)
    {
        int itemsWidth = 0;

        std::vector<gui::window::Control*>::const_iterator it;
        for(it = children().begin(); it != children().end(); ++it)
        {
          Control* item = *it;
          itemsWidth += item->preferredSize().width();
          itemsWidth += item->margin().leftRight();
        }

        posRight -= (rect.width() - itemsWidth) / 2;
    }

    if (!_reverse)
    {
        std::vector<Control*>::const_iterator it = children().begin();
        std::vector<Control*>::const_iterator end = children().end();

        for (; it != end; ++it)
        {
            Control* item = *it;

            if (!item->isVisible())
                continue;

            posRight -= item->preferredSize().width();
            posRight -= item->margin().right();

            int x = posRight;
            int y = padding().top() + item->margin().top();

            posRight -= item->margin().left();

            gfx::Size itemSize(item->preferredSize().width(),
                rect.size().height() -
                padding().top() -
                padding().bottom() -
                item->margin().top() -
                item->margin().bottom());

            gfx::Point pos(x, y);

            item->setPosition(pos);
            item->setSize(itemSize);
        }
    }
    else
    {
        std::vector<Control*>::const_reverse_iterator it = children().rbegin();
        std::vector<Control*>::const_reverse_iterator end = children().rend();

        for (; it != end; ++it)
        {
            Control* item = *it;

            if (!item->isVisible())
                continue;

            posRight -= item->preferredSize().width();
            posRight -= item->margin().right();

            int x = posRight;
            int y = padding().top() + item->margin().top();

            posRight -= item->margin().left();

            gfx::Size itemSize(item->preferredSize().width(),
                rect.size().height() -
                padding().top() -
                padding().bottom() -
                item->margin().top() -
                item->margin().bottom());

            gfx::Point pos(x, y);

            item->setPosition(pos);
            item->setSize(itemSize);
        }
    }
}

void FlowLayout::onLayoutTop(const gfx::Rect& rect, bool center)
{
    int posTop = padding().top();
    
    if(center)
    {
        int itemsHeight = 0;

        std::vector<gui::window::Control*>::const_iterator it;
        for(it = children().begin(); it != children().end(); ++it)
        {
          Control* item = *it;
          itemsHeight += item->preferredSize().height();
          itemsHeight += item->margin().topBottom();
        }

        posTop = (rect.height() - itemsHeight) / 2;
    }

    if (!_reverse)
    {
        std::vector<Control*>::const_iterator it = children().begin();
        std::vector<Control*>::const_iterator end = children().end();

        for (; it != end; ++it)
        {
            Control* item = *it;

            if (!item->isVisible())
                continue;

            int x = padding().left() + item->margin().left();
            int y = posTop + item->margin().top();

            posTop += item->preferredSize().height() +
                item->margin().topBottom();

            gfx::Size itemSize(rect.size().width() -
                padding().left() -
                padding().right() -
                item->margin().left() -
                item->margin().right(),
                item->preferredSize().height());

            gfx::Point pos(x, y);

            item->setPosition(pos);
            item->setSize(itemSize);
        }
    }
    else
    {
        std::vector<Control*>::const_reverse_iterator it = children().rbegin();
        std::vector<Control*>::const_reverse_iterator end = children().rend();

        for (; it != end; ++it)
        {
            Control* item = *it;

            if (!item->isVisible())
                continue;

            int x = padding().left() + item->margin().left();
            int y = posTop + item->margin().top();

            posTop += item->preferredSize().height() +
                item->margin().topBottom();

            gfx::Size itemSize(rect.size().width() -
                padding().left() -
                padding().right() -
                item->margin().left() -
                item->margin().right(),
                item->preferredSize().height());

            gfx::Point pos(x, y);

            item->setPosition(pos);
            item->setSize(itemSize);
        }
    }
}

void FlowLayout::onLayoutBottom(const gfx::Rect& rect, bool center)
{
    int posBottom = rect.height() - padding().bottom();

    if(center)
    {
        int itemsHeight = 0;

        std::vector<gui::window::Control*>::const_iterator it;
        for(it = children().begin(); it != children().end(); ++it)
        {
          Control* item = *it;
          itemsHeight += item->preferredSize().height();
          itemsHeight += item->margin().topBottom();
        }

        posBottom -= (rect.height() - itemsHeight) / 2;
    }

    if (!_reverse)
    {
        std::vector<Control*>::const_iterator it = children().begin();
        std::vector<Control*>::const_iterator end = children().end();

        for (; it != end; ++it)
        {
            Control* item = *it;

            if (!item->isVisible())
                continue;

            posBottom -= item->preferredSize().height();
            posBottom -= item->margin().bottom();

            int x = padding().left() + item->margin().left();
            int y = posBottom;

            posBottom -= item->margin().top();

            gfx::Size itemSize(rect.size().width() -
                padding().left() -
                padding().right() -
                item->margin().left() -
                item->margin().right(),
                item->preferredSize().height());

            gfx::Point pos(x, y);

            item->setPosition(pos);
            item->setSize(itemSize);
        }
    }
    else
    {
        std::vector<Control*>::const_reverse_iterator it = children().rbegin();
        std::vector<Control*>::const_reverse_iterator end = children().rend();

        for (; it != end; ++it)
        {
            Control* item = *it;

            if (!item->isVisible())
                continue;

            posBottom -= item->preferredSize().height();
            posBottom -= item->margin().bottom();

            int x = padding().left() + item->margin().left();
            int y = posBottom;

            posBottom -= item->margin().top();

            gfx::Size itemSize(rect.size().width() -
                padding().left() -
                padding().right() -
                item->margin().left() -
                item->margin().right(),
                item->preferredSize().height());

            gfx::Point pos(x, y);

            item->setPosition(pos);
            item->setSize(itemSize);
        }
    }
}

}}}

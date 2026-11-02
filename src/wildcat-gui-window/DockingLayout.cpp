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
#include <wildcat/gui/window/DockingLayout.h>
#include <wildcat/gui/window/Application.h>

namespace wildcat {
namespace gui {
namespace window {

wildcat::gui::base::TypeInit<DockingLayout> DockingLayout::_typeInit("wildcat.gui.window.DockingLayout");

DockingLayout::DockingLayout()
{
    rs(200,100);
    setName("DockingLayout");
    setTitle("DockingLayout");
}

DockingLayout::~DockingLayout()
{
}


gfx::Size DockingLayout::onMeasure(const gfx::Size& parentSize)
{
    gfx::Size policy  = size();
    gfx::Size fillSize = size();

    fillSize.subWidth( padding().leftRight() );
    fillSize.subHeight( padding().topBottom() );

    Control* fillWidget = 0;
    bool hasDockedLeftRight = false;
    bool hasDockedTopBottom = false;

    std::vector<Control*>::const_iterator it;
    std::vector<Control*>::const_iterator end = children().end();

    for(it = children().begin(); it != end; ++it)
    {
        if( ! (*it)->isVisible() )
            continue;

        DockPosition d = (*it)->dockPosition();

        if( d == Control::DockPosNone )
            continue;

        const int hspace = padding().leftRight() + (*it)->margin().leftRight();
        const int vspace = padding().topBottom() + (*it)->margin().topBottom();

        
        switch(d)
        {
            case DockingLayout::DockPosCenter:
                fillWidget = *it;
                break;

            case DockingLayout::DockPosLeft:
            case DockingLayout::DockPosRight:
            {
                gfx::Size itemPolicy;
                itemPolicy.setWidth( policy.width() - hspace );
                itemPolicy.setHeight( fillSize.height() - (*it)->margin().topBottom() );

                const gfx::Size itemSize = (*it)->measure(itemPolicy);

                int itemWidth = itemSize.width() + (*it)->margin().leftRight();
                fillSize.subWidth(itemWidth);
                break;
            }

            case DockingLayout::DockPosTop:
            case DockingLayout::DockPosBottom:
            {
                gfx::Size itemPolicy;
                itemPolicy.setWidth( fillSize.width() - (*it)->margin().leftRight() );
                itemPolicy.setHeight( policy.height() - vspace );

                const gfx::Size itemSize = (*it)->measure(itemPolicy);

                const int itemHeight = itemSize.height() + (*it)->margin().topBottom();
                fillSize.subHeight(itemHeight);
                break;
            }
        }
    }


    if(fillWidget)
    {
        gfx::Size itemSize;
        itemSize.setWidth( fillSize.width() - fillWidget->margin().leftRight() );
        itemSize.setHeight( fillSize.height() - fillWidget->margin().topBottom() );

        fillWidget->measure(itemSize);
    }

    return size();
}

void DockingLayout::onLayout(const gfx::Rect& rect)
{
    std::vector<Control*>::const_iterator it  = children().begin();
    std::vector<Control*>::const_iterator end = children().end();

    int posLeft   = padding().left();
    int posTop    = padding().top();
    int posRight  = rect.width() -padding().right();
    int posBottom = rect.height() - padding().bottom();
    bool hasFilled   = false;

    for( ; it != end; ++it)
    {

        if( ! (*it)->isVisible() )
            continue;

        DockPosition d = (*it)->dockPosition();

        
        if( d == Control::DockPosNone )
            continue;

        switch(d)
        {
            default:
            case DockingLayout::DockPosCenter:
                hasFilled = true;
            break;

            case DockingLayout::DockPosLeft:
            {
                const int x = posLeft + (*it)->margin().left();
                const int y = posTop  + (*it)->margin().top();

                const gfx::Size itemSize( (*it)->preferredSize().width(),
                                          (posBottom - posTop ) - (*it)->margin().topBottom() );

                const gfx::Point pos(x, y);

                (*it)->setPosition(pos);
                (*it)->setSize(itemSize);

                posLeft += (*it)->preferredSize().width()  + (*it)->margin().leftRight();

                break;
            }

            case DockingLayout::DockPosTop:
            {
                const int x = posLeft + (*it)->margin().left();
                const int y = posTop  + (*it)->margin().top();

                const gfx::Size itemSize( (posRight - posLeft ) - (*it)->margin().leftRight(),
                                          (*it)->preferredSize().height() );

                const gfx::Point pos(x, y);
  
                (*it)->setPosition(pos);
                (*it)->setSize(itemSize);

                posTop += (*it)->preferredSize().height() + (*it)->margin().topBottom();

                break;
            }

            case DockingLayout::DockPosRight:
            {
                posRight -= (*it)->preferredSize().width()+ (*it)->margin().right();

                const int x = posRight;
                const int y = posTop + (*it)->margin().top();

                posRight -=  (*it)->margin().left();

                const gfx::Size itemSize( (*it)->preferredSize().width(),
                                     (posBottom - posTop ) -
                                     (*it)->margin().topBottom() );

                const gfx::Point pos(x, y);

                (*it)->setPosition(pos);
                (*it)->setSize(itemSize);
                break;
            }

            case DockingLayout::DockPosBottom:
            {
                const int preferredHeight = (*it)->preferredSize().height();

                posBottom -= preferredHeight + (*it)->margin().bottom();

                const int x = posLeft + (*it)->margin().left();
                const int y = posBottom;

                posBottom -= (*it)->margin().top();

                const gfx::Size itemSize( (posRight - posLeft) - (*it)->margin().leftRight(),
                                     preferredHeight );
                
                const gfx::Point pos(x, y);

                (*it)->setPosition(pos);
                (*it)->setSize(itemSize);
                break;
            }
        }
    }

    if( ! hasFilled )
    {
        Pt::Any a;
        layoutSignal().send(*this, a);
        return;
    }

    gfx::Size fillSize(posRight - posLeft + 1,
                        posBottom - posTop + 1);

    for(it = children().begin(); it != end; ++it)
    {
        DockPosition d = (*it)->dockPosition();

        if(d == DockingLayout::DockPosCenter)
        {
            if( ! (*it)->isVisible() )
                continue;

            const gui::window::Spacing& margin = (*it)->margin();

            gfx::Point pos(posLeft + margin.left(),
                            posTop + margin.top());

            gfx::Size itemSize( fillSize.width() - margin.leftRight()  - 1 ,
                                 fillSize.height() - margin.topBottom()  - 1 );

            (*it)->setPosition(pos);
            (*it)->setSize(itemSize);
        }
    } 

    Pt::Any a;
    layoutSignal().send(*this, a);   
}

}}}

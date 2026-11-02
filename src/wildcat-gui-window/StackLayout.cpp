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
#include <wildcat/gui/window/StackLayout.h>

namespace wildcat {
namespace gui {
namespace window {

gui::base::TypeInit<StackLayout> StackLayout::_typeInit("wildcat.gui.window.StackLayout");


StackLayout::StackLayout()
: _current(0)
{
    rs(200, 100);
    setName("StackLayout");  
    setTitle("StackLayout");
}

StackLayout::~StackLayout()
{
}

void StackLayout::onAddChild(Control& w)
{
    Layout::onAddChild(w);

    w.show(true);

    if (_current)
        _current->show(false);

    _current = &w;
}

void StackLayout::onRemoveChild(Control& w)
{
    Layout::onRemoveChild(w);

    if( _current == &w)
        _current = 0;  
}


gfx::Size StackLayout::onMeasure(const gfx::Size& parentSize)
{
    Base::onMeasure(parentSize);

    std::vector<Control*>::const_iterator it;

    for (it = children().begin(); it != children().end(); ++it)
        (*it)->measure(size());

    return size();
}

int StackLayout::current() const
{
    return _current == 0 ? -1 :indexOf(*_current);
}

void StackLayout::setCurrent(int n)
{
    for (size_t i = 0; i < children().size(); ++i)
        children().at(i)->show(false);

    _current = 0;
    
    if( children().empty())
    {
        Pt::Any a(-1);
        _currentChanged.send(*this,a);
        return;
    }

    _current = childAt(n);

    if( _current)
    {
        _current->show(true);

        Pt::Any a(n);
        _currentChanged.send(*this, a);
    }   
    else
    {
        Pt::Any a(-1);
        _currentChanged.send(*this, a );
    }
}


void StackLayout::onMoveEvent(const MoveEvent& ev)
{
    Base::onMoveEvent(ev);
}

void StackLayout::onLayout(const gfx::Rect& rect)
{
    std::vector<Control*>::const_iterator it;

    for (it = children().begin(); it != children().end(); ++it)
    {   
        Control* widget = *it;

        gfx::Point pos(padding().left() + widget->margin().left(), 
                        padding().top()  + widget->margin().top());
        
        const int hspace = padding().leftRight() + widget->margin().leftRight();
        const int vspace = padding().topBottom() + widget->margin().topBottom();

        gfx::Size s;
        s.setWidth( size().width() - hspace );
        s.setHeight(size().height() - vspace );

        widget->setPosition(pos);
        widget->setSize(s);
    }
}

}}}

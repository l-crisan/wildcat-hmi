
/*
   Copyright (C) 2026 Marc Boris Duerner
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
#include <wildcat/gui/window/View.h>
#include <wildcat/gui/window/Widget.h>
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/MouseEvent.h>
#include <wildcat/gui/window/TouchEvent.h>
#include <wildcat/gui/window/ScrollEvent.h>
#include <wildcat/gui/window/EnterEvent.h>
#include <wildcat/gui/window/LeaveEvent.h>
#include <wildcat/gui/window/KeyEvent.h>

namespace wildcat {
namespace gui {
namespace window {

View::View()
: _designMode(false)
, _iconId(0)
{
    registerProperty("DesignMode", *this, &View::designMode, &View::setDesignMode, "Design=false;Serialize=false");
    registerProperty("Title", *this, &View::title, &View::setTitle, "Cat=Apparence");
    registerProperty("Icon", *this, &View::iconId, &View::setIconId, "Cat=Apparence;Editor=Icon");
}

View::~View()
{
}

gfx::Point View::toControl(const Control& widget,  const gfx::Point& pos) const
{ 
    return onToControl(widget, pos); 
}

gfx::Point View::fromControl(const Control& widget, const gfx::Point& pos) const
{ 
    return onFromControl(widget, pos);
}

void View::onSetIcon(uint32_t id)
{
    _iconId = id;
    Pt::Any a(id);
    _iconChanged.send(*this, a);
    invalidate();
}

const gfx::Pixmap& View::iconImage() const
{
    return _iconPixmap;
}

void View::setIcon(const std::string& name)
{
    const uint32_t id = resourceMng().id(name);

    setIconId(id);
}

void View::onInvalidate()
{
    _trTitle = tr(_title);
    _iconPixmap.reset();

    const gfx::Icon* p = getRecource<gfx::Icon>(_iconId);

    if (p != nullptr)
        _iconPixmap = p->getImage(Application::inst().screen().scaleFactor());


    Element::onInvalidate();    
}

bool View::onGetDesignMode() const
{
    return _designMode;
}

void View::onSetDesignMode(bool mode)
{
    _designMode = mode;
}

}}}

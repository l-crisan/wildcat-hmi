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
#include <wildcat/gui/window/MdiArea.h>
#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/MouseEvent.h>
#include <wildcat/gui/window/KeyEvent.h>
#include <wildcat/gui/window/LayoutEvent.h> // RescaleEvent
#include <wildcat/gui/window/PaintEvent.h>
#include <wildcat/gui/gfx/Point.h>
#include <Pt/String.h>
#include <cmath>

namespace wildcat {
namespace gui {
namespace window {

base::TypeInit<MdiArea> MdiArea::_typeInit("wildcat.gui.window.MdiArea");

MdiArea::MdiArea()
: _content(0)
, _pointer(0)
{
    rs(400, 300);

    setFocusPolicy(FocusPolicy::NoFocus);
    setName("MdiContainer");
    setTitle("MdiContainer");

    _wm.setParent(this);

    unregisterProperty("Mnemonic");
    unregisterProperty("Shortcut");
    unregisterProperty("ActionKey");
    unregisterProperty("Icon");
    unregisterProperty("MinimumSize");
    unregisterProperty("MaximumSize");
}

MdiArea::~MdiArea()
{
}

void MdiArea::addWindow(Window& w)
{
    w.setParent(_wm);
}

void MdiArea::removeWindow(Window& w)
{
    w.unparent();
}

const std::vector<Window*>& MdiArea::windows() const
{
    return _wm.windows();
}

WindowManagerBase& MdiArea::windowManager()
{
    return _wm;
}

Control* MdiArea::content()
{
    return _content;
}

const Control* MdiArea::content()  const
{
    return _content;
}

void MdiArea::setContent(Control* widget)
{
    if(_content)
    {
        _content->unparent();
    }

    _content = widget;

    if(widget)
    {
        widget->setParent(*this);
    }
}

///////////////////////////////////////////////////////////////////////
// Widget
///////////////////////////////////////////////////////////////////////

void MdiArea::onSetGraphics(gfx::Graphics* gf, const gfx::Point& pos)
{
    Base::onSetGraphics(gf, pos);

    _wm.setGraphics( gf, pos );
}

void MdiArea::onRemoveChild(Control& w)
{
    Control::onRemoveChild(w);

    if(&w == _content)
        _content = 0;
}

///////////////////////////////////////////////////////////////////////
// Element
///////////////////////////////////////////////////////////////////////

Element* MdiArea::onHitTest(const gfx::Point& p, HitTestHint hint)
{    
    if( ! bounds().contains(p) )
        return 0;

    // TODO: convert to WM coordinates

    Element* hit = _wm.hitTest(p, hint);
    if(hit)
        return hit;

    return Control::onHitTest(p, hint);
}

///////////////////////////////////////////////////////////////////////
// Widget
///////////////////////////////////////////////////////////////////////

gfx::Size MdiArea::onMeasure(const gfx::Size& policy)
{
    if(_content)
        return _content->measure(policy);

    return size();
}

void MdiArea::onLayout(const gfx::Rect& rect)
{
    Control::onLayout(rect);

    if(_content)
    {        
        gfx::Point pos(padding().left() + _content->margin().left(), 
                        padding().top()  + _content->margin().top());
        
        int hspace = padding().leftRight() + _content->margin().leftRight();
        int vspace = padding().topBottom() + _content->margin().topBottom();

        gfx::Size size;
        size.setWidth( rect.width() - hspace );
        size.setHeight( rect.height() - vspace );

        _content->setPosition(pos);
        _content->setSize(size);
    }
}

void MdiArea::onPaint(gfx::Graphics& gf, const gfx::Rect& rect)
{
    Control::onPaint(gf, rect);
}


void MdiArea::onProcessPaintEvent(const PaintEvent& ev)
{
    Control::onProcessPaintEvent(ev);

    const gfx::Rect& rect = ev.rect();

    PaintEvent pev(_wm, rect);
    _wm.processEvent(pev);
}

void MdiArea::onProcessResizeEvent(const ResizeEvent& ev)
{
    Control::onProcessResizeEvent(ev);

    ResizeEvent rev(_wm, ev.size());
    _wm.processEvent(rev);
}

void MdiArea::onProcessMoveEvent(const MoveEvent& ev)
{
    Control::onProcessMoveEvent(ev);
   
   _wm.processEvent(ev);
}

void MdiArea::onProcessEnableEvent(const EnableEvent& ev)
{
    Control::onProcessEnableEvent(ev);

    EnableEvent eev(_wm, ev.enabled());
    _wm.processEvent(eev);
}


bool MdiArea::onProcessMouseEvent(const MouseEvent& ev)
{
    if( ! acceptsInput() )
        return false;
    
    bool consumed = _wm.processMouseEvent(ev);
    if(consumed)
        return true;

    return Control::onProcessMouseEvent(ev);
}

bool MdiArea::onProcessTouchEvent(const TouchEvent& ev)
{
    bool consumed = _wm.processTouchEvent(ev);
    if(consumed)
        return true;

    return Control::onProcessTouchEvent(ev);
}

void MdiArea::onProcessEnterEvent(const EnterEvent& ev)
{
    Control::onProcessEnterEvent(ev);
}

void MdiArea::onProcessLeaveEvent(const LeaveEvent& ev)
{
    Control::onProcessLeaveEvent(ev);
}

bool MdiArea::onProcessScrollEvent(const ScrollEvent& ev)
{
    if( ! acceptsInput() )
        return false;

    if( _wm.activeWindow() )
        return _wm.processEvent(ev);

    return Control::onProcessScrollEvent(ev);
}

void MdiArea::onMoveEvent(const MoveEvent& ev)
{
    Control::onMoveEvent(ev);
}

void MdiArea::onFocusEvent(const FocusEvent& ev)
{    
    Control::onFocusEvent(ev);
}

bool MdiArea::onProcessKeyEvent(const KeyEvent& ev)
{
    if( ! acceptsInput() )
        return false;

    if(_wm.activeWindow())
        return _wm.processEvent(ev);

    return Control::onProcessKeyEvent(ev);
}


void MdiArea::onUpdateScaling()
{
    Control::onUpdateScaling();

    _wm.updateScaling();
}


///////////////////////////////////////////////////////////////////////
// WindowManager
///////////////////////////////////////////////////////////////////////

void MdiArea::onRepaint(WindowManagerBase& wm, const gfx::Rect& rect)
{
    repaint(rect);
}

void MdiArea::onActivate(WindowManagerBase& wm, bool active)
{
    activate(active);
}

void MdiArea::onRequestActivate(bool active)
{
    if (_wm.activeWindow() && !active)
        _wm.activeWindow()->activate(false);
        
    Control::onActivateRequest(*this, active);
}


}}} 

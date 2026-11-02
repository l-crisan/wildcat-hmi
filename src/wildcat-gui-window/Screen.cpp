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
#include "ScreenImpl.h"

#include <wildcat/gui/window/Screen.h>
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/LayoutEvent.h> // RescaleEvent
#include <wildcat/gui/window/ResizeEvent.h>
#include <wildcat/gui/window/MouseEvent.h>
#include <wildcat/gui/window/TouchEvent.h>
#include <wildcat/gui/window/ScrollEvent.h>
#include <wildcat/gui/window/KeyEvent.h>
#include <wildcat/gui/window/PaintEvent.h>
#include <wildcat/gui/window/Window.h>

namespace wildcat {
namespace gui {
namespace window {

Screen::Screen(ApplicationImpl& app)
: _impl( new ScreenImpl(app) )
, _updates(0)
{

    setName("Screen");  
    setScaleFactor(1);

    _impl->setParent(this);    
}

Screen::~Screen()
{
    delete _impl;
}

void Screen::addWindow(Window& w)
{
    _impl->addWindow(w);
}

void Screen::removeWindow(Window& w)
{
    _impl->removeWindow(w);
}

const std::vector<Window*>& Screen::windows() const
{
  return _impl->windows();
}

WindowManagerBase& Screen::windowManager()
{
  return _impl->windowManager();
}


///////////////////////////////////////////////////////////////////////
// ScreenImpl
///////////////////////////////////////////////////////////////////////

ScreenImpl* Screen::impl()
{
    return _impl;
}

void Screen::onInit(ScreenImpl& s)
{
}

void Screen::onRelease(ScreenImpl& s)
{
}

void Screen::onResize(ScreenImpl& s, const gfx::Size& size)
{
    ResizeEvent rev(*this, size);
    processEvent(rev);
}

void Screen::onShow(ScreenImpl& s, bool isShow)
{
    ShowEvent showEv(*this, true);
    processEvent(showEv);
}

///////////////////////////////////////////////////////////////////////
// Element
///////////////////////////////////////////////////////////////////////

Element* Screen::onHitTest(const gfx::Point& p, HitTestHint hint)
{
    if( ! bounds().contains(p) )
        return 0;

    // TODO: convert to WM coordinates
    gfx::Point pos = p;
    
    Element* hit = _impl->hitTest(pos, hint);
    if(hit)
        return hit;

    return this;
}

gfx::Point Screen::onToParent(const gfx::Point& pos) const
{
    return pos;
}

gfx::Point Screen::onFromParent(const gfx::Point& pos) const
{
    return pos;
}

gfx::Point Screen::onToGlobal(const gfx::Point& pos) const
{
    return pos;
}

gfx::Point Screen::onFromGlobal(const gfx::Point& pos) const
{
    return pos;
}

bool Screen::onProcessEvent(const Pt::Event& ev)
{
    return Base::onProcessEvent(ev);
}

//
// scaling
//


void Screen::setScaleFactor(double v)
{
    _impl->setScaleFactor(v);
    updateScaling();       
    Application::instance().invalidate();   

}

double Screen::scaleFactor() const
{
    return _impl->scaleFactor();
}   

void Screen::onUpdateScaling()  
{
    Base::onUpdateScaling();

    for(size_t i = 0; i < _impl->windows().size(); ++i)
    {
        Window* w = _impl->windows()[i];

        if(w)
            w->updateScaling(); 
    }   
}

//
// painting
//

void Screen::onRequestRepaint(const gfx::Rect& rect)
{
    _updateRect.unify(rect);
    ++_updates;

    PaintEvent uev(*this, _updateRect);
    Application::instance().loop().commitEvent(uev);
}

void Screen::onProcessPaintEvent(const PaintEvent& ev)
{
    if(_updates == 0)
        return;

    --_updates;

    // skip all updates except the last one
    if(_updates > 0)
      return;        

    _updateRect.clear();

    
    Base::onProcessPaintEvent(ev);

    const gfx::Rect& screenRect = ev.rect();
    PaintEvent pev(*_impl, screenRect);
    
    _impl->processEvent(pev);
    

}

void Screen::onPaintEvent(const PaintEvent& ev)
{    
    Base::onPaintEvent(ev);

    const gfx::Rect& rect = ev.rect();
    onPaint(rect);
}

void Screen::onPaint(const gfx::Rect& rect)
{
}

//
// geometry
//

void Screen::onProcessResizeEvent(const ResizeEvent& ev)
{
    Base::onProcessResizeEvent(ev);

    _impl->processEvent(ev);
}

void Screen::onResizeEvent(const ResizeEvent& ev)
{
    Base::onResizeEvent(ev);
}

//
// input
//

bool Screen::onProcessMouseEvent(const MouseEvent& ev)
{
    return _impl->processEvent(ev);
}

bool Screen::onMouseEvent(const MouseEvent& ev)
{ 
    return Base::onMouseEvent(ev);
}

bool Screen::onProcessTouchEvent(const TouchEvent& ev)
{
    return _impl->processEvent(ev);
}

bool Screen::onTouchEvent(const TouchEvent& ev)
{ 
    return Base::onTouchEvent(ev);
}

bool Screen::onProcessScrollEvent(const ScrollEvent& ev)
{
    return _impl->processEvent(ev);
}

bool Screen::onScrollEvent(const ScrollEvent& ev)
{ 
    return Base::onScrollEvent(ev);
}

bool Screen::onProcessKeyEvent(const KeyEvent& ev)
{
    return _impl->processEvent(ev);
}

bool Screen::onKeyEvent(const KeyEvent& ev)
{ 
    return Base::onKeyEvent(ev);
}

}}}

/*
   Copyright (C) 2010-2026 by Laurentiu-Gheorghe Crisan
   Copyright (C) 2010-2026 by ATESiON GmbH
   Copyright (C) 2026 Marc Boris Duerner

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
#include "ApplicationImpl.h"
#include "WindowFrameImpl.h"
#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/PaintEvent.h>
#include <Windows.h>

namespace wildcat {
namespace gui {
namespace window {

ScreenImpl::ScreenImpl(ApplicationImpl&)
: _parent(0)
, _screenScaling(1.0)
, _scaleFactor(1.0)
{
    HWND desktop = GetDesktopWindow();
    HDC screenDC = GetDC(desktop);

    int dpix = GetDeviceCaps(screenDC, LOGPIXELSX);
    //std::clog << "SCREEN SCALING DPI: " << dpix << std::endl;
    
    _screenScaling = dpix / 96.0;
    //std::clog << "SCREEN SCALING: " << _screenScaling << std::endl;

    ReleaseDC(desktop, screenDC);
}


ScreenImpl::~ScreenImpl()
{
    while( ! _windows.empty() )
        _windows.back()->unparent();

    setParent(0);
}


void ScreenImpl::setParent(Screen* screen)
{
    _parent = screen;

    if(_parent)
    {
        HWND desktop = GetDesktopWindow();

        RECT r;   
        GetWindowRect(desktop, &r);
    
        gfx::Size size(r.right, r.bottom);
        size.scale(1.0/_scaleFactor);

        _parent->onResize(*this, size);
        _parent->onShow(*this, true);
    }

    onSetParent(_parent);
}

void ScreenImpl::onUpdateScaling()
{
    WindowManagerBase::onUpdateScaling();   

    if (_parent)
    {
        HWND desktop = GetDesktopWindow();

        RECT r;
        GetWindowRect(desktop, &r);

        gfx::Size size(r.right, r.bottom);
        size.scale(1.0/_scaleFactor);

        _parent->onResize(*this, size);
        _parent->onShow(*this, true);
    }
}



void ScreenImpl::addWindow(Window& w)
{
    w.setParent(*this); 
}


void ScreenImpl::removeWindow(Window& w)
{ 
    w.unparent(); 
}


const std::vector<Window*>& ScreenImpl::windows() const
{
    return _windows;
}


WindowManagerBase& ScreenImpl::windowManager()
{
    return *this;
}


gfx::Point ScreenImpl::toFrame(const WindowFrameImpl& frame,
                                const gfx::Point& pos) const
{
    return frame.fromScreen(pos);
}


gfx::Point ScreenImpl::fromFrame(const WindowFrameImpl& frame,
                                  const gfx::Point& pos) const
{
    return frame.toScreen(pos);
}


void ScreenImpl::setCapture(Element* capture)
{
    if( ! capture )
    {
        //std::clog << "RELEASE CAPTURE HWND" << std::endl;
        ::ReleaseCapture();
        return;
    }

    std::vector<Window*>::iterator wit;
    for(wit = _windows.begin(); wit != _windows.end(); ++wit)
    {
        Window* window = *wit;
        
        if( capture == window || capture->isDescendantOf(*window) )
        {
            WindowFrameImpl* impl = static_cast<WindowFrameImpl*>( window->frame() );
            ::SetCapture( impl->hwnd() );
            //std::clog << "SET CAPTURE HWND: " << impl->hwnd() << std::endl;
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////
// Element
///////////////////////////////////////////////////////////////////////

Element* ScreenImpl::onHitTest(const gfx::Point& p, HitTestHint hint)
{
    POINT pnt;
    pnt.x = std::round(p.x() * scaleFactor());
    pnt.y = std::round(p.y() * scaleFactor());

    HWND hwnd = WindowFromPoint(pnt);
    Window* win = Application::instance().impl()->findWindow(hwnd);
    if( ! win )
        return 0;

    WindowFrameImpl* frame = static_cast<WindowFrameImpl*>( win->frame() );
    gfx::Point pos = toFrame(*frame, p);
    
    return win->hitTest(pos, hint);
}


gfx::Point ScreenImpl::onToParent(const gfx::Point& pos) const
{
    return pos + position();
}


gfx::Point ScreenImpl::onFromParent(const gfx::Point& pos) const
{
    return pos - position();
}


bool ScreenImpl::onProcessEvent(const Pt::Event& ev)
{
    return Base::onProcessEvent(ev);
}


void ScreenImpl::onRequestRepaint(const gfx::Rect& rect)
{
    if(_parent)
        _parent->repaint(rect);
}

///////////////////////////////////////////////////////////////////////
// WindowManager
///////////////////////////////////////////////////////////////////////

WindowFrameBase* ScreenImpl::onAttach(Window& w)
{
    WindowFrameImpl* frame = new WindowFrameImpl(*this, w);
    

    _windows.push_back(&w);

    w.updateScaling();

    return frame;
}


void ScreenImpl::onDetach(WindowFrameBase& frame)
{    
    Window& w = frame.window();

    const Element* pointed = Application::instance().pointed();

    if (pointed == &w || pointed == &frame)
        Application::instance().setPointed(0);

    std::vector<Window*>::iterator it;
    it = std::remove(_windows.begin(), _windows.end(), &w);
    _windows.erase(it, _windows.end());
}


void ScreenImpl::onInit(WindowFrameBase& frame)
{
    
}


void ScreenImpl::onRelease(WindowFrameBase& w)
{
}

///////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////


void ScreenImpl::onProcessPaintEvent(const PaintEvent& ev)
{
    const gfx::Rect& screenRect = ev.rect();

    Base::onProcessPaintEvent(ev);

    std::vector<Window*>::iterator it;

    for(it = _windows.begin(); it != _windows.end(); ++it)
    {
        Window* window = *it;
        WindowFrameImpl* frame = static_cast<WindowFrameImpl*>( window->frame() );

        gfx::Point winPos = toFrame( *frame, screenRect.topLeft() );
        gfx::Rect winRect( winPos, screenRect.size() );

        winRect = winRect.intersect( gfx::Rect( window->size() ) );

        winRect.scale(_scaleFactor);

        frame->paint(winRect);
    }
}


void ScreenImpl::onPaintEvent(const PaintEvent& ev)
{    
    Base::onPaintEvent(ev);

    const gfx::Rect& rect = ev.rect();
    onPaint(rect);
}


void ScreenImpl::onPaint(const gfx::Rect& rect)
{
}


void ScreenImpl::onProcessEnableEvent(const EnableEvent& ev)
{
    Base::onProcessEnableEvent(ev);

    for( size_t i = 0; i < _windows.size(); ++i)
    {
        Window* w = _windows[i];
        WindowFrameImpl* frame = static_cast<WindowFrameImpl*>( w->frame() );

        frame->onEnable(*w, ev.enabled() );
    }
}


void ScreenImpl::onEnableEvent(const EnableEvent& ev)
{    
    Base::onEnableEvent(ev);
}


void ScreenImpl::onEnable(bool e)
{
    Base::onEnable(e);
}


bool ScreenImpl::onProcessMouseEvent(const MouseEvent& ev)
{
    Element* visual = Application::inst().findVisual(ev.id());

    if( visual && visual != this && visual != _parent )
    {
        return visual->processEvent(ev);
    }

    POINT screenPos;
    screenPos.x = std::round(ev.position().x() * scaleFactor());
    screenPos.y = std::round(ev.position().y() * scaleFactor());

    Window* window = 0;
        
    for(HWND h = GetTopWindow(NULL); h != NULL; h = GetWindow(h, GW_HWNDNEXT))
    {
        RECT rect;
        GetClientRect(h, &rect);

        POINT pos = screenPos;
        ScreenToClient(h, &pos);
            
        if( PtInRect(&rect, pos) )
        {
            window = Application::instance().impl()->findWindow(h);
            if(window)
                break;
        }
    }

    if(window)
        return window->processEvent(ev);

    return Base::onProcessMouseEvent(ev);
}


bool ScreenImpl::onMouseEvent(const MouseEvent& ev)
{ 
    // TODO: possibly pass on to application
    return Base::onMouseEvent(ev);
}


bool ScreenImpl::onProcessTouchEvent(const TouchEvent& ev)
{
    return ev.element()->processEvent(ev);
}


bool ScreenImpl::onTouchEvent(const TouchEvent& ev)
{ 
    // TODO: possibly pass on to application
    return Base::onTouchEvent(ev);
}


bool ScreenImpl::onProcessScrollEvent(const ScrollEvent& ev)
{
    return ev.element()->processEvent(ev);
}


bool ScreenImpl::onScrollEvent(const ScrollEvent& ev)
{ 
    // TODO: possibly pass on to application
    return Base::onScrollEvent(ev);
}


bool ScreenImpl::onProcessKeyEvent(const KeyEvent& ev)
{
    Element* visual = ev.element();
    if(visual && visual != this && visual != _parent)
    {
        return visual->processEvent(ev);
    }

    HWND h = GetActiveWindow();

    Window* window = Application::instance().impl()->findWindow(h);

    if(window)
        return window->processEvent(ev);

    return false;
}


bool ScreenImpl::onKeyEvent(const KeyEvent& ev)
{ 
    // TODO: possibly pass on to application
    return Base::onKeyEvent(ev); 
}

}} }

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
#include "WindowFrame.h"
#include <wildcat/gui/window/WindowManager.h>
#include <wildcat/gui/window/MdiArea.h>
#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/WindowStateEvent.h>
#include <wildcat/gui/gfx/Graphics.h>
#include <iostream>

namespace wildcat {
namespace gui {
namespace window {

 WindowManager::WindowManager()
: _parent(0)
, _activeWindow(0)
, _topMostWindow(0)
, _borderWidth(4)
, _titleHeight(20)
, _activeColor( gfx::Color::fromRgb8(100, 128, 205) )
, _inactiveColor( gfx::Color::fromRgb8(173, 178, 191) )
, _textColor( gfx::Color::fromRgb8(255, 255, 255) )
, _inactiveTextColor( gfx::Color::fromRgb8(50, 50, 50) )
{
     setName("WindowManager");
}

 WindowManager::~WindowManager()
{
    while( ! _windowList.empty() )
    {
        _windowList.front()->unparent();
    }
}

void WindowManager::setParent(MdiArea* shell)
{
    _parent = shell;

    onSetParent(_parent);
}

WindowBase* WindowManager::parentWindow()
{
    if(_parent == nullptr)
        return nullptr;

    return _parent->window();
}

void WindowManager::setGraphics(gfx::Graphics* gf, const gfx::Point& pos)
{
    if (!gf)
    {
        _graphics.detach();
    }
    else
    {
        gfx::Rect surfaceRect(pos, size());
        _graphics.attach(*gf, surfaceRect);
    }
}

gfx::Graphics& WindowManager::graphics()
{
    return _graphics;
}

const gfx::Graphics& WindowManager::graphics() const
{
    return _graphics;
}

Window* WindowManager::activeWindow()
{
    return _activeWindow;
}

void WindowManager::onRequestActivate(bool active)
{
    if(_parent)
        _parent->onActivate(*this, active);
}

const std::vector<Window*>& WindowManager::windows() const
{
    return _windowList;
}

gfx::Point WindowManager::toFrame(const WindowFrame& w, const gfx::Point& pos) const
{
     return pos - w.position() - position();
}

gfx::Point WindowManager::fromFrame(const WindowFrame& w, const gfx::Point& pos) const
{
     return position() + pos + w.position();
}

///////////////////////////////////////////////////////////////////////
// Element
///////////////////////////////////////////////////////////////////////

gfx::Point WindowManager::onToParent(const gfx::Point& pos) const
{
    return pos + position();
}

gfx::Point WindowManager::onFromParent(const gfx::Point& pos) const
{
    return pos - position();
}

Element* WindowManager::onHitTest(const gfx::Point& p, HitTestHint hint)
{
    if( ! bounds().contains(p) )
        return 0;

    std::vector<Window*>::const_reverse_iterator rit;
    for(rit = _windowList.rbegin() ; rit != _windowList.rend(); ++rit )
    {
        Window* window = *rit;
        WindowFrame* frame = static_cast<WindowFrame*>( window->frame() );

        if( ! window->isVisible() )
            continue;

        gfx::Point pos = toFrame(*frame, p);
        Element* hit = frame->hitTest(pos, hint);
        if(hit)
            return hit;
    }

    return 0;
}

void WindowManager::onRequestCapture(bool capture)
{
    WindowManagerBase::onRequestCapture(capture);
}

WindowFrameBase* WindowManager::onAttach(Window& w)
{
    WindowFrame* frame = new WindowFrame(*this, w);
    
    if(_topMostWindow)
        _windowList.insert( --_windowList.end(), &w );
    else
        _windowList.push_back(&w);

    w.updateScaling();
    return frame;
}

void WindowManager::onDetach(WindowFrameBase& frame)
{    
    Window& w = frame.window();

    std::vector<Window*>::iterator wit;
    for(wit = _windowList.begin(); wit != _windowList.end(); ++wit)
    {
        Window* window = *wit;

        if(window == &w)
        {
            const Element* pointed = Application::instance().pointed();

            if(  pointed== &w || pointed == &frame)
                Application::instance().setPointed(0);

            if(_activeWindow && _activeWindow == &w)
                _activeWindow  = 0;

            if(_topMostWindow && _topMostWindow == &w)
                _topMostWindow = 0;

            _windowList.erase(wit);
            break;
        }
    }
}

void WindowManager::onInit(WindowFrameBase& frame)
{
}

void WindowManager::onRelease(WindowFrameBase& frame)
{
    if( frame.isVisible() )
    {
        gfx::Rect frameRect( frame.position(), frame.size() );
        repaint(frameRect);
    }
}

void WindowManager::onShow(WindowFrame& frame, bool visible)
{
    ShowEvent windowEvent( frame, visible );
    Application::instance().loop().commitEvent(windowEvent);
}

void WindowManager::onActivate(WindowFrame& frame, bool active)
{
    Window& w = frame.window();

    if(active)
    {
        //
        // deactivate other active window
        //

        if(_activeWindow && _activeWindow != &w)
            _activeWindow->activate(false);

        //
        // raise to top of window stack
        //
        std::vector<Window*>::iterator it = std::find(_windowList.begin(), _windowList.end(), &w);

        _windowList.erase(it);

        if(_topMostWindow && _topMostWindow != &w)
        {
            _windowList.insert(--_windowList.end(), &w);
        }
        else
        {
            _windowList.push_back(&w);
        }

        _activeWindow = &w;
    }
    else
    {
        if(_activeWindow == &w)
            _activeWindow = 0;
    }

    ActivateEvent aev(frame, active);
    Application::instance().commitEvent(aev);
    
    activate(active);
}

void WindowManager::onEnable(WindowFrame& frame, bool enable)
{
    EnableEvent eev(frame, enable);
    Application::instance().commitEvent(eev);
}

void WindowManager::onSetAbove(WindowFrame& frame, bool above)
{
    Window& w = frame.window();

    if(above)
    {
        if(_topMostWindow && _topMostWindow != &w)
            _topMostWindow->setAbove(false);

        // move top most to the back
        std::vector<Window*>::iterator it = std::find(_windowList.begin(), 
                                                      _windowList.end(), &w);
        if( it != _windowList.end() )
            _windowList.erase(it);
        
        _windowList.push_back(&w);

        _topMostWindow = &w;
    }
    else if(_topMostWindow == &w)
    {
        _topMostWindow = 0;
    }
}

void WindowManager::onSetSizeLimits(WindowFrame& w, const gfx::Size& minSize,  const gfx::Size& maxSize)
{
}

void WindowManager::onClose(WindowFrame& wf)
{
    CloseEvent ev(wf);
    Application::instance().loop().commitEvent(ev);
    Application::inst().nextEvent();
}

void WindowManager::onRequestRepaint(const gfx::Rect& rect)
{
    if(_parent)
        _parent->onRepaint(*this, rect);
}

void WindowManager::onProcessPaintEvent(const PaintEvent& ev)
{
    const gfx::Rect& rect = ev.rect();

    if( rect.isNull() )
        return;

    WindowManagerBase::onProcessPaintEvent(ev);
        
    // paint child windows
    std::vector<Window*>::iterator wit;

    for(wit = _windowList.begin(); wit != _windowList.end(); ++wit)
    {
        Window* window = *wit;

        if (!window->isVisible())
            continue;

        WindowFrame* frame = static_cast<WindowFrame*>( window->frame() );

        gfx::Rect fr( frame->position(), frame->size() );
        gfx::Rect frameRect = rect.intersect(fr);

        if(frameRect.isNull())
            continue;   

        PaintEvent pev( *frame, frameRect);
        frame->processEvent(pev);
    }
}

void WindowManager::onProcessEnableEvent(const EnableEvent& ev)
{
    WindowManagerBase::onProcessEnableEvent(ev);
    
    std::vector<Window*>::iterator wit;
    for(wit = _windowList.begin(); wit != _windowList.end(); ++wit)
    {
        Window* window = *wit;

        EnableEvent eev( *window, ev.enabled() );
        Application::instance().loop().commitEvent(eev);
    }
}

void WindowManager::onMove(WindowFrame& frame, const gfx::Point& pos)
{
    gfx::Point aligedPos = pos;

    if( aligedPos.x() < 0)
        return;

    if (aligedPos.y() < 0)
        return;

    if (aligedPos.x() > size().width() - 20)
        return;

    if (aligedPos.y() > size().height() -10 )
        return;

    MoveEvent mev(frame, aligedPos);
    Application::instance().commitEvent(mev);
}

void WindowManager::onResize(WindowFrame& frame, const gfx::Size& to)
{
    ResizeEvent rev(frame, to);
    Application::instance().commitEvent(rev);
}

void WindowManager::onUpdateScaling()
{
    WindowManagerBase::onUpdateScaling();   

    for(size_t i = 0; i < _windowList.size(); ++i)
        _windowList[i]->updateScaling();
}

void WindowManager::onResizeEvent(const ResizeEvent& ev)
{
    WindowManagerBase::onResizeEvent(ev);

    std::vector<Window*>::iterator wit;
    for(wit = _windowList.begin(); wit != _windowList.end(); ++wit)
    {
        Window* window = *wit;

        if(window->state() == WindowState::Maximized)
        {
            window->setState(WindowState::Maximized);
        }
    }

    _graphics.resize(ev.size());
}

void WindowManager::onMoveEvent(const MoveEvent& ev)
{
    MoveEvent evw(*this,gfx::Point(0,0));

    WindowManagerBase::onMoveEvent(evw);

    _graphics.move(ev.position());
}

bool WindowManager::processMouseEvent(const MouseEvent& ev)
{    
    const gfx::Point pos = fromGlobal( ev.position() );
    
    WindowFrameBase* hitFrame = 0;

    std::vector<Window*>::const_reverse_iterator rit;

    for(rit = _windowList.rbegin() ; rit != _windowList.rend(); ++rit )
    {
        Window* window = *rit;
        WindowFrame* frame = static_cast<WindowFrame*>( window->frame() );

        gfx::Point p = toFrame(*frame, pos);
        
        Element* hit = frame->hitTest(p, HitTestHint::HitAll);
        if(hit)
        {         
            hitFrame = frame;
            break;
        }
    }

    if( ev.isPress() )
    {
        if( ! hitFrame && _activeWindow )
        {
            _activeWindow->activate(false);
        }
        
        if( hitFrame && ! hitFrame->window().isActive() )
        {
            hitFrame->window().activate();
        }
    }

    // propagate event to window frame
    if(hitFrame)
    {
        hitFrame->processEvent(ev);
        return true;
    }

    //setCursor(Cursor::defaultCursor());
    return false;
}

bool WindowManager::processTouchEvent(const TouchEvent& ev)
{
    //if( ! acceptsInput() )
    //    return;

    //
    // hit test
    //
    gfx::Point pos = fromGlobal( ev.position() );

    WindowFrameBase* hitFrame = 0;

    std::vector<Window*>::const_reverse_iterator rit;
    for(rit = _windowList.rbegin() ; rit != _windowList.rend(); ++rit )
    {
        Window* window = *rit;
        WindowFrame* frame = static_cast<WindowFrame*>( window->frame() );

        gfx::Point p = toFrame(*frame, pos);
        Element* hit = frame->hitTest(p, HitTestHint::HitAll);
        if(hit)
        {
            hitFrame = frame;
            break;
        }
    }

    //
    // window activation
    //
    if( ev.isPress() )
    {
        if( ! hitFrame && _activeWindow )
        {
            _activeWindow->activate(false);
        }
        
        if( hitFrame && ! hitFrame->window().isActive() )
        {
            hitFrame->window().activate();
        }
    }

    //
    // propagate event to window frame
    //
    if(hitFrame)
    {
        hitFrame->processEvent(ev);
        return true;
    }

    //setCursor(Cursor::defaultCursor());

    return false;
}

bool WindowManager::onProcessMouseEvent(const MouseEvent& ev)
{
    return processMouseEvent(ev);
}

bool WindowManager::onProcessTouchEvent(const TouchEvent& ev)
{
    return processTouchEvent(ev);
}

void WindowManager::onProcessEnterEvent(const EnterEvent& ev)
{
    WindowManagerBase::onProcessEnterEvent(ev);
}

void WindowManager::onProcessLeaveEvent(const LeaveEvent& ev)
{
    WindowManagerBase::onProcessLeaveEvent(ev);
}

bool WindowManager::onProcessScrollEvent(const ScrollEvent& ev)
{
    //if( ! acceptsInput() )
    //    return;

    if( _activeWindow )
        return _activeWindow->processEvent(ev);
}

bool WindowManager::onProcessKeyEvent(const KeyEvent& ev)
{
    //if( ! acceptsInput() )
    //    return;


    if(_activeWindow)
    {
        KeyEvent kev = ev;
        kev.setElement(_activeWindow);
        return _activeWindow->processEvent(ev);
    }
}

}}}

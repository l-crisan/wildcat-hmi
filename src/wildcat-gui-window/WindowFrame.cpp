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
#include <wildcat/gui/window/WindowManagerBase.h>
#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/WindowManager.h>
#include <wildcat/gui/window/WindowStateEvent.h>
#include <wildcat/gui/window/ResizeEvent.h>
#include <wildcat/gui/window/MoveEvent.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Color.h>

namespace wildcat {
namespace gui {
namespace window {

//ToDo:
// System menu
// MDI arrage window 


class ClipContext
{
    public:
        ClipContext(gfx::Graphics& gf, const gfx::Rect& clip)
        : _gf(gf)
        {
            _gf.setClip(clip);
        }

        ~ClipContext()
        {
            _gf.resetClip();
        }   

    private:
        gfx::Graphics& _gf;
};


WindowFrame::WindowFrame(WindowManager& shell, Window& window)
: WindowFrameBase(shell, window)
, _wm(&shell)
, _window(&window)
, _borderWidth( shell.borderWidth() )
, _titleHeight( shell.titleHeight() )
, _state(WindowState::Normal)
, _isClient(false)
, _isMoving(false)
, _isLeftResizing(false)
, _isRightResizing(false)
, _isTopResizing(false)
, _isBottomResizing(false)
, _currentFrameItem(OnNone)
{

    setName("WindowFrame");
    Base::onSetParent(&shell);    

    switch( window.frameType() )
    {
        case WindowType::Borderless:
            setFrame(0, 0);
        break;
        
        default:

        case WindowType::Default:
        case WindowType::Fixed:
            setFrame(_borderWidth, _titleHeight);
        break;
    }

    _maximizeButton.setParent(*this);
    _maximizeButton.clicked() += Pt::slot(*this, &WindowFrame::onMaximize);
    _buttons.push_back(&_maximizeButton);

    _minimizeButton.setParent(*this);
    _minimizeButton.clicked() += Pt::slot(*this, &WindowFrame::onMinimize);
    _buttons.push_back(&_minimizeButton);

    _closeButton.setParent(*this);
    _closeButton.clicked() += Pt::slot(*this, &WindowFrame::onCloseButton);
    _buttons.push_back(&_closeButton);

    _menuButton.setParent(*this);
    _menuButton.clicked() += Pt::slot(*this, &WindowFrame::onMenu);
    _buttons.push_back(&_menuButton);
}

WindowFrame::~WindowFrame()
{
}

const SystemColors& WindowFrame::colors() const
{
    return window().colors();  
}

void WindowFrame::onInit(Window& w)
{
    _menuButton.setRm(&w.resourceMng());
}

void WindowFrame::onRelease(Window& w)
{
    
}

void WindowFrame::onSetTitle(Window& w, const Pt::String& text)
{
    invalidate();
}

void WindowFrame::onSetIcon(Window& w, uint32_t icon)
{
   _menuButton.setIcon(icon);

   invalidate();
}

void WindowFrame::onShowSystemMenu(Window& w, bool v)
{
    _menuButton.setVisible(v);
    invalidate();
}

void WindowFrame::onShowCloseButton(Window& w, bool v)
{
    _closeButton.setVisible(v);
    invalidate();
}

void WindowFrame::onShowMinimizeButton(Window& w, bool v)
{
    _minimizeButton.setVisible(v);
    invalidate();
}

void WindowFrame::onShowMaximizeButton(Window& w, bool v)
{
    _maximizeButton.setVisible(v);
    invalidate();
}


void WindowFrame::onInvalidate()
{ 
    _menuButton.invalidate();   
    _closeButton.invalidate();
    _minimizeButton.invalidate();
    _maximizeButton.invalidate();   
    WindowFrameBase::onInvalidate();
}

void WindowFrame::onSetState(Window& w, const WindowState& state)
{
    Window::State oldState = _state;

    if(oldState == WindowState::Normal)
        setRestore( _window->position(), _window->size() );

    if(state == WindowState::Maximized)
    {
        gfx::Size maxSize = _wm->size();
        maxSize.subWidth( 2 * _borderWidth );
        maxSize.subHeight( (2 * _borderWidth) + _titleHeight );

        onMove( *_window, gfx::Point(0, 0) );
        onResize(*_window, maxSize);
    }
    else if(state == WindowState::Minimized)
    {
        if(oldState == WindowState::Maximized)
        {
            onMove( *_window, restorePosition() );
        }

        gfx::Size minSize(restoreSize().width(), 0);
        onResize(*_window, minSize);
    }
    else if(state == WindowState::Normal)
    {
        if(oldState != WindowState::Normal)
        {
            onMove( *_window, restorePosition() );
            onResize(*_window, restoreSize());
        }
    }

    WindowStateEvent wse(*this, state);
    Application::instance().loop().commitEvent(wse);
}

void WindowFrame::onProcessWindowStateEvent(const WindowStateEvent& ev)
{
    WindowStateEvent wse( *_window, ev.state() );
    Application::instance().processEvent(wse);

    Base::onProcessWindowStateEvent(ev);
}

void WindowFrame::onWindowStateEvent(const WindowStateEvent& ev)
{
    _state = ev.state();
}

void WindowFrame::onSetAbove(Window& w, bool above)
{
    _wm->onSetAbove(*this, above);
}

void WindowFrame::onSetSizeLimits(Window& w, const gfx::Size& minSize,  const gfx::Size& maxSize)
{
    _wm->onSetSizeLimits(*this, minSize, maxSize);
}

const gfx::Point& WindowFrame::restorePosition() const
{
    return _restorePos;
}

const gfx::Size& WindowFrame::restoreSize() const
{
    return _restoreSize;
}

void WindowFrame::setRestore(const gfx::Point& pos, const gfx::Size& size)
{
    _restorePos = pos;
    _restoreSize = size;
}

const gfx::Rect& WindowFrame::clientRect() const
{
    return _clientRect;
}

const gfx::Rect& WindowFrame::frameRect() const
{
    return _frameRect;
}

void WindowFrame::setFrame(int bw, int th)
{
    _borderWidth = bw;
    _titleHeight = th;

    gfx::Point clientBoundsPos(_borderWidth, _borderWidth + _titleHeight);
    _clientBounds.setOrigin(clientBoundsPos);
}

void WindowFrame::onMenu()
{
}

void WindowFrame::onMinimize()
{
    if(_state == WindowState::Minimized)
        _window->setState(WindowState::Normal);
    else
        _window->setState(WindowState::Minimized);
}

void WindowFrame::onMaximize()
{
    if(_state == WindowState::Maximized)
        _window->setState(WindowState::Normal);
    else
        _window->setState(WindowState::Maximized);
}

void WindowFrame::onCloseButton()
{
    CloseEvent ev(*_window);    
    Application::instance().commitEvent(ev);    
}

void WindowFrame::onClose(Window& w)
{
    
    _wm->onClose(*this);    
}

void WindowFrame::onProcessCloseEvent(const CloseEvent& ev)
{
    onCloseEvent(ev);

    CloseEvent cev(*_window);
    _window->processEvent(cev);
}

void WindowFrame::onCloseEvent(const CloseEvent& ev)
{
}

void WindowFrame::onLayout()
{
    const int buttonWidth = _titleHeight - _borderWidth;

    gfx::Point menuPos = _frameRect.topLeft();
    menuPos += gfx::Point( _borderWidth+2, _borderWidth+2); // Thin frame 2
       
    _menuButton.moveEvent( MoveEvent(*_window, menuPos ) );
    _menuButton.resizeEvent( ResizeEvent(*_window, gfx::Size(buttonWidth, buttonWidth) ) );
    
    int buttonX = _frameRect.x() + _frameRect.width() - (_borderWidth*2 + buttonWidth);
    int buttonY = _frameRect.y() + _borderWidth + 2;

    if( _closeButton.isVisible())
    {
        _closeButton.moveEvent( MoveEvent(*_window, gfx::Point(buttonX, buttonY) ) );
        _closeButton.resizeEvent( ResizeEvent(*_window, gfx::Size(buttonWidth, buttonWidth) ) );

        buttonX -= _borderWidth + buttonWidth;
    }

    if( _maximizeButton.isVisible())
    {
        _maximizeButton.moveEvent( MoveEvent(*_window, gfx::Point(buttonX, buttonY) ) );
        _maximizeButton.resizeEvent( ResizeEvent(*_window, gfx::Size(buttonWidth, buttonWidth) ) );

        buttonX -= _borderWidth + buttonWidth;
    }

    if( _minimizeButton.isVisible())
    {
        _minimizeButton.moveEvent( MoveEvent(*_window, gfx::Point(buttonX, buttonY) ) );
        _minimizeButton.resizeEvent( ResizeEvent(*_window, gfx::Size(buttonWidth, buttonWidth) ) );
    }
}

gfx::Point WindowFrame::onToWindow(const Window& w, const gfx::Point& pos) const
{
    if(w.frameType() == Window::Type::Borderless)
        return pos;

    const int offX = _borderWidth;
    const int offY = _borderWidth + _titleHeight;

    return pos - gfx::Point(offX, offY);
}

gfx::Point WindowFrame::onFromWindow(const Window& w, const gfx::Point& pos) const
{
    if (w.frameType() == Window::Type::Borderless)
        return pos;

    const int offX = _borderWidth;
    const int offY = _borderWidth + _titleHeight;

    return pos + gfx::Point(offX, offY);
}

Element* WindowFrame::onHitTest(const gfx::Point& pos, HitTestHint hint)
{    
    if( ! bounds().contains(pos) || ! isVisible() )
        return nullptr;

    if(_window)
    {
        gfx::Point p = onToWindow(*_window, pos);

        Element* hit = _window->hitTest(p, hint);

        if(hit)
            return hit;
    }

    return this;
}

gfx::Point WindowFrame::onToParent(const gfx::Point& pos) const
{ 
    return _wm->fromFrame(*this, pos);
}
        
gfx::Point WindowFrame::onFromParent(const gfx::Point& pos) const
{
    return _wm->toFrame(*this, pos);
}

bool WindowFrame::onProcessEvent(const Pt::Event& ev)
{
    return Base::onProcessEvent(ev);
}


void WindowFrame::onShow(Window& w, bool visible)
{
    _wm->onShow(*this, visible);
}

void WindowFrame::onProcessShowEvent(const ShowEvent& ev)
{
    Base::onProcessShowEvent(ev);

    ShowEvent rev( *_window, ev.visible() );
    _window->processEvent(rev);
}

void WindowFrame::onShowEvent(const ShowEvent& ev)
{
    Base::onShowEvent(ev);

    repaint(_frameBounds);
}

void WindowFrame::onEnable(Window& w, bool enable)
{
    _wm->onEnable(*this, enable);
}

void WindowFrame::onProcessEnableEvent(const EnableEvent& ev)
{
    Base::onProcessEnableEvent(ev);

    EnableEvent eev( *_window, ev.enabled() );
    _window->processEvent(eev);
}

void WindowFrame::onEnableEvent(const EnableEvent& ev)
{    
    Base::onEnableEvent(ev);

    repaint(_frameBounds);
}

void WindowFrame::onActivate(Window& w, bool active)
{
    _wm->onActivate(*this, active);
}

void WindowFrame::onProcessActivateEvent(const ActivateEvent& ev)
{
    Base::onProcessActivateEvent(ev);

    ActivateEvent aev( *_window, ev.isActive() );
    _window->processEvent(aev);
}

void WindowFrame::onActivateEvent(const ActivateEvent& ev)
{
    Base::onActivateEvent(ev);

    repaint(_frameBounds);
}

void WindowFrame::onRequestMove(const gfx::Point& posIn)
{    
    gfx::Point pos = posIn;


    _frameRect.setOrigin(pos);

    gfx::Point clientPos = pos;

    if (window().frameType() != Window::Type::Borderless)
    {
        clientPos.addX(_borderWidth);
        clientPos.addY(_borderWidth + _titleHeight);
    }

    _clientRect.setOrigin(clientPos);
}

void WindowFrame::onMove(Window& w, const gfx::Point& posIn)
{    
    gfx::Point pos = posIn;

    // window position is physical if not scaled, frame coordinates are logical
    if( ! w.scalePosition() )
        pos.scale( 1.0 / Application::inst().screen().scaleFactor() );

    _frameRect.setOrigin(pos);

    gfx::Point clientPos = pos;

    if(w.frameType() != Window::Type::Borderless)
    {
        clientPos.addX(_borderWidth);
        clientPos.addY(_borderWidth + _titleHeight);
    }

    _clientRect.setOrigin(clientPos);

    _wm->onMove(*this, pos);        
}

void WindowFrame::onProcessMoveEvent(const MoveEvent& ev)
{
    Base::onProcessMoveEvent(ev);
}

void WindowFrame::onMoveEvent(const MoveEvent& ev)
{

    gfx::Rect oldRect = bounds();
    oldRect.move(-1, -1);
    oldRect.expand(2, 2);
    repaint(oldRect);

    gfx::Point pos = ev.position();

    gfx::Point delta = pos - position();
    _lastPointer = _lastPointer - delta;

    _frameRect.setOrigin(pos);

    gfx::Point clientPos = pos;

    if (_window->frameType() != Window::Type::Borderless)
    {
        clientPos.addX(_borderWidth);
        clientPos.addY(_borderWidth + _titleHeight);
    }

    _clientRect.setOrigin(clientPos);        


    Base::onMoveEvent(MoveEvent(*this, pos));
    onLayout();
    repaint();
}

void WindowFrame::onRequestResize(const gfx::Size& size)
{
    _clientRect.setSize(size);

    _clientBounds.setSize(size);

    gfx::Size frameSize = size;

    frameSize.addWidth( 2 * _borderWidth);
    frameSize.addHeight(2 * _borderWidth);
    frameSize.addHeight(_titleHeight);

    _frameRect.setSize(frameSize);
    _frameBounds.setSize(frameSize);
}

void WindowFrame::onResize(Window& w, const gfx::Size& s)
{
    gfx::Size alignedSize = s;

    if( alignedSize.width() > w.maximumSize().width() )
        alignedSize.setWidth( w.maximumSize().width() );

    if( alignedSize.height() > w.maximumSize().height() )
        alignedSize.setHeight( w.maximumSize().height() );

    if( alignedSize.width() < w.minimumSize().width() )
        alignedSize.setWidth( w.minimumSize().width() );

    if( alignedSize.height() < w.minimumSize().height() )
        alignedSize.setHeight( w.minimumSize().height() );

    _clientRect.setSize(alignedSize);
    _clientBounds.setSize(alignedSize);

    gfx::Size frameSize = alignedSize;

    if (w.frameType() != Window::Type::Borderless)
    {
        frameSize.addWidth(2 * _borderWidth);
        frameSize.addHeight(2 * _borderWidth);
        frameSize.addHeight(_titleHeight);
    }

    _frameRect.setSize(frameSize);

    _frameBounds.setSize(frameSize);


    _wm->onResize(*this, frameSize);
}

void WindowFrame::onProcessResizeEvent(const ResizeEvent& ev)
{   
    gfx::Size clientSize = ev.size();

    clientSize.subWidth(2 * _borderWidth);
    clientSize.subHeight(2 * _borderWidth);
    clientSize.subHeight(_titleHeight);
    
    _clientRect.setSize(clientSize);
    _clientBounds.setSize(clientSize);

    _frameRect.setSize( ev.size() );
    _frameBounds.setSize( ev.size() );

    Base::onProcessResizeEvent(ev);

    ResizeEvent rev(*_window, clientSize);
    _window->processEvent(rev);
}

void WindowFrame::onResizeEvent(const ResizeEvent& ev)
{
    repaint();

    Base::onResizeEvent(ev);

    onLayout();

    repaint();
}

bool WindowFrame::onEnterEvent( const EnterEvent& ev)
{
    _currentFrameItem = OnNone;    
    
    return Base::onEnterEvent(ev);
}

bool WindowFrame::onLeaveEvent(const LeaveEvent& ev)
{
    _currentFrameItem = OnNone;

    const bool isAction = _isLeftResizing || _isRightResizing || _isTopResizing || _isBottomResizing || _isMoving;

    if(!isAction)
        setCapture(false);  
    
    return Base::onLeaveEvent(ev);;
}

bool WindowFrame::onProcessMouseEvent(const MouseEvent& ev)
{
    if( ! window().acceptsInput() )
        return false;

    const bool r = Element::onProcessMouseEvent(ev);
    
    // hit test   
    const gfx::Point pos = fromGlobal( ev.position() );

    Window* window = checkWindow(pos);

    if(window)
        return window->processEvent(ev);
    
        
    return r;
}

gfx::Point WindowFrame::onClientOffset() const
{
    return gfx::Point(_borderWidth, _borderWidth + _titleHeight);
}

bool WindowFrame::onProcessTouchEvent(const TouchEvent& tev)
{
    bool r =  Element::onProcessTouchEvent(tev);

    gfx::Point pos = fromGlobal( tev.position() );

    Window* window = checkWindow(pos);

    if(window)
        return window->processEvent(tev);
        
    return r;
}

bool WindowFrame::onMouseEvent(const MouseEvent& mev)
{
    gfx::Point pos =  mev.position();
    
    FrameButton* button = checkButton(pos + _frameRect.topLeft());

    if(button)
    {
        setCurrentFrameItem(OnButton);
        button->setHightlight(true);
        button->mouseEvent(mev);
    }
    else if( isTitle(pos) )
    {
        setCurrentFrameItem(OnTitle);

        gfx::Point pp = pos;

        checkMove(pp, mev.isPressed(), mev.isPress() );
    }
    else if (_window->frameType() != Window::Type::Fixed)        
    {
        bool onLeftBorder   = isLeftBorder(pos);
        bool onRightBorder  = isRightBorder(pos);
        bool onTopBorder    = isTopBorder(pos);
        bool onBottomBorder = isBottomBorder(pos);

        if(onLeftBorder || onRightBorder || onTopBorder || onBottomBorder)
        {
            if(onTopBorder && onLeftBorder) 
                setCurrentFrameItem(OnFrameTopLeft);
            else if(onTopBorder && onRightBorder) 
                setCurrentFrameItem(OnFrameTopRight);
            else if(onTopBorder)
                setCurrentFrameItem(OnFrameTop);
            else if(onBottomBorder && onLeftBorder) 
                setCurrentFrameItem(OnFrameBottomLeft);
            else if(onBottomBorder && onRightBorder) 
                setCurrentFrameItem(OnFrameBottomRight);
            else if(onBottomBorder)
                setCurrentFrameItem(OnFrameBottom);
            else if(onLeftBorder)
                setCurrentFrameItem(OnFrameLeft);
            else if(onRightBorder)
                setCurrentFrameItem(OnFrameRight);

            checkResize(pos, mev.isPressed(), mev.isPress());
        }
        else
        {
            setCurrentFrameItem(OnNone);
        }
    }


    _lastPointer = pos;
    
    return true;
}

bool WindowFrame::onTouchEvent(const TouchEvent& tev)
{
    gfx::Point pos = tev.position();

    FrameButton* button = checkButton(pos);
    if(button)
    {
        setCurrentFrameItem(OnButton);
        button->touchEvent(tev);
    }
    else if( isTitle(pos) )
    {
        setCurrentFrameItem(OnTitle);
        checkMove(pos, tev.isPressed(), tev.isPress() );
    }
    else
    {
        const bool onLeftBorder = isLeftBorder(pos);
        const bool onRightBorder = isRightBorder(pos);
        const bool onTopBorder = isTopBorder(pos);
        const bool onBottomBorder = isBottomBorder(pos);

        if(onLeftBorder || onRightBorder || onTopBorder || onBottomBorder)
        {
            if(onTopBorder && onLeftBorder) 
                setCurrentFrameItem(OnFrameTopLeft);
            else if(onTopBorder && onRightBorder) 
                setCurrentFrameItem(OnFrameTopRight);
            else if(onTopBorder)
                setCurrentFrameItem(OnFrameTop);
            else if(onBottomBorder && onLeftBorder) 
                setCurrentFrameItem(OnFrameBottomLeft);
            else if(onBottomBorder && onRightBorder) 
                setCurrentFrameItem(OnFrameBottomRight);
            else if(onBottomBorder)
                setCurrentFrameItem(OnFrameBottom);
            else if(onLeftBorder)
                setCurrentFrameItem(OnFrameLeft);
            else if(onRightBorder)
                setCurrentFrameItem(OnFrameRight);

            checkResize(pos, tev.isPressed(), tev.isPress());
        }
        else
        {
            setCurrentFrameItem(OnNone);
        }
    }

    _lastPointer = pos;

    return true;
}

void WindowFrame::setCurrentFrameItem(FrameItem item)
{
    if(_currentFrameItem == item)
        return;

    switch(item)
    {
      case OnFrameLeft:
      case OnFrameRight:
          setCapture(true); 
          setCursor("SizeWECursor");
      break;

      case OnFrameTopLeft:
      case OnFrameBottomRight:
          setCapture(true);
          setCursor("SizeNWSECursor");
      break;

      case OnFrameTop:
      case OnFrameBottom:
          setCapture(true);
          setCursor("SizeNSCursor");
      break;

      case OnFrameBottomLeft:
      case OnFrameTopRight:
          setCapture(true);
          setCursor("SizeNESWCursor");
      break;

      case OnTitle:
          setCapture(true);
          setCursor("MoveCursor");
      break;

      case OnButton:
          setCapture(false);
          setCursor("ArrowCursor");
      break;

      default:
          setCapture(false);
          setCursor("ArrowCursor");
      break;

    }
    
    _currentFrameItem = item;
}

bool WindowFrame::isTitle(const gfx::Point& p) const
{
    if (_window->frameType() == Window::Type::Borderless)
        return false;

    bool isResizing = _isLeftResizing || _isRightResizing ||
                      _isTopResizing || _isBottomResizing;

    gfx::Point localPos = p - _frameBounds.topLeft();

    bool overTitle = localPos.x() >= _borderWidth &&
                     localPos.x() < _borderWidth + _clientBounds.width() &&
                     localPos.y() >= _borderWidth &&
                     localPos.y() < _borderWidth + _titleHeight;

    return (_isMoving || overTitle) && ! isResizing;
}

bool WindowFrame::isLeftBorder(const gfx::Point& p) const
{
    if (_window->frameType() == Window::Type::Borderless)
        return false;


    const gfx::Point localPos = p - _frameBounds.topLeft();

    const bool r =  localPos.x() >= 0 &&
              localPos.x() < _borderWidth &&
              localPos.y() >= 0 &&
              localPos.y() < _frameBounds.height();

    return _isLeftResizing || r;
}

bool WindowFrame::isRightBorder(const gfx::Point& p) const
{
    if (_window->frameType() == Window::Type::Borderless)
        return false;

    gfx::Point localPos = p - _frameBounds.topLeft();

    const bool r =   localPos.x() >= _borderWidth + _clientBounds.width() &&
               localPos.x() < 2 * _borderWidth + _clientBounds.width() &&
               localPos.y() >= 0 &&
               localPos.y() < _frameBounds.height();

    return _isRightResizing || r;
}

bool WindowFrame::isTopBorder(const gfx::Point& p) const
{
    if (_window->frameType() == Window::Type::Borderless)
        return false;

    const gfx::Point localPos = p - _frameBounds.topLeft();

    const bool r =   localPos.x() >= 0 &&
               localPos.x() < _frameBounds.width() &&
               localPos.y() >= 0 &&
               localPos.y() < _borderWidth;

    return _isTopResizing || r;
}

bool WindowFrame::isBottomBorder(const gfx::Point& p) const
{
    if (_window->frameType() == Window::Type::Borderless)
        return false;

    gfx::Point localPos = p - _frameBounds.topLeft();

    int minY = _clientBounds.height() + _borderWidth + _titleHeight;

    bool r =   localPos.x() >= 0 &&
               localPos.x() < _frameBounds.width() &&
               localPos.y() >= minY &&
               localPos.y() < _frameBounds.height();

    return _isBottomResizing || r;
}

Window* WindowFrame::checkWindow(const gfx::Point& pos)
{
    bool isResizing = _isLeftResizing || _isRightResizing ||
                      _isTopResizing || _isBottomResizing;

    if( ! _isMoving && ! isResizing )
    {
        if(_clientBounds.contains( pos ) )
        {
            if( ! _isClient )
            {
                _isClient = true;
                //EnterEvent eev(*_window);
                //_window->processEvent(eev);
            }

            return _window;
        }

        if(_isClient)
        {
            _isClient = false;
            //LeaveEvent lev( *_window );
            //_window->processEvent(lev);
        }
    }

    return 0;
}

FrameButton* WindowFrame::checkButton(const gfx::Point& pos)
{
    if (_window->frameType() == Window::Type::Borderless)
        return 0;

    bool isResizing = _isLeftResizing || _isRightResizing ||
                      _isTopResizing || _isBottomResizing;

    if( ! _isMoving && ! isResizing )
    {
        std::vector<FrameButton*>::iterator it;

        for(it = _buttons.begin(); it != _buttons.end(); ++it)
        {
            FrameButton* button = *it;
         
            if(button->geometry().contains(pos) )
            {
                button->enterEvent(EnterEvent(*_window));
                repaint();
                return button;
            }
            else if(button->geometry().contains(_lastPointer) )
            {
                button->leaveEvent( LeaveEvent(*_window) );
                repaint();
            }             
        }
    }

    return 0;
}

bool WindowFrame::checkMove(const gfx::Point& pos, bool isDrag, bool isPress)
{

    if( isTitle(pos) )
    {
        _isMoving = (_isMoving && isDrag) || isPress;

        if(_isMoving && ! isPress)
        {
            gfx::Point to = _frameRect.topLeft() + pos - _lastPointer;

            if( to.x() < 0)
                to.setX(0);

            if (to.y() < 0)
                to.setY(0);

            // frame coordinates are logical, window position is physical if not scaled
            if( ! _window->scalePosition() )
                to.scale( Application::inst().screen().scaleFactor() );

            _window->move(to);
        }

        return _isMoving;
    }

    return false;
}

bool WindowFrame::checkResize(const gfx::Point& pos, bool isDrag, bool isPress)
{
    if( _window->frameType() == Window::Type::Fixed )
        return false;

    bool onLeftBorder = isLeftBorder(pos);
    bool onRightBorder = isRightBorder(pos);
    bool onTopBorder = isTopBorder(pos);
    bool onBottomBorder = isBottomBorder(pos);

    if(onLeftBorder || onRightBorder || onTopBorder || onBottomBorder)
    {
        _isLeftResizing   = onLeftBorder && ((_isLeftResizing && isDrag) || isPress);
        _isRightResizing  = onRightBorder && ((_isRightResizing && isDrag) || isPress);
        _isTopResizing    = onTopBorder && ((_isTopResizing && isDrag) || isPress);
        _isBottomResizing = onBottomBorder && ((_isBottomResizing && isDrag) || isPress);

        bool isResizing = _isLeftResizing || _isRightResizing ||
                          _isTopResizing || _isBottomResizing;

        if(isResizing && ! isPress)
        {
            gfx::Size winSize = _clientBounds.size();
            gfx::Point winpos = _frameRect.topLeft();
            gfx::Point delta = pos - _lastPointer;

            if( _isLeftResizing )
            {
                winpos.addX( delta.x() );
                winSize.subWidth(delta.x());
            }

            if(_isRightResizing)
                winSize.addWidth( delta.x() );

            if(_isTopResizing)
            {
                winpos.addY( delta.y() );
                winSize.subHeight(delta.y());
            }

            if(_isBottomResizing)
                winSize.addHeight( delta.y() );


            if( winSize != _clientBounds.size() )
            {
                _window->resize(winSize);
                //resize(winSize);
            }

            if( winpos != _frameRect.topLeft() )
            {
                _window->move(winpos);
                //move(winpos);
            }
        }
        else
        {
        }

        return isResizing;
    }

    return false;
}

void WindowFrame::onRepaint(Window& w, const gfx::Rect& rect)
{
    gfx::Point windowPos = onFromWindow( *_window, rect.topLeft() );
    gfx::Rect windowRect( windowPos, rect.size() );

    repaint(windowRect);
}

void WindowFrame::onRequestRepaint(const gfx::Rect& rect)
{
    gfx::Point updatePos = rect.topLeft() + position();
    gfx::Rect updateRect( updatePos, rect.size() );
    
    _wm->repaint(updateRect);
}

void WindowFrame::onProcessPaintEvent(const PaintEvent& ev)
{
    const gfx::Rect& rect = ev.rect();
    
    gfx::Rect winRect(ev.rect().topLeft()- position(), ev.rect().size());

    winRect.setOrigin(onToWindow(*_window, winRect.topLeft()));

    PaintEvent pev(*_window, winRect);
    _window->processEvent(pev);

    Base::onProcessPaintEvent(ev);
}

void WindowFrame::onPaintEvent(const PaintEvent& ev)
{
    Base::onPaintEvent(ev);

    gfx::Graphics& gf = _wm->graphics();

    ClipContext setClipContext(gf, ev.rect());

    //Blitt window content
    const gfx::Pixmap& image = _window->graphics().image();

    
    const gfx::Rect imageRect(_clientRect.topLeft(), image.size());
    const gfx::Rect dirty = ev.rect().intersect(imageRect);

    if( ! dirty.isNull() )
    {    
        const gfx::Rect srcRect(dirty.topLeft() - _clientRect.topLeft(), dirty.size());
        gf.drawImage(dirty.topLeft(), image, srcRect);
    }

    if (_window->frameType() == Window::Type::Borderless)
        return;
            
    //Draw window frame        
    // frame background    
    gfx::Brush brush(colors().aWindowFrame());
    gf.setBrush(brush);
    
    const gfx::Point pos = _frameRect.topLeft();
    
    //Left
    gfx::Rect leftBorder(  pos.x(),
                           pos.x() + _borderWidth-1,
                           pos.y() + _borderWidth,
                           pos.y() + size().height() - _borderWidth);

    gf.fillRect(leftBorder);

    //Top
    gfx::Rect topBorder(pos.x(),
                         pos.x() + size().width(),
                         pos.y(),
                         pos.y() + _borderWidth-1);

    gf.fillRect(topBorder);
    
    //Right
    gfx::Rect rightBorder(pos.x() + size().width() - _borderWidth,
                           pos.x() + size().width(),
                           pos.y() + _borderWidth,
                           pos.y() + size().height() - _borderWidth);

    gf.fillRect(rightBorder);

    //Bottom
    gfx::Rect bottomBorder(pos.x(),
                            pos.x() + size().width(),
                            pos.y() + size().height() - _borderWidth,
                            pos.y() + size().height());

    gf.fillRect(bottomBorder);
    
    
    //Title area
    gfx::Rect titleArea( pos.x() + _borderWidth,
                         pos.x() + size().width() - _borderWidth - 1,
                         pos.y() + _borderWidth,
                         pos.y() + _borderWidth + _titleHeight  -1);
    

    if(isActive())
        gf.setBrush(colors().aWindowTitleBarActive());
    else
        gf.setBrush(colors().aWindowTitleBar());

    if(!isEnabled())
        gf.setBrush(colors().aWindowTitleBarDisabled());

    gf.fillRect(titleArea);


    //Border
    const gfx::ArgbColor borderCoolor = isActive() ? colors().aWindowBorderActive() : colors().aWindowBorder();

    
    gf.setPen(borderCoolor);
    
    gf.drawFrame(_frameRect, 0);

    const size_t penSize = 1;
    const int offset = penSize;
    
    // Title bar text    
    const gfx::ArgbColor titleTextColor = colors().aWindowTitleText();
    Pt::String title = _window->title().c_str();

    gf.setFont(Application::instance().font());
    const gfx::FontMetrics fm = gf.fontMetrics(title);

    const int textOffset = titleArea.height()/2 - fm.height() / 2;

    gfx::Point textPos(titleArea.x() + _borderWidth + _titleHeight,
                       titleArea.y() + textOffset);

    textPos += fm.baseline();
               
    gf.setPen(titleTextColor);
    
    gf.drawText(textPos, title);
    
    // Draw button window buttons        

    std::vector<FrameButton*>::iterator it;

    for(it = _buttons.begin(); it != _buttons.end(); ++it)
    {
        FrameButton* button = *it;     

        if(button->isVisible())
            button->paint(gf);
    }    
}

}}}

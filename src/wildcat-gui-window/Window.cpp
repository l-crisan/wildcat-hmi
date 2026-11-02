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
#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/WindowFrameBase.h>
#include <wildcat/gui/window/WindowManagerBase.h>
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/WindowStateEvent.h>
#include <wildcat/gui/gfx/Algorithm.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/BlockScale.h>


namespace wildcat {
namespace gui {
namespace window {


base::TypeInit<Window> Window::_typeInit("wildcat.gui.window.Window");

Window::Window(WindowManagerBase* parent, WindowType type)
: _frame(0)
, _wm(0)
, _show(false)
, _isActive(false)
, _requestEnabled(true)
, _isClosed(false)
, _type(type)
, _state(WindowState::Normal)
, _isAbove(false)
, _startPostion(WindowPositon::None)
, _showSystemMenu(true)
, _showCloseButton(true)
, _showMinimizeButton(true)
, _showMaximizeButton(true)
, _isModal(false)
{
    setName("Window");  

    eventReceived() += Pt::slot(*this, &Window::onProcessActivateEvent);
    eventReceived() += Pt::slot(*this, &Window::onProcessCloseEvent);
    eventReceived() += Pt::slot(*this, &Window::onProcessWindowStateEvent);

    if(parent)
        setParent(*parent);

    registerProperty("State", *this, &Window::stateI, &Window::setStateI, "Cat=Behavior;Editor=Combo;Param=[Normal,Minimized,Maximized]");
    registerProperty("StartPosition", *this, &Window::startPostionI, &Window::setStartPositionI, "Cat=Behavior;Editor=Combo;Param=[None,CenterParent,CenterScreen]");
    registerProperty("FrameType", *this, &Window::frameTypeI, &Window::setFrameTypeI, "Cat=Apparence;Editor=Combo;Param=[Default,Borderless,Fixed]");
    registerProperty("SystemMenuVisible", *this, &Window::systemMenuVisible, &Window::setShowSystemMenu, "Cat=Apparence;Editor=Check");
    registerProperty("CloseButtonVisible", *this, &Window::closeButtonVisible, &Window::setShowCloseButton, "Cat=Apparence;Editor=Check");
    registerProperty("MinimizeButtonVisible", *this, &Window::minimizeButtonVisible, &Window::setShowMinimizeButton, "Cat=Apparence;Editor=Check");
    registerProperty("MaximizeButtonVisible", *this, &Window::maximizeButtonVisible, &Window::setShowMaximizeButton, "Cat=Apparence;Editor=Check");

    titleChanged() += Pt::slot(*this, &Window::onTitleChanged); 
    setBackground(gfx::Color(0, (int)window::SystemColors::ColorType::Workingspace));
}

Window::~Window()
{
    unparent();
}

void Window::setParent(WindowManagerBase& wm)
{
    if(_wm == &wm)
        return;

    unparent();

    _isClosed = false;

    _wm = &wm;
    _frame = wm.onAttach(*this);
    wm.onInit(*_frame);

    _frame->onInit(*this);

    _frame->onSetSizeLimits(*this, minimumSize(), maximumSize());
    _frame->onSetState(*this, _state);
    _frame->onSetTitle(*this, title());
    _frame->onSetIcon(*this, iconId());
    _frame->onSetAbove(*this, _isAbove);
    _frame->onShowSystemMenu(*this, _showSystemMenu);
    _frame->onShowCloseButton(*this, _showCloseButton);
    _frame->onShowMinimizeButton(*this, _showMinimizeButton);
    _frame->onShowMaximizeButton(*this, _showMaximizeButton);

    if(_state == WindowState::Normal)
    {
        _frame->onMove(*this, position());
        _frame->onResize(*this, size());
    }
    
    
    _frame->onActivate(*this, _isActive);
    _frame->onEnable(*this, _requestEnabled);
    _frame->onShow(*this, _show);

    onSetParent(_frame);    
 }

void Window::unparent()
{
    if( ! _frame )
        return;

    _frame->onRelease(*this);

    _wm->onRelease(*_frame);
    _wm->onDetach(*_frame);
    _wm = 0;

    delete _frame;
    _frame = 0;

    onSetParent(0);
    
}

WindowManagerBase* Window::windowManager()
{
    return _wm;
}

const WindowManagerBase* Window::windowManager() const
{
    return _wm;
}

WindowFrameBase* Window::frame()
{
    return _frame;   
}

const WindowFrameBase* Window::frame() const
{
    return _frame;
}

gfx::Pixmap Window::getImage() const
{
    return graphics().image();
}

void Window::onTitleChanged(Object& obj, Pt::Any& param)
{
    Pt::String t = Pt::any_cast<Pt::String>(param); 

    if(_frame)
        _frame->onSetTitle(*this, t);
}


bool Window::isAbove() const
{
    return _isAbove;
}

void Window::setAbove(bool above)
{
    _isAbove = above;

    if(_frame)
        _frame->onSetAbove(*this, above);
}

///////////////////////////////////////////////////////////////////////
// Element
///////////////////////////////////////////////////////////////////////

Element* Window::onHitTest(const gfx::Point& p, HitTestHint hint)
{
    if( ! bounds().contains(p) || ! isVisible() )
        return nullptr;

    Element* hit = Base::onHitTest(p, hint);
    if(hit)
        return hit;

    return this;
}

WindowBase* Window::onTopParentWindow()
{
    if( _wm == nullptr)
     return this;
        
    if( _wm == &Application::inst().screen().windowManager())
        return this;
    
    WindowBase* w =  _wm->parentWindow();

    if( w == nullptr)
        return this;
        
    return w->topParentWindow();
}

gfx::Point Window::onToParent(const gfx::Point& pos) const
{
    if( ! _frame )
        return pos;

    return _frame->onFromWindow(*this, pos);
}

gfx::Point Window::onFromParent(const gfx::Point& pos) const
{
    if( ! _frame )
        return pos;

    return _frame->onToWindow(*this, pos);
}

bool Window::onProcessEvent(const Pt::Event& ev)
{
    return Base::onProcessEvent(ev);
}

///////////////////////////////////////////////////////////////////////
// invalidation
///////////////////////////////////////////////////////////////////////

void Window::onInvalidateEvent(const InvalidateEvent& ev)
{
    Base::onInvalidateEvent(ev);
}

void Window::onInvalidate()
{
    Base::onInvalidate();    
    repaint( bounds() );     
}

///////////////////////////////////////////////////////////////////////
// painting
///////////////////////////////////////////////////////////////////////

void Window::onRequestRepaint(const gfx::Rect& rect)
{
    //std::clog << "WINDOW REPAINT: " << title() << std::endl;

    if(_frame)
        _frame->onRepaint(*this, rect);
}

void Window::onProcessPaintEvent(const PaintEvent& ev)
{
    const gfx::Rect& rect = ev.rect();
    if( rect.isNull() )
        return;

    if( ! this->isVisible() )
        return;

    Base::onProcessPaintEvent(ev);

    if(_wm)
      _wm->surfaceChanged().send(*this);
}

void Window::onPaintEvent(const PaintEvent& ev)
{    
    Base::onPaintEvent(ev);

    gfx::Graphics& gf =  graphics();
    gf.setClip(ev.rect());

    gf.setBrush(colors().aWindow());
    gf.fillRect( ev.rect() );
}




void Window::onUpdateScaling()  
{    
    const double scaleFactor = Application::inst().screen().scaleFactor();

    graphics().setScaleFactor(scaleFactor);

    if( _frame )
        _frame->updateScaling();

    Base::onUpdateScaling();
}

///////////////////////////////////////////////////////////////////////
// geometry
///////////////////////////////////////////////////////////////////////

void Window::onRequestMove(const gfx::Point& posIn)
{    
    gfx::Point pos = posIn;

    setState(WindowState::Normal);

    if(_frame)
        _frame->onMove(*this, pos);


    WindowBase::onRequestMove(pos);
}

void Window::onProcessMoveEvent(const MoveEvent& ev)
{
    Base::onProcessMoveEvent(ev);
}

void Window::onMoveEvent(const MoveEvent& ev)
{    
    Base::onMoveEvent(ev);
}

void Window::onSetSizeLimits(const gfx::Size& minSize, const gfx::Size& maxSize)
{
    Base::onSetSizeLimits(minSize, maxSize);
    
    if(_frame)
        _frame->onSetSizeLimits(*this, minSize, maxSize);
}

void Window::onRequestResize(const gfx::Size& s)
{    
    setState(WindowState::Normal);
    
    if(_frame)
        _frame->onResize(*this, s);

    WindowBase::onRequestResize(s);
}

void Window::onProcessResizeEvent(const ResizeEvent& ev)
{
    Base::onProcessResizeEvent(ev);
}

void Window::onResizeEvent(const ResizeEvent& ev)
{
    Base::onResizeEvent(ev);   
}

///////////////////////////////////////////////////////////////////////
// visibility
///////////////////////////////////////////////////////////////////////

void Window::onRequestShow(bool b)
{   
    _show = b;

    if( ! _frame )
    {
        Screen& screen = Application::instance().screen();
        screen.addWindow(*this);
    }
    
    invalidate();

    _frame->onShow(*this, b);
}

void Window::onProcessShowEvent(const ShowEvent& ev)
{
    Base::onProcessShowEvent(ev);
}

void Window::onShowEvent(const ShowEvent& ev)
{
    Base::onShowEvent(ev);
}

void Window::onShow(bool visible)
{
    switch (_startPostion)
    {
        case WindowPositon::None:
        break;

        case WindowPositon::CenterParent:
        {
            WindowBase* lp = dynamic_cast<WindowBase*>(logicalParent());

            if (lp)
            {
                gfx::Size psize = lp->size();

                gfx::Size mySize= size();

                gfx::Size s( psize.width()/2  - mySize.width()/ 2, psize.height()/2 - mySize.height()/2);

                gfx::Point startPos = lp->position();

                startPos.addX(s.width());
                startPos.addY(s.height());
            
                move(startPos);
            }
        }
        break;

        case WindowPositon::CenterScreen:
        {
            gfx::Size psize = Application::inst().screen().size();

            gfx::Size mySize = size();

            gfx::Size s(psize.width() / 2 - mySize.width() / 2, psize.height() / 2 - mySize.height() / 2);

            gfx::Point startPos(s.width(), s.height());

            move(startPos);
        }
        break;
    }    

    Base::onShow(visible);
}

int Window::showModal()
{
    Window* activeWindow = 0;
    
    Screen& screen = Application::instance().screen();
    const std::vector<Window*>& windows = screen.windows();
        
    std::vector<Window*>::const_iterator it;    

    for(it = windows.begin(); it != windows.end(); ++it)
    {
        Window* w = (*it);
        
        w->setEnabledState(false);   

        if( w->isActive() )
            activeWindow = w;
    }

    _isModal = true;

    setAbove(true); 
    setEnabledState(true);

    enable(true);
    show(true);

    //Message loop     
    while( ! isClosed() )
        Application::instance().nextEvent();
    
    _isModal = false;

    setAbove(false);

    for(it = windows.begin(); it != windows.end(); ++it)
    {
        Window* w = (*it);        

        w->setEnabledState(true);
        w->enable(true);

        if( activeWindow && activeWindow->id() == w->id() )
            activeWindow->activate();
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////
// enabling
///////////////////////////////////////////////////////////////////////

void Window::onRequestEnable(bool e)
{
    _requestEnabled = e;

    if(_frame)
        _frame->onEnable(*this, e);
}

void Window::onProcessEnableEvent(const EnableEvent& ev)
{
    Base::onProcessEnableEvent(ev);
}

void Window::onEnableEvent(const EnableEvent& ev)
{        
    Base::onEnableEvent(ev);
}

void Window::onSetEnabledState(bool v)
{
    Base::onSetEnabledState(v);

    if (_frame)
        _frame->onEnable(*this, _requestEnabled && v);
 }

void Window::onEnable(bool e)
{
    Base::onEnable(e);

    invalidate();
}

///////////////////////////////////////////////////////////////////////
// activation
///////////////////////////////////////////////////////////////////////

bool Window::isActive() const
{
    return _isActive;
}

void Window::onRequestActivate(bool active)
{
    _isActive = active;

    if( _frame )
        _frame->onActivate(*this, active);
    else
        _isActive = active;
}

void Window::onProcessActivateEvent(const ActivateEvent& ev)
{
    onActivateEvent(ev);
}

void Window::onActivateEvent(const ActivateEvent& ev)
{
    _isActive = ev.isActive();
}

///////////////////////////////////////////////////////////////////////
// window state
///////////////////////////////////////////////////////////////////////

WindowState Window::state() const
{   
    return _state;
}

void Window::setState(const WindowState& s)
{
    _state = s;

    if(_frame)
        _frame->onSetState(*this, _state);
}

void Window::setShowSystemMenu(bool v)
{
    _showSystemMenu = v;

    if (_frame)
        _frame->onShowSystemMenu(*this, _showSystemMenu);
}

void Window::setShowCloseButton(bool v)
{
    _showCloseButton = v;

    if (_frame)
        _frame->onShowCloseButton(*this, _showCloseButton);
}

void Window::setShowMinimizeButton(bool v)
{
    _showMinimizeButton = v;

    if (_frame)
        _frame->onShowMinimizeButton(*this, _showMinimizeButton);
}

void Window::setShowMaximizeButton(bool v)
{
    _showMaximizeButton = v;


    if (_frame)
        _frame->onShowMaximizeButton(*this, _showMaximizeButton);
}

void Window::onProcessWindowStateEvent(const WindowStateEvent& ev)
{
    onWindowStateEvent(ev);
}

void Window::onWindowStateEvent(const WindowStateEvent& ev)
{
    _state = ev.state();
}

///////////////////////////////////////////////////////////////////////
// closing
///////////////////////////////////////////////////////////////////////

bool Window::isClosed() const
{
    return _isClosed;
}

void Window::close()
{
    if( designMode() )
        return;

    if(_frame)
      _frame->onClose(*this);
}

void Window::onProcessCloseEvent(const CloseEvent& ev)
{
    if (designMode())
        return;

    onCloseEvent(ev);
}

void Window::onCloseEvent(const CloseEvent& ev)
{
    if (designMode())
        return;

     // TODO: find better solution to set base class visible
    ShowEvent sev(*this, false);
    Application::instance().processEvent(sev);
    
    unparent();
    
    _isClosed = true;

    Pt::Any a;

    _closeSignal.send(*this, a);       
    
}

///////////////////////////////////////////////////////////////////////
// input
///////////////////////////////////////////////////////////////////////

bool Window::acceptsInput() const
{
    if( ! isEnabled() )
        return false;

    if( ! isVisible() )
        return false;

    return true;
}

bool Window::onProcessMouseEvent(const MouseEvent& ev)
{
    if( ! acceptsInput() )
        return false;

    return Base::onProcessMouseEvent(ev);
}

bool Window::onProcessTouchEvent(const TouchEvent& ev)
{
    if( ! acceptsInput() )
        return false;

    return Base::onProcessTouchEvent(ev);
}

bool Window::onProcessScrollEvent(const ScrollEvent& ev)
{
    if( ! acceptsInput() )
        return false;
  
    return Base::onProcessScrollEvent(ev);
}

void Window::onProcessEnterEvent(const EnterEvent& ev)
{
    if (!acceptsInput())
        return;

    Base::onProcessEnterEvent(ev);
}

void Window::onProcessLeaveEvent(const LeaveEvent& ev)
{
    if (!acceptsInput())
        return;

    Base::onProcessLeaveEvent(ev);
}

bool Window::onProcessKeyEvent(const KeyEvent& ev)
{
    if( ! acceptsInput() )
        return false;
    
    return Base::onProcessKeyEvent(ev);
}

bool Window::onMouseEvent(const MouseEvent& ev)
{
    return Base::onMouseEvent(ev);
}

bool Window::onTouchEvent(const TouchEvent& ev)
{ 
    return Base::onTouchEvent(ev);
}

bool Window::onScrollEvent(const ScrollEvent& ev)
{
    return Base::onScrollEvent(ev);
}

bool Window::onKeyEvent(const KeyEvent& ev)
{
    return Base::onKeyEvent(ev);
}

bool Window::onEnterEvent(const EnterEvent& ev)
{
    return Base::onEnterEvent(ev);
}

bool Window::onLeaveEvent(const LeaveEvent& ev )
{

    return Base::onLeaveEvent(ev);
}

void Window::onSetIcon(uint32_t icon)
{
    WindowBase::onSetIcon(icon);
    
    if (_frame)
        _frame->onSetIcon(*this, icon);

    invalidate();
}


}}}


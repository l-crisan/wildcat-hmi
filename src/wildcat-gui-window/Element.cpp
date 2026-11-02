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
#include <wildcat/gui/window/Element.h>
#include <wildcat/gui/window/Widget.h>
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/MouseEvent.h>
#include <wildcat/gui/window/TouchEvent.h>
#include <wildcat/gui/window/ScrollEvent.h>
#include <wildcat/gui/window/EnterEvent.h>
#include <wildcat/gui/window/LeaveEvent.h>
#include <wildcat/gui/window/KeyEvent.h>
#include <wildcat/gui/window/WindowBase.h>
#include <wildcat/gui/gfx/Graphics.h>

namespace wildcat {
namespace gui {
namespace window {


Element::Element()
: _logicalParent(nullptr)
, _parent(nullptr)
, _invalidates(0)
, _enabled(true)
, _enabledByParent(true)
, _isVisible(false)
, _minimumSize(0, 0)
, _maximumSize(64000, 64000)
, _acceptScrollEvent(false)
, _cursor(resourceMng().id("ArrowCursor"))
, _langId(0)
, _colorSchemeId(0)
{         
    Object::onSetResourceManager(Application::instance().resourceMng());
    Application::instance().registerElement(*this);

    _dispatcher += Pt::slot(*this, &Element::onProcessInvalidateEvent);    
    _dispatcher += Pt::slot(*this, &Element::onProcessPaintEvent);
    _dispatcher += Pt::slot(*this, &Element::onProcessResizeEvent);
    _dispatcher += Pt::slot(*this, &Element::onProcessEnableEvent);
    _dispatcher += Pt::slot(*this, &Element::onProcessShowEvent);
    _dispatcher += Pt::slot(*this, &Element::onProcessMoveEvent);
    _dispatcher += Pt::slot(*this, &Element::onProcessEnterEvent);
    _dispatcher += Pt::slot(*this, &Element::onProcessLeaveEvent);

    //Signals
    registerSignal("MousePressSignal", _mousePressSignal, "Cat=Element");
    registerSignal("MouseReleaseSignal", _mouseReleaseSignal, "Cat=Element");
    registerSignal("MouseMoveSignal", _mouseMoveSignal, "Cat=Element");
    registerSignal("PaintSignal", _paintSignal, "Cat=Element");
    registerSignal("InvalidateSignal", _invalidateSignal, "Cat=Element");
    registerSignal("ShowSignal", _showSignal, "Cat=Element");
    registerSignal("EnableSignal", _enableSignal, "Cat=Element");
    registerSignal("MouseSignal", _mouseSignal, "Cat=Element");
    registerSignal("TouchSignal", _touchSignal, "Cat=Element");
    registerSignal("ScrollSignal", _scrollSignal, "Cat=Element");
    registerSignal("KeySignal", _keySignal, "Cat=Element");
    registerSignal("EnterSignal", _enterSignal, "Cat=Element");
    registerSignal("LeaveSignal", _leaveSignal, "Cat=Element");
    registerSignal("CursorChanged", _cursorChanged, "Cat=Control");
    registerSignal("PositionChanged", _positionChanged, "Cat=Element");
    registerSignal("SizeChanged", _sizeChanged, "Cat=Element");
    registerSignal("MinimumSizeChanged", _minimumSizeChanged, "Cat=Element");
    registerSignal("MaximumSizeChanged", _maximumSizeChanged, "Cat=Element");   
    registerSignal("AcceptScrollEventChanged", _acceptScrollEventChanged, "Cat=Element");

    //Properties
    registerProperty("Visible", *this, &Element::isVisible, &Element::show,"Cat=Behavior;Editor=Check");
    registerProperty("Enable", *this, &Element::isEnabled, &Element::enable, "Cat=Behavior;Editor=Check");
    registerProperty("Position", *this, &Element::position, &Element::move, "Cat=Layout");
    registerProperty("Size", *this, &Element::size, &Element::resize, "Cat=Layout");
    registerProperty("MinimumSize", *this, &Element::minimumSize, &Element::setMinimumSize, "Cat=Layout");
    registerProperty("MaximumSize", *this, &Element::maximumSize, &Element::setMaximumSize, "Cat=Layout");
    registerProperty("Cursor", *this, &Element::cursorId, &Element::setCursorId,"Cat=Apparence;Editor=Cursor");
}

Element::~Element()
{
    setCapture(false);
   
    Application::instance().unregisterElement(*this);
}

//
// hierachy management
// 

Element* Element::parent()
{
    return _parent;
}

const Element* Element::parent() const
{
    return _parent;
}

Element* Element::logicalParent()
{
    return _logicalParent == nullptr ? _parent : _logicalParent;
}

const Element* Element::logicalParent() const
{
    return _logicalParent == nullptr? _parent:  _logicalParent;
}

void Element::setLogicalParent(Element* parent)
{
    _logicalParent = parent;
}


void Element::onSetParent(Element* Element)
{
    _parent = Element;

    if( _parent == nullptr)
        _isVisible = false; 
}

const base::Resource* Element::appResource(uint32_t id) const
{
    return Application::instance().resourceMng().get<base::Resource>(id);
}

base::Resource* Element::appResource(uint32_t id)
{
    return Application::instance().resourceMng().get<base::Resource>(id);
}


bool Element::isDescendantOf(const Element& v) const
{
    const Element* parent = this->parent();
    if( ! parent )
        return false;

    if( parent == &v )
        return true;

    return parent->isDescendantOf(v);
}

bool Element::isAncestorOf(const Element& v) const
{
    return v.isDescendantOf(*this);
}

Element* Element::hitTest(const gfx::Point& pos, HitTestHint hint)
{
    return onHitTest(pos, hint);
}

Element* Element::onHitTest(const gfx::Point& pos, HitTestHint hint)
{
    return nullptr;
}

gfx::Point Element::toParent(const gfx::Point& pos) const
{
    return onToParent(pos);
}

gfx::Point Element::fromParent(const gfx::Point& pos) const
{
    return onFromParent(pos);
}

gfx::Point Element::toGlobal(const gfx::Point& pos) const
{
    return onToGlobal(pos); 
}
    
gfx::Point Element::fromGlobal(const gfx::Point& pos) const
{
    return onFromGlobal(pos);
}

gfx::Point Element::onToGlobal(const gfx::Point& pos) const
{
    const Element* parent = this->parent();
    if( ! parent )
        return pos;

    gfx::Point parentPos = toParent(pos);
    return parent->toGlobal(parentPos);
}

gfx::Point Element::onFromGlobal(const gfx::Point& pos) const
{
    const Element* parent = this->parent();
    if( ! parent )
        return pos;

    gfx::Point parentPos = parent->fromGlobal(pos);
    return fromParent(parentPos);
}

void Element::setAcceptScrollEvent(bool v)
{
    onSetAcceptScrollEvent(v);
}

void Element::onSetAcceptScrollEvent(bool v)
{
    _acceptScrollEvent = v;
    Pt::Any a(v);
    _acceptScrollEventChanged.send(*this, a);
    invalidate();
}

//
// event processing
//

bool Element::processEvent(const Pt::Event& ev)
{
    return onProcessEvent(ev);
}

bool Element::onProcessEvent(const Pt::Event& ev)
{
    //std::clog << typeid(ev).name() << std::endl;    
    bool consumed = false;

    if (ev.typeInfo() == typeid(MouseEvent))
    {
        const MouseEvent* mev =  (const MouseEvent*)&ev;
        consumed =  onProcessMouseEvent(*mev);
    }
    
    if (ev.typeInfo() == typeid(TouchEvent))
    {
        const TouchEvent* tev = (const TouchEvent*)&ev;
        consumed = onProcessTouchEvent(*tev);
    }

    if (ev.typeInfo() == typeid(KeyEvent))
    {
        const KeyEvent* kev = (const KeyEvent*)&ev;
        consumed = onProcessKeyEvent(*kev);
    }

    if (ev.typeInfo() == typeid(ScrollEvent))
    {
        const ScrollEvent* kev = (const ScrollEvent*)&ev;
        consumed = onProcessScrollEvent(*kev);
    }

    if (ev.typeInfo() == typeid(ResizeEvent))
    {
        const ResizeEvent* kev = (const ResizeEvent*)&ev;
        onProcessResizeEvent(*kev);
        consumed = true;
    }    
    if( consumed)
        return true;

    _dispatcher.send(ev);
            
    return false;
}

Pt::Signal<const Pt::Event&>& Element::eventReceived()
{
    return _dispatcher;
}

//
// invalidation
//

void Element::invalidate()
{
    ++_invalidates;

    InvalidateEvent ev(*this);
    Application::instance().commitEvent(ev);
}

// TODO: invalidate() should lead to an update event
void Element::onProcessInvalidateEvent(const InvalidateEvent& ev)
{
    --_invalidates;

    if(_invalidates > 0)
      return;

    onInvalidateEvent(ev);
}

// TODO: remove and only use onInvalidateEvent
void Element::onInvalidateEvent(const InvalidateEvent& ev)
{
    onInvalidate();
}
    
void Element::onInvalidate()
{
    Pt::Any a;

  
    _invalidateSignal.send(*this, a);    
}

//
// painting
//

void Element::repaint(const gfx::Rect& rect)
{
    onRequestRepaint(rect);
}

void Element::repaint()
{
    repaint( bounds() );
}

void Element::onRequestRepaint(const gfx::Rect& rect)
{
}

void Element::onProcessPaintEvent(const PaintEvent& ev)
{
    
    if( ev.rect().isNull() )
        return;

    onPaintEvent(ev);    
}

void Element::onPaintEvent(const PaintEvent& ev)
{
    Pt::Any a(&ev);

    _paintSignal.send(*this, a);
}

void Element::onUpdateScaling()
{
    move(position());   
    resize(size());
}

//
// visibility
//

bool Element::isVisible() const
{
    return _isVisible;
}

void Element::show(bool isShow)
{
    onRequestShow(isShow);
}

void Element::onRequestShow(bool e)
{
}

void Element::onProcessShowEvent(const ShowEvent& ev)
{
    if( _isVisible == ev.visible() )
        return;

    onShowEvent(ev);
}

void Element::onShowEvent(const ShowEvent& ev)
{
    onShow( ev.visible() );
}


// TODO: remove and only use onShowEvent
void Element::onShow(bool isShown)
{
    _isVisible = isShown;

    Pt::Any a(_isVisible);
    _showSignal.send(*this, a);  
}

// enabling


bool Element::isEnabled() const
{
    return (_enabled && _enabledByParent);
}

void Element::enable(bool isEnable)
{
    onRequestEnable(isEnable);
}

void Element::setEnabledState(bool v)
{
    onSetEnabledState(v);
}

bool Element::isEnabledState() const
{
    return _enabledByParent;
}

void Element::onSetEnabledState(bool v)
{
    _enabledByParent = v;
}

void Element::onRequestEnable(bool e)
{
}

bool Element::onMousePress(const MouseEvent& ev)
{
    bool result = false;

    EventSignal evSig(result, ev);

    Pt::Any a(&evSig);
    _mousePressSignal.send(*this, a)  ;

    return result;
}

bool Element::onMouseRelease(const MouseEvent& ev)
{
    bool result = false;

    EventSignal evSig(result, ev);

    Pt::Any a(&evSig);
    _mouseReleaseSignal.send(*this, a);

    return result;
}

bool Element::onMouseMove(const MouseEvent& ev)
{
    bool result = false;

    EventSignal evSig(result, ev);

    Pt::Any a(&evSig);
    _mouseMoveSignal.send(*this, a);

    return result;
}

void Element::onProcessEnableEvent(const EnableEvent& ev)
{
    onEnableEvent(ev);
}

void Element::onEnableEvent(const EnableEvent& ev)
{    
    _enabled = ev.enabled();

    onEnable( ev.enabled() );
}

void Element::onEnable(bool isEnable)
{
    Pt::Any a(isEnable);
    _enableSignal.send(*this, a);   
}

//
// activation
//

void Element::activate(bool active)
{
    onRequestActivate(active);
}

bool Element::isActivate() const
{
    return false;
}

void Element::onRequestActivate(bool active)
{
}

//
// geometry
//

const gfx::Point& Element::position() const
{
    return _pos;
}

void Element::move(const gfx::Point& pos)
{
    onRequestMove(pos);
    repaint();
}

void Element::setPosition(const gfx::Point& pos)
{
    onRequestMove(pos);
}

void Element::onRequestMove(const gfx::Point& pos)
{
    _pos = pos;
    
}

void Element::onProcessMoveEvent(const MoveEvent& ev)
{
    onMoveEvent(ev);   
}

void Element::onMoveEvent(const MoveEvent& ev)
{
    repaint();
    _pos = ev.position();
    repaint();
    Pt::Any a(_pos);
    _positionChanged.send(*this, a);   
}

const gfx::Size& Element::size() const
{
    return _size;
}

const gfx::Size& Element::minimumSize() const
{
    return _minimumSize;
}

void Element::setMinimumSize(const gfx::Size& s)
{
    _minimumSize = s;

    onSetSizeLimits(_minimumSize, _maximumSize);

    Pt::Any a(s);
    _minimumSizeChanged.send(*this, a); 
}

void Element::setMinimumSize(int w, int h)
{
    setMinimumSize( gfx::Size(w, h) );
}

void Element::setMinimumWidth(int w)
{
    setMinimumSize( w, _minimumSize.height() );
}

void Element::setMinimumHeight(int h)
{
    setMinimumSize( _minimumSize.width(), h );

}

const gfx::Size& Element::maximumSize() const
{
    return _maximumSize;
}

void Element::setMaximumSize(const gfx::Size& s)
{
    _maximumSize = s;

    onSetSizeLimits(_minimumSize, _maximumSize);
    Pt::Any a(_maximumSize);
    _maximumSizeChanged.send(*this, a); 
}

bool Element::scrollEvent(const ScrollEvent& ev)
{
    return onScrollEvent(ev);
}

void Element::setMaximumSize(int w, int h)
{
    setMaximumSize( gfx::Size(w, h) );
    
}

void Element::setMaximumWidth(int w)
{
    setMaximumSize( w, _maximumSize.height() );
    
}

void Element::setMaximumHeight(int h)
{
    setMaximumSize( _maximumSize.width(), h );
    
}

void Element::onSetSizeLimits(const gfx::Size& minSize, 
                             const gfx::Size& maxSize)
{
}

void Element::setSize(const gfx::Size& s)
{
    onRequestResize(s);
}


uint32_t Element::getColorSchemeId() const
{
    if (_colorSchemeId != 0)
        return _colorSchemeId;

    if (_parent != 0)
        return _parent->getColorSchemeId();

    return 0;
}

uint32_t Element::getLangId() const
{
    if( _langId != 0)
        return _langId;

    if( _parent != 0)
        return _parent->getLangId();

    return 0;
}

const Pt::String& Element::tr(const Pt::String& key) const
{
    uint32_t id = getLangId();
    
    if (id == 0)
          id = resourceMng().id(Application::inst().languageCode());
        
    const base::TextTable* tb = getRecource<base::TextTable>(id);

    if( tb == nullptr)
        return key;

    return tb->tr(key);
}


void Element::resize(const gfx::Size& s)
{       
    onRequestResize(s);
    invalidate();
}

void Element::onRequestResize(const gfx::Size& s)
{
    _size = s;
}

void Element::onProcessResizeEvent(const ResizeEvent& ev)
{
    onResizeEvent(ev);
}

void Element::onResizeEvent(const ResizeEvent& ev)
{
    repaint();
    _size = ev.size();    
    repaint();

    Pt::Any a(_size);
    _sizeChanged.send(*this, a);
}

//
// input capture
//

void Element::setCapture(bool capture)
{ 
    onRequestCapture(capture);
}

bool Element::hasCapture() const
{
    return Application::instance().capture() == this;  
}

void Element::onRequestCapture(bool capture)
{
    Application::instance().onRequestCapture(*this, capture);
}


//
// input processing
//


bool Element::mouseEvent(const MouseEvent& ev)
{
    gfx::Point localPos = onFromGlobal(ev.position());

    MouseEvent localEv(ev);
    localEv.setPosition(localPos);

    return onMouseEvent(localEv);;
}

bool Element::onMouseEvent(const MouseEvent& ev)
{    
    if (ev.isPress())
    {
        if(onMousePress(ev))
            return true;
    }
    else if (ev.isRelease())
    {
        if(onMouseRelease(ev))
            return true;
    }
    else if (!ev.isAnyPress())
    {
        if(onMouseMove(ev))
            return true;
    }

    bool r = false;    

    EventSignal sigEv(r, ev);    

    Pt::Any a(&sigEv);
    _mouseSignal.send(*this, a);

    return r;
}

bool Element::touchEvent(const TouchEvent& ev)
{
    gfx::Point localPos = onFromGlobal(ev.position());

    TouchEvent localEv(ev);
    localEv.setPosition(localPos);

    return onTouchEvent(localEv);
}

bool Element::enterEvent(const EnterEvent& ev)
{

    return  onEnterEvent(ev);
}

bool Element::leaveEvent(const LeaveEvent& ev)
{
    return onLeaveEvent(ev);
}

bool Element::onProcessMouseEvent(const MouseEvent& ev)
{ 

    return mouseEvent(ev);
}

bool Element::keyEvent(const KeyEvent& ev)
{

    return onKeyEvent(ev);
}

bool Element::onProcessTouchEvent(const TouchEvent& ev)
{
    return touchEvent(ev);
}

bool Element::onTouchEvent(const TouchEvent& ev)
{

    bool r = false;
    EventSignal sigEv(r, ev);   

    Pt::Any a(&sigEv);
    _touchSignal.send(*this, a);  

    return r;
}

bool Element::onProcessScrollEvent(const ScrollEvent& ev)
{
    if (!acceptScrollEvent())
    {
        if(dynamic_cast<WindowBase*>(parent()))
            return true;

        return parent() ? parent()->processEvent(ev) : false;
    }

    return scrollEvent(ev);
}

const Cursor& Element::cursor() const
{
    const Cursor* p = getRecource<Cursor>(_cursor);

    if (p == nullptr)
    {
        static Cursor emptyPixmap;
        return emptyPixmap;
    }

    return *p;
}

uint32_t Element::cursorId() const
{
    return _cursor;
}

void Element::setCursor(const std::string& name)
{
    uint32_t id = resourceMng().id(name);
    setCursorId(id);
}

void Element::setCursorId(uint32_t csr)
{    
    bool changed = false;

    if (_cursor != csr)
    {
        _cursor = csr;
        changed = true;
    }

    Application& app = Application::instance();

    Element* ep = app.pointed();

    if (app.capture() == this)
    {
        app.setCursor(&cursor());
    }
    else if (app.capture() == nullptr && app.pointed() == this)
    {
        app.setCursor(&cursor());
    }

    if(changed)
    {
        Pt::Any a(_cursor);
        _cursorChanged.send(*this, a);
    }
}


bool Element::onScrollEvent(const ScrollEvent& ev)
{
    bool r = false;

    EventSignal sigEv(r, ev);
    Pt::Any a(&sigEv);
    _scrollSignal.send(*this, a);

    return r;
}

void Element::onProcessEnterEvent(const EnterEvent& ev)
{
    enterEvent(ev);
}

bool Element::onEnterEvent( const EnterEvent& ev)
{    
    bool r = false;
    
    setCursorId(_cursor);    
    EventSignal sigEv(r, ev);

    Pt::Any a(&sigEv);
    _enterSignal.send(*this, a);

    return r;

}

void Element::onProcessLeaveEvent(const LeaveEvent& ev)
{
    leaveEvent(ev);
}

bool Element::onLeaveEvent(const LeaveEvent& ev)
{    
    bool r = false;

    EventSignal sigEv(r, ev);

    Pt::Any a(&sigEv);
    _leaveSignal.send(*this, a);

    return r;    
}

bool Element::onProcessKeyEvent(const KeyEvent& ev)
{
    return keyEvent(ev);
}

bool Element::onKeyEvent(const KeyEvent& ev)
{
    bool r = false;

    EventSignal sigEv(r, ev);

    Pt::Any a(&sigEv);
    _keySignal.send(*this, a);

    return r;

}

}}}

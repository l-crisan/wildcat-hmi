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
#include <wildcat/gui/window/Control.h>
#include <wildcat/gui/window/WindowBase.h>
#include <wildcat/gui/window/Application.h>
#include <Pt/String.h>
#include <cmath>
#include <cassert>

namespace wildcat {
namespace gui {
namespace window {

Control::Control()
: _parent(nullptr)
, _form(nullptr)
, _show(true)
, _enabled(true)
, _preferredSize()
, _hasFocus(false)
, _focusPolicy(NoFocus)
, _focusIndex(0)
, _actionKey(Key::Space)
, _shortcutKey(Key::NoKey)
, _mnemonic()
, _mnemonicEntered()
, _padding()
, _margin()
, _dockPosition(DockPosNone)
, _toolTip()
, _dockPermission(DockPerAll)
{
    eventReceived() += Pt::slot(*this, &Control::onProcessLayoutEvent);
    eventReceived() += Pt::slot(*this, &Control::onProcessFocusEvent);

    registerSignal("LayoutSignal", _layoutSignal, "Cat=Control");
    registerSignal("PaddingChanged", _paddingChanged, "Cat=Control");
    registerSignal("MarginChanged", _marginChanged, "Cat=Control");
    registerSignal("DockPositionChanged", _dockPositionChanged, "Cat=Control");
    registerSignal("DockPermisionChanged", _dockPermissionChanged, "Cat=Control");
    registerSignal("ToolTipChanged", _toolTipChanged, "Cat=Control");
    registerSignal("TextChanged", _textChanged, "Cat=Control");
    registerSignal("IconChanged", _iconChanged, "Cat=Control");
    registerSignal("FocusPolicyChanged", _focusPolicyChanged, "Cat=Control");
    registerSignal("FocusIndexChanged", _focusIndexChanged, "Cat=Control");
    registerSignal("ActionKeyChanged", _actionKeyChanged, "Cat=Control");
    registerSignal("ShortcutKeyChanged", _shortcutKeyChanged, "Cat=Control");
    registerSignal("MnemonicChanged", _mnemonicChanged, "Cat=Control");

    
    registerProperty("FocusPolicy", *this, &Control::focusPolicyI, &Control::setFocusPolicyI, "Cat=Behavior;Editor=Combo;Param=[NoFocus,AcceptFocus]");
    registerProperty("FocusIndex", *this, &Control::focusIndex, &Control::setFocusIndex, "Cat=Behavior");
    registerProperty("ActionKey", *this, &Control::actionKey, &Control::setActionKey, "Cat=Behavior;Editor=Key");
    registerProperty("Shortcut", *this, &Control::shortcut, &Control::setShortcut, "Cat=Behavior;Editor=Key");
    registerProperty("Mnemonic", *this, &Control::mnemonic, &Control::setMnemonic, "Cat=Behavior");
    registerProperty("Margin", *this, &Control::margin, &Control::setMargin,"Cat=Layout");
    registerProperty("Padding", *this, &Control::padding, &Control::setPadding, "Cat=Layout");
    registerProperty("Children", *this, &Control::objectAt, &Control::addObject, &Control::childrenCount,"Cat=Development");
    registerProperty("ToolTip", *this, &Control::toolTip, &Control::setToolTip, "Cat=Behavior");
    registerProperty(DockModeProperty, *this, &Control::dockModeI, &Control::setDockModeI, "Cat=Layout;Editor=Combo;Param=[Left,Right,Top,Bottom,Fill,None#-1]");
    registerProperty("DockPermission", *this, &Control::dockPermission, &Control::setDockPermission, "Cat=Layout;Editor=BitOption;Param=[None#0,Left#1,Right#2,Top#4,Bottom#8,Center#16,All#31]");
}

Control::~Control()
{
    while( ! _children.empty() )
        remove( *_children.back() );

    unparent();
}

void Control::setParent(View& parent)
{
    if(_parent == &parent)
        return;

    unparent();

    parent.onAttach(*this);
    _parent = &parent;

    _parent->onInit(*this);
    _parent->onEnableRequest(*this, _enabled);
    _parent->onShowRequest(*this, _show);
    _parent->onMoveRequest(*this, position());
    _parent->onResizeRequest(*this, size());

    invalidate();

    onSetParent(_parent);
}

void Control::unparent()
{
    if( ! _parent )
        return;

    _parent->onRelease(*this);
    _parent->onDetach(*this);
    _parent = 0;
        
    invalidate();

    onSetParent(0);
}

void Control::showToolTip(const gfx::Point& pos)
{
    onShowToolTip(pos);
}

void Control::onSetResourceManager(base::ResourceManager& v)
{
    ControlBase::onSetResourceManager(v);

    for(size_t i = 0; i < _children.size(); ++i)
    {
        Control* w = _children[i];
        w->setResourceManager(v);
    }
}

void Control::closeToolTip()
{
    onCloseToolTip();   
}


void Control::setToolTip(const Pt::String& text)    
{
    onSetToolTip(text);
}

const Pt::String& Control::onGetToolTip() const
{
    return _toolTip;
}

void Control::onSetToolTip(const Pt::String& text)
{
    _toolTip = text;
    Pt::Any a(text);
    _toolTipChanged.send(*this, a);
}


bool Control::contains(const Control* control) const
{
    if (control == this)
        return true;

    std::vector<Control*>::const_iterator it;

    for (it = _children.begin(); it != _children.end(); ++it)
    {
        const Control* child = *it;

        if (child->contains(control))
            return true;
    }

    return false;       
}


void Control::onAttach(Control& widget)
{
    _children.push_back(&widget);
}

void Control::onDetach(Control& widget)
{
    std::vector<Control*>::iterator it;

    it = std::find(_children.begin(), _children.end(), &widget);

    if( it != _children.end() )
        _children.erase(it);
}

void Control::onInit(Control& widget)
{
    const gfx::Point surfacePos = _graphics.area().topLeft() + widget.position();

    widget.setGraphics(&_graphics, surfacePos);
    
    widget.setForm(_form);
    
    relayout();
}

void Control::onRelease(Control& widget)
{
    widget.setForm(0);
    widget.setGraphics( 0, widget.position() );
    
    relayout();
}

void Control::setDockPosition(DockPosition v)
{
    _dockPosition = v;
    Pt::Any a(v);
    _dockPositionChanged.send(*this, a);
    invalidate();
}



void Control::setDockPermission(int p)
{
    _dockPermission = p;
    Pt::Any a(p);
    _dockPermissionChanged.send(*this, a);
    invalidate();
}

void Control::setForm(WindowBase* form)
{
    if(_form)
        _form->onRemoveElement(*this);

    _form = 0;

    if(form)
        form->onAddElement(*this);

    _form = form;

    std::vector<Control*>::iterator it;
    for(it = _children.begin(); it != _children.end(); ++it)
    {
        Control* widget = *it;
        widget->setForm(form);
    }
}

void Control::add(Control& w)
{
    onAddChild(w); 
    
}

void Control::remove(Control& w)
{
    onRemoveChild(w);
    
}

void Control::removeAll()
{
    while( ! _children.empty() )
        remove( *_children.back() );
}   

void Control::onAddChild(Control& w)
{
    w.setParent(*this);
}

void Control::onRemoveChild(Control& w)
{
    w.unparent();
}

const std::vector<Control*>& Control::children() const
{
    return _children;
}

gfx::Graphics& Control::graphics()
{
    return _graphics;
}

const gfx::Graphics& Control::graphics() const
{
    return _graphics;
}

void Control::setGraphics(gfx::Graphics* gf, const gfx::Point& pos)
{
    _graphics.setName(name());
    if( !gf)
    {
        _graphics.detach();
        
    }
    else
    {
        gfx::Rect clientRect( pos, size() );
        _graphics.attach(*gf, clientRect);
    }
    
    onSetGraphics(gf, pos);

    std::vector<Control*>::iterator it;
    for(it = _children.begin(); it != _children.end(); ++it)
    {
        Control* widget = *it;

        widget->setGraphics(&_graphics, widget->position());
    }
}

void Control::onSetGraphics(gfx::Graphics* gf, const gfx::Point& pos)
{
        
}

gfx::Point Control::onToControl(const Control& widget, const gfx::Point& pos) const
{
    return pos - widget.position();
}

gfx::Point Control::onFromControl(const Control& widget, const gfx::Point& pos) const
{
    return pos + widget.position();
}

const gfx::Rect Control::geometry() const
{
    return gfx::Rect( position(), size() );
}

Control::FocusPolicy Control::focusPolicy() const
{
    return _focusPolicy;
}

void Control::setFocusPolicy(FocusPolicy policy)
{
    _focusPolicy = policy;

    if(_form)
        _form->onSetFocusPolicy(*this, policy);
}

int Control::focusPolicyI() const
{
    return (int)focusPolicy();
}


WindowBase* Control::topParentWindow()
{
    if (_form == nullptr)
        return nullptr;

    return _form->topParentWindow();
}

void Control::setFocusPolicyI(int f)
{
    setFocusPolicy((FocusPolicy)f);
}

size_t Control::focusIndex() const
{
    return _focusIndex;
}

void Control::setFocusIndex(size_t index)
{
    _focusIndex = index;
    
    if(_form)
        _form->onSetFocusIndex(*this, index);

    Pt::Any a(_focusIndex);
    _focusIndexChanged.send(*this, a);   
}

bool Control::hasFocus() const
{
    return _hasFocus;
}

void Control::focus()
{
    if (focusPolicy() == FocusPolicy::NoFocus)
        return;

    if(_form)
        _form->onSetFocus(*this);
}

void Control::onProcessFocusEvent(const FocusEvent& ev)
{
    onFocusEvent(ev);
}

void Control::onFocusEvent(const FocusEvent& ev)
{
    _hasFocus = ev.isFocused();
    
    Pt::Any a(_hasFocus);
    _focusPolicyChanged.send(*this, a);   
}


Key Control::actionKey() const
{
    return onGetActionKey();
}

void Control::setActionKey(Key ak)
{
    onSetActionKey(ak);
}

void Control::onSetActionKey(Key ak)
{
    _actionKey = ak;    
    Pt::Any a(_actionKey);
    _actionKeyChanged.send(*this, a);    
}

Key Control::onGetActionKey() const
{
    return _actionKey;  
}


void Control::onActionKey(const KeyEvent& kev)
{
}

const Key& Control::shortcut() const
{
    return onGetShortcut();
}

void Control::setShortcut(const Key& key)
{
    onSetShortcut(key); 
}

void Control::onSetShortcut(const Key& key)
{
    _shortcutKey = key;

    if(_form) 
        _form->onSetShortcut(*this, onGetShortcuts());

    Pt::Any a(_shortcutKey);    

    _shortcutKeyChanged.send(*this, a);    
}

const Key& Control::onGetShortcut() const
{
    return _shortcutKey;
}

bool Control::processShortcut(const Key& key)
{
    if( !acceptsInput())
        return false;

    return onShortcut(key);
}

const std::vector<Key> Control::onGetShortcuts()
{
    std::vector<Key> shc;

    if(_shortcutKey.empty())
        return shc;

    shc.push_back(_shortcutKey);

    return shc;
}

const std::vector<Pt::Char> Control::onGetMnemonics()
{
    std::vector<Pt::Char> mcs;

    if(_mnemonic == 0)
        return mcs;

    mcs.push_back(_mnemonic);
    return mcs;
}


void Control::onSetEnabledState(bool v)
{
    ControlBase::onSetEnabledState(v);

    for(size_t i = 0; i < children().size(); ++i)
        children()[i]->setEnabledState(v);    
}

bool Control::onShortcut(const Key& key)
{
    return true;
}

const Pt::Char& Control::mnemonic() const
{
    return onGetMnemonic();
}

void Control::setMnemonic(const Pt::Char& ch)
{
    onSetMnemonic(ch);
}

void Control::onSetMnemonic(const Pt::Char& ch)
{
    _mnemonic = ch;
    if(_form)
        _form->onSetMnemonic(*this, onGetMnemonics());

    Pt::Any a(_mnemonic);
    _mnemonicChanged.send(*this, a);  
}

const Pt::Char& Control::onGetMnemonic() const
{
    return _mnemonic;   
}

Pt::String Control::setMnemonic(const Pt::String& text)
{
    Pt::String str;
    Pt::Char mnemonic = 0;

    bool onAmp = false;
    for(Pt::String::const_iterator it = text.begin(); it != text.end(); ++it)
    {
        if(onAmp)
        {
            if(*it != '&')
                mnemonic = *it;

            str += *it;
            onAmp = false;
        }
        else
        {
            if(*it == '&')
                onAmp = true;
            else
                str += *it;
        }
    }

    if(onAmp)
        str += '&';

    setMnemonic(mnemonic);

    return str;
}

void Control::setMnemonicWidget(Control* w)
{
    _mnemonicEntered.disconnect();

    if(w)
        _mnemonicEntered += Pt::slot(*w, &Control::onMnemonic);
}

void Control::processMnemonic(Pt::Char m)
{
    onMnemonic(m);
}

void Control::onMnemonic(Pt::Char m)
{
    _mnemonicEntered.send(m);
}

void Control::onInvalidateEvent(const InvalidateEvent& ev)
{
    if (_graphics.base())
        ControlBase::onInvalidateEvent(ev);
}

void Control::onShowToolTip(const gfx::Point& pos)
{

}

void Control::onCloseToolTip()
{

}

void Control::onInvalidate()
{
    ControlBase::onInvalidate();

    _trToolTip = tr(_toolTip);  

    relayout();
}

void Control::onRequestRepaint(const gfx::Rect& rect)
{
    if(_parent)
        _parent->onRepaintRequest(*this, rect);
}

void Control::onRepaintRequest(Control& w, const gfx::Rect& rect)
{
    gfx::Point widgetPos = fromControl( w, rect.topLeft() );
    gfx::Rect widgetRect( widgetPos, rect.size() );

    repaint(widgetRect);
}

void Control::onProcessPaintEvent(const PaintEvent& ev)
{    
    const gfx::Rect& r = ev.rect();
    if( r.isNull() )
        return;

    if( ! isVisible() )
        return;
        
    View::onProcessPaintEvent(ev);
            
    // paint child widgets    
    std::vector<Control*>::const_iterator it;

    for(it = _children.begin() ; it != _children.end(); ++it)
    {
        Control* w = (*it);        

        // clip widget update rect
        const gfx::Rect cr = w->geometry();        

        gfx::Rect updateRect = cr.intersect(r);        

        if( updateRect.isNull() )
            continue;

        // paint widget rect
        gfx::Point updatePos = toControl( *w , updateRect.topLeft() );
        updateRect.setOrigin(updatePos);
         
        PaintEvent pev( *w, updateRect);
        w->processEvent(pev);
    }    
}

void Control::onPaintEvent(const PaintEvent& ev)
{    
    ControlBase::onPaintEvent(ev);

    onPaint( _graphics, ev.rect() );
}

void Control::onPaint(gfx::Graphics& g, const gfx::Rect& r)
{
}

void Control::onRelayoutRequest(Control&)
{
    relayout();
}

void Control::relayout()
{
    if(_parent)
        _parent->onRelayoutRequest(*this);
}

gfx::Size Control::preferredSize() const
{
    return _preferredSize;
}

void Control::updateSize()
{
    setSize(preferredSize());
}

gfx::Size Control::measure(const gfx::Size& parentSize)
{
    _preferredSize = onMeasure(parentSize);
    return _preferredSize;
}

gfx::Size Control::onMeasure(const gfx::Size& parentSize)
{
    for( size_t i = 0; i < children().size(); ++i)
        children()[i]->measure(parentSize);

   return size();
}

void Control::onProcessLayoutEvent(const LayoutEvent& ev)
{
    const gfx::Rect& r = geometry();
    gfx::Rect rect =r;

    LayoutEvent lev(*this);

    lev.setRect(rect);

    onLayoutEvent(lev);
    
    std::vector<Control*>::const_iterator it;
    for(it = _children.begin() ; it != _children.end(); ++it)
    {
        Control* widget = (*it);

        LayoutEvent ev( *widget, widget->geometry() );
        widget->processEvent(ev);
    }

}

void Control::onLayoutEvent(const LayoutEvent& ev)
{
    onLayout( ev.rect() );
}

void Control::onLayout(const gfx::Rect& rect)
{
    for (size_t i = 0; i < children().size(); ++i)
        children()[i]->setSize(children()[i]->preferredSize());

    Pt::Any a(rect);
    _layoutSignal.send(*this, a);
}


void Control::onRequestMove(const gfx::Point& pos)
{

    if(_parent)
        _parent->onMoveRequest(*this, pos);
    else
        ControlBase::onRequestMove(pos);
}

void Control::onMoveRequest(Control& widget, const gfx::Point& pos)
{
    MoveEvent mev(widget, pos);
    widget.onProcessMoveEvent(mev);
}

void Control::onProcessMoveEvent(const MoveEvent& ev)
{
    ControlBase::onProcessMoveEvent(ev);
}

void Control::onMoveEvent(const MoveEvent& ev)
{
    _graphics.move(ev.position());    

    View::onMoveEvent(ev);
}

void Control::onSetSizeLimits(const gfx::Size& minSize, const gfx::Size& maxSize)
{
    ControlBase::onSetSizeLimits(minSize, maxSize);
    relayout();
}

void Control::onRequestResize(const gfx::Size& size)
{   
    _preferredSize = size;

    if(_parent)
        _parent->onResizeRequest(*this, size);
    else
        ControlBase::onRequestResize(size);
}

void Control::onResizeRequest(Control& widget, const gfx::Size& size)
{
    ResizeEvent rev(widget, size);

    widget.onProcessResizeEvent(rev);
}

void Control::onProcessResizeEvent(const ResizeEvent& ev)
{
    ControlBase::onProcessResizeEvent(ev);
}

void Control::onResizeEvent(const ResizeEvent& ev)
{   
    _graphics.resize( ev.size() );
    View::onResizeEvent(ev);
}

bool Control::acceptsInput() const
{    
    if( ! isEnabled() )
        return false;

    if( ! isVisible() )
        return false;

    return true;
}

void Control::onRequestShow(bool isShown)
{
    if( _show == isShown)
        return;

    _show = isShown;

    if(_parent)
        _parent->onShowRequest(*this, isShown);
}

void Control::onProcessShowEvent(const ShowEvent& ev)
{
    ControlBase::onProcessShowEvent(ev);
}

void Control::onShowEvent(const ShowEvent& ev)
{
    ControlBase::onShowEvent(ev);

    // parent should only layout visible children
    relayout();
}

void Control::onShow(bool isShown)
{
    ControlBase::onShow(isShown);
}

void Control::onShowRequest(Control& widget, bool visible)
{
    ShowEvent sev(widget, visible);
    widget.processEvent(sev);
}

void Control::onRequestEnable(bool isEnable)
{
    _enabled = isEnable;

    if(_parent)
        _parent->onEnableRequest(*this, isEnable);
}

void Control::onProcessEnableEvent(const EnableEvent& ev)
{
    bool isEnabled = ev.enabled();
    if( ! _enabled )
        isEnabled = false;

    EnableEvent eev(*this, isEnabled);
    ControlBase::onProcessEnableEvent(eev);

    for( size_t i = 0; i < _children.size(); ++i)
    {
        Control* w = _children[i];
        
        EnableEvent widgetEvent(*w, isEnabled);
        w->processEvent(widgetEvent);
    }
}

void Control::onEnableEvent(const EnableEvent& ev)
{
    ControlBase::onEnableEvent(ev);

    // TODO: move to derived classes
    invalidate();
}

void Control::onEnable(bool e)
{
    ControlBase::onEnable(e);
}

void Control::onEnableRequest(Control& widget, bool enable)
{
    if( ! isEnabled() )
      enable = false;

    EnableEvent eev(widget, enable);
    widget.processEvent(eev);
}

void Control::onRequestActivate(bool active)
{
    if(_parent)
        _parent->onActivateRequest(*this, active);
}

void Control::onActivateRequest(Control& w, bool active)
{
    if(_parent)
        _parent->onActivateRequest(w, active);
}

void Control::raise()
{
    if(_parent)
        _parent->onRaiseRequest(*this);
}

void Control::onRaiseRequest(Control& w)
{
    std::vector<Control*>::iterator it = std::find(_children.begin(), 
                                                  _children.end(), &w);
    if( it == _children.end() )
        return;

    _children.erase(it);
    _children.push_back(&w);

    w.repaint( w.bounds() );
}

const Spacing& Control::margin() const
{
    return _margin;
}

void Control::setMargin(const Spacing& s)
{  
    _margin.set( s.left(), s.top() , s.right() , s.bottom() );

    Pt::Any a(_margin);
    _marginChanged.send(*this, a);   
    relayout();
}

void Control::setMargin(int n)
{
    setMargin( Spacing(n) );
}

void Control::setMargin(int horiz, int vertical)
{
    setMargin( Spacing(horiz, vertical) );
}

const Spacing& Control::padding() const
{
    return _padding;
}

void Control::setPadding( const Spacing& p )
{   
    _padding.set( p.left() , p.top() , p.right() , p.bottom()  );

    Pt::Any a(_padding);
    _paddingChanged.send(*this, a);
    relayout();
}

void Control::setPadding(int n)
{
    setPadding( Spacing(n) );
}

void Control::setPadding(int horiz, int vertical)
{
    setPadding( Spacing(horiz, vertical) );
}

base::Object* Control::objectAt(Pt::uint32_t index) const
{
    return (Object*)childAt(index);
}

void Control::addObject(Pt::uint32_t index, base::Object* w)
{
    if (w)
        add(*(Control*)w);
}

uint32_t Control::onChildrenCount() const
{
    return _children.size();
}

void Control::bringToBack(Control& w)
{
    auto it = std::find(_children.begin(), _children.end(), &w);
    if (it == _children.end())
        return;

    _children.erase(it);

    _children.insert(_children.begin(), &w);    
}

void Control::bringToFront(Control& w)
{ 
    auto it = std::find(_children.begin(), _children.end(), &w);

    if (it == _children.end())
        return;

    _children.erase(it);
    _children.push_back(&w);    
}


void Control::onUpdateScaling()
{
    ControlBase::onUpdateScaling();

    _margin.set(_margin.left(),
                _margin.top() ,
                _margin.right() ,
                _margin.bottom() );

    _padding.set(_padding.left(),
                 _padding.top() ,
                 _padding.right(),
                 _padding.bottom());

    for(size_t i = 0; i < _children.size(); ++i)
        _children[i]->updateScaling();  
}



Control* Control::onChildAt(size_t n)
{
    return _children[n];    
}

const Control* Control::onChildAt(size_t n) const
{
    return _children[n];
}

// Element
Element* Control::onHitTest(const gfx::Point& p, HitTestHint hint)
{
    if (!isVisible())
        return nullptr;

    if( ! bounds().contains(p) )
        return nullptr;

    std::vector<Control*>::reverse_iterator it;
    for(it = _children.rbegin(); it != _children.rend(); ++it)
    {
        Control* w = *it;

        gfx::Point pos = toControl(*w, p);
        Element* hit = w->hitTest(pos, hint);
        if(hit)
            return hit;
    }

    return this;
}

gfx::Point Control::onToParent(const gfx::Point& pos) const
{
    if( ! _parent )
        return pos;

    return _parent->onFromControl(*this, pos);
}

gfx::Point Control::onFromParent(const gfx::Point& pos) const
{
    if( ! _parent )
        return pos;

    return _parent->onToControl(*this, pos);
}

bool Control::onProcessEvent(const Pt::Event& ev)
{
    return ControlBase::onProcessEvent(ev);
}

bool Control::onProcessMouseEvent(const MouseEvent& ev)
{
    if( ! acceptsInput() )
        return false;

    if( ControlBase::onProcessMouseEvent(ev))
        return true;
        
    
    // hit test
    gfx::Point pos = fromGlobal( ev.position() );

    std::vector<Control*>::reverse_iterator it;
    for(it = _children.rbegin(); it != _children.rend(); ++it)
    {
        Control* widget = *it;

        if( widget->geometry().contains(pos) && 
            widget->acceptsInput() )
        {
            if(widget->processEvent(ev))
                return true;
        }
    }   

    return false;
}

bool Control::onMouseEvent(const MouseEvent& ev)
{
    bool r = ControlBase::onMouseEvent(ev);

    if( ev.isPress(MouseEvent::Left) )
        focus();

    return r;
}

bool Control::onProcessTouchEvent(const TouchEvent& ev)
{
    if( ! acceptsInput() )
        return false;

    // hit test
    gfx::Point pos = fromGlobal( ev.position() );

    std::vector<Control*>::reverse_iterator it;
    for(it = _children.rbegin(); it != _children.rend(); ++it)
    {
        Control* widget = *it;

        if( widget->geometry().contains(pos) && 
            widget->acceptsInput() )
        {
            return widget->processEvent(ev);
        }
    }

    //
    // start capture on press
    //
    if( ev.isPress() )
    {
    }

    return ControlBase::onProcessTouchEvent(ev);
}

bool Control::onTouchEvent(const TouchEvent& ev)
{
    ControlBase::onTouchEvent(ev);

    if( ev.isPress() )
    {
        focus();
    }

    return false;
}

bool Control::onProcessScrollEvent(const ScrollEvent& ev)
{
    if( ! acceptsInput() )
        return false;

    if(ControlBase::onProcessScrollEvent(ev))
        return true;

    //Process to children
    std::vector<Control*>::reverse_iterator it;
    for (it = _children.rbegin(); it != _children.rend(); ++it)
    {
        Control* widget = *it;

        if (widget->processEvent(ev))
            return true;
    }

    return false;
}

bool Control::onScrollEvent(const ScrollEvent& ev)
{
    return ControlBase::onScrollEvent(ev);
}

void Control::onProcessEnterEvent(const EnterEvent& ev)
{
    ControlBase::onProcessEnterEvent(ev);
}

bool Control::onEnterEvent( const EnterEvent& ev)
{
    if (!_toolTip.empty())
        Application::instance().toolTip().start(*this);   

    return ControlBase::onEnterEvent(ev);
}

void Control::onProcessLeaveEvent(const LeaveEvent& ev)
{
    Application::instance().toolTip().stop(*this);

    ControlBase::onProcessLeaveEvent(ev);
}

bool Control::onLeaveEvent(const LeaveEvent& ev)
{
    return ControlBase::onLeaveEvent(ev);
}

bool Control::onProcessKeyEvent(const KeyEvent& ev)
{
    if( ! acceptsInput() )
        return false;

    return ControlBase::onProcessKeyEvent(ev);
}

bool Control::onKeyEvent(const KeyEvent& ev)
{
    if( ev.key() == actionKey() && hasFocus() )
    {
        onActionKey(ev);
        return true;
    }

    return ControlBase::onKeyEvent(ev);
}


}}}

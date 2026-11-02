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
#include <wildcat/gui/window/WindowBase.h>
#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/LayoutEvent.h>

namespace {

bool lowerFocusIndex(wildcat::gui::window::Control* a, wildcat::gui::window::Control* b)
{
    return a->focusIndex() < b->focusIndex();
}

}

namespace wildcat {
namespace gui {
namespace window {

WindowBase::WindowBase()
: _mainWidget(0)
, _layouts(0)
, _active(0)
, _focusWidget(0)
, _scalePosition(true)
{
    setAcceptScrollEvent(true);

    eventReceived() += Pt::slot(*this, &WindowBase::onProcessLayoutEvent);
    
    registerProperty("Content", *this, &WindowBase::contentI, &WindowBase::setContentI);
}

WindowBase::~WindowBase()
{
    if(_mainWidget)
        _mainWidget->unparent();
}

Control* WindowBase::content()
{
    return _mainWidget;
}

const Control* WindowBase::content()  const
{
    return _mainWidget;
}

void WindowBase::setContent(Control* widget)
{
    if(_mainWidget)
    {
        _mainWidget->unparent();
    }

    if(widget)
    {
        widget->setParent(*this);
    }
}

gfx::Graphics& WindowBase::graphics()
{
    return _graphics;
}

const gfx::Graphics& WindowBase::graphics() const
{
    return _graphics;
}

void WindowBase::relayout()
{
    _layouts++;

    LayoutEvent ev( *this, bounds() );
    Application::instance().loop().commitEvent(ev);
}

void WindowBase::onProcessLayoutEvent(const LayoutEvent& ev)
{
    if(_layouts == 0)
    {
        return;
    }

    --_layouts;

    if(_layouts > 0)
    {
        //std::clog << "RELAYOUT EVENT " << " deferred" << std::endl;
        return;
    }

    //std::clog << "RELAYOUT EVENT" << std::endl;

    if(_mainWidget)
    {
        //
        // 1. Pass
        //  
        onMeasure();

        //
        // 2. Pass layout position and size of contents
        //
        gfx::Rect rect( size() );
        LayoutEvent lev(*this, rect);
        onLayoutEvent(lev);

    }
}

gfx::Size WindowBase::onMeasure()
{    
    return _mainWidget ? _mainWidget->measure(size()) : size();
}

void WindowBase::onLayoutEvent(const LayoutEvent& ev)
{   
    if( _mainWidget )
    {        
        const gfx::Point widgetPos(_mainWidget->margin().left(), _mainWidget->margin().top());

        gfx::Size widgetSize(size().width() - _mainWidget->margin().leftRight(),
                                          size().height()- _mainWidget->margin().topBottom());

        _mainWidget->setPosition(widgetPos);
        _mainWidget->setSize(widgetSize);

        _mainWidget->processEvent(ev);
        
    }
}

Control* WindowBase::focusWidget()
{
    return _focusWidget;
}

void WindowBase::focusPrev()
{
    moveFocus(_focusList.rbegin(), _focusList.rend());
}

void WindowBase::focusNext()
{
    moveFocus(_focusList.begin(), _focusList.end());
}

template <typename Iter>
void WindowBase::moveFocus(Iter begin, Iter end)
{
    Iter current = std::find(begin, end, _focusWidget);
    Iter it = current;
    
    if( it != end )
        ++it;
    else
        it = begin;

    while(it != current)
    {
        if( it == end )
            it = begin;

        Control* w = *it;
        
        if( w->focusPolicy() != Control::NoFocus )
        {
            onSetFocus(*w);
            return;
        }

        ++it;
    }
}

void WindowBase::onSetFocusPolicy(Control& w, FocusPolicy policy)
{
    if( _focusWidget == &w && policy == View::NoFocus )
    {
        FocusEvent fev(*_focusWidget, false);
        _focusWidget->processEvent(fev);
        _focusWidget = nullptr;
    }
}

void WindowBase::onSetFocusIndex(Control& w, unsigned index)
{
    std::sort(_focusList.begin(), _focusList.end(), &lowerFocusIndex);
}

void WindowBase::onSetFocus(Control& widget)
{
    if( _focusWidget == &widget )
        return;

    if(_focusWidget)
    {
        if(_focusWidget->focusPolicy() == Control::KeepFocus)
            return;
    }

    if(_focusWidget)
    {
        Window* imeWindow = Application::instance().inputMethod().activeWindow();
        if(imeWindow)
        {
          if( ! _focusWidget->isDescendantOf(*imeWindow) )
              Application::instance().inputMethod().finish();
        }

        FocusEvent fev(*_focusWidget, false);
        _focusWidget->processEvent(fev);
    }
    
    _focusWidget = &widget;

    if(_focusWidget)
    {
        FocusEvent fev(*_focusWidget, true);
         _focusWidget->processEvent(fev);
    }

    repaint();
}

void WindowBase::onSetShortcut(Control& w, const std::vector<Key>& keys)
{
    std::map<Key, Control*>::iterator it = _shortcuts.begin();

    while( it != _shortcuts.end() )
    {
        if(it->second == &w)
            _shortcuts.erase(it++);
        else
            ++it;
    }

    for(size_t i = 0; i < keys.size();++i)
        _shortcuts[keys[i]] = &w;
}

void WindowBase::onSetMnemonic(Control& w, const std::vector<Pt::Char>& chs)
{
    std::map<Pt::Char, Control*>::iterator it = _mnemonics.begin();

    while( it != _mnemonics.end() )
    {
        if(it->second == &w)
            _mnemonics.erase(it++);
        else
            ++it;
    }

    for( size_t i = 0; i < chs.size(); ++i)
        _mnemonics[std::tolower(chs[i])] = &w;
}

void WindowBase::onAddElement(Control& widget)
{
    
    // focus handling    
    if( _focusList.empty() )
        widget.setFocusIndex(0);
    else
        widget.setFocusIndex( _focusList.back()->focusIndex() + 1);

    _focusList.push_back(&widget);

    onSetShortcut( widget, widget.onGetShortcuts() );
    onSetMnemonic( widget, widget.onGetMnemonics() );
}

void WindowBase::onRemoveElement(Control& widget)
{
    if(_active == &widget)
        _active = nullptr;
    
    // focus handling    
    if( _focusWidget == &widget )
    {
        FocusEvent fev(*_focusWidget, false);
        _focusWidget->processEvent(fev);
        _focusWidget = 0;
    }

    std::vector<Control*>::iterator it;
    it = std::find(_focusList.begin(), _focusList.end(), &widget);

    if( it != _focusList.end() )
        _focusList.erase(it);

    onSetShortcut(widget, std::vector<gui::window::Key>());
    onSetMnemonic(widget, std::vector<Pt::Char>());
}

// View

void WindowBase::onAttach(Control& widget)
{
    _mainWidget = &widget;
    
    relayout();
}

void WindowBase::onSetResourceManager(base::ResourceManager& v)
{
    View::onSetResourceManager(v);  

    if (_mainWidget)
        _mainWidget->setResourceManager(v);
}

void WindowBase::onDetach(Control& widget)
{
  if(_active == &widget)
      _active = 0;

    if(_mainWidget == &widget)
        _mainWidget = 0;

    relayout();
}

void WindowBase::onInit(Control& widget)
{
    gfx::Point surfacePos = widget.position();

    widget.setGraphics(&_graphics, surfacePos);

    widget.setForm(this);
}

void WindowBase::onRelease(Control& widget)
{
    widget.setForm(0);
    widget.setGraphics( 0, widget.position() );
}

gfx::Point WindowBase::onToControl(const Control& widget, const gfx::Point& pos) const
{
    return pos - widget.position();   
}

gfx::Point WindowBase::onFromControl(const Control& widget, const gfx::Point& pos) const
{
    return pos + widget.position();
}

void WindowBase::onRaiseRequest(Control& widget)
{
}

//
// Element
//

Element* WindowBase::onHitTest(const gfx::Point& p, HitTestHint hint)
{
    if( ! bounds().contains(p) )
        return 0;

    if(_mainWidget)
    {
        gfx::Point pos = toControl(*_mainWidget, p);
        Element* hit = _mainWidget->hitTest(pos, hint);
        if(hit)
            return hit;
    }

    return this;
}


void WindowBase::onUpdateScaling()
{
    Base::onUpdateScaling();

    if(_mainWidget)
        _mainWidget->updateScaling();   
}

bool WindowBase::onProcessEvent(const Pt::Event& ev)
{
    return Base::onProcessEvent(ev);
}

//
// invalidation
//

void WindowBase::onInvalidateEvent(const InvalidateEvent& ev)
{
    Base::onInvalidateEvent(ev);
}

void WindowBase::onInvalidate()
{
    _colors.load(); 
    Base::onInvalidate();
    
    
    relayout();
}

//
// painting
//

void WindowBase::onProcessPaintEvent(const PaintEvent& ev)
{    
    const gfx::Rect& rect = ev.rect();

    if( rect.isNull() )
        return;

    Base::onProcessPaintEvent(ev);

    //
    // paint main widget
    //
    if(_mainWidget)
    {
        gfx::Rect updateRect = _mainWidget->geometry().intersect(rect);
        if( updateRect.isNull() )
            return;
        
        gfx::Point updatePos = onToControl( *_mainWidget, updateRect.topLeft() );
        updateRect.setOrigin(updatePos);


        PaintEvent pev( *_mainWidget, updateRect );
        _mainWidget->processEvent(pev);        

    }
}

void WindowBase::onPaintEvent(const PaintEvent& ev)
{
    Base::onPaintEvent(ev);
}

void WindowBase::onRepaintRequest(Control& w, const gfx::Rect& rect)
{
    gfx::Point widgetPos = onFromControl( w, rect.topLeft() );
    gfx::Rect widgetRect( widgetPos, rect.size() );

    repaint(widgetRect);
}

void WindowBase::onRelayoutRequest(Control& widget)
{
    relayout();
}


//
// enable
//

void WindowBase::onProcessEnableEvent(const EnableEvent& ev)
{
    Base::onProcessEnableEvent(ev);
}

void WindowBase::onEnableEvent(const EnableEvent& ev)
{    
    Base::onEnableEvent(ev);
}

void WindowBase::onSetEnabledState(bool v)
{
    Base::onSetEnabledState(v);

    if( _mainWidget)
        _mainWidget->setEnabledState(v);    
}   

void WindowBase::onEnable(bool e)
{
    Base::onEnable(e);
}

void WindowBase::onEnableRequest(Control& widget, bool enable)
{
    if( ! isEnabled() )
      enable = false;

    EnableEvent eev(widget, enable);
    widget.processEvent(eev);
}

//
// activation
//

void WindowBase::onActivateRequest(Control& widget, bool active)
{
    if(active)
        _active = &widget;

    if( ! active && _active == &widget )
        _active = 0;
}

//
// visibility
//

void WindowBase::onProcessShowEvent(const ShowEvent& ev)
{
    Base::onProcessShowEvent(ev);
}

void WindowBase::onShowEvent(const ShowEvent& ev)
{
    Base::onShowEvent(ev);
}

void WindowBase::onShow(bool visible)
{
    Base::onShow(visible);
}

void WindowBase::onShowRequest(Control& widget, bool isShown)
{
    ShowEvent sev(widget, isShown);
    widget.processEvent(sev);
}

//
// geometry
//

void WindowBase::onMoveRequest(Control& widget, const gfx::Point& pos)
{    
    MoveEvent mev(widget, pos);
//    onProcessMoveEvent(mev);
    Application::instance().commitEvent(mev);
}

void WindowBase::onProcessMoveEvent(const MoveEvent& ev)
{
    Base::onProcessMoveEvent(ev);
}

void WindowBase::onMoveEvent(const MoveEvent& ev)
{
    Base::onMoveEvent(ev);
}

void WindowBase::onResizeRequest(Control& widget, const gfx::Size& size)
{ 
    ResizeEvent rev(widget, size);
    widget.processEvent(rev);
}

void WindowBase::onProcessResizeEvent(const ResizeEvent& ev)
{
    Base::onProcessResizeEvent(ev);
}

void WindowBase::onResizeEvent(const ResizeEvent& ev)
{
    _graphics.resize( ev.size() );

    Base::onResizeEvent(ev);

    relayout();
}



//
// input capture
//

void WindowBase::onRequestCapture(bool capture)
{
    Base::onRequestCapture(capture);
}

//
// input
//

bool WindowBase::onProcessMouseEvent(const MouseEvent& ev)
{
    gfx::Point pos = fromGlobal( ev.position() );           

    if (_active != nullptr && _active->parent() && ev.isAnyPress())
    {
        gfx::Point loc = _active->parent()->fromGlobal(ev.position());

        if( !_active->geometry().contains(loc))
            _active->activate(false);        
    }

    if(_mainWidget &&  _mainWidget->geometry().contains(pos) &&  _mainWidget->acceptsInput() )
    {    
        if (ev.isAnyPress())
        {
            int z = 0;
            ++z;
        }

        if( _mainWidget->processEvent(ev))
            return true;
    }

    return Base::onProcessMouseEvent(ev);
}

bool WindowBase::onMouseEvent(const MouseEvent& ev)
{
    return Base::onMouseEvent(ev);
}

bool WindowBase::onProcessTouchEvent(const TouchEvent& ev)
{     
    gfx::Point pos = fromGlobal( ev.position() );

    if(_mainWidget && 
       _mainWidget->geometry().contains(pos) && 
       _mainWidget->acceptsInput() )
    {
        return _mainWidget->processEvent(ev);
    }

    //
    // process event
    //    
    return Base::onProcessTouchEvent(ev);
}

bool WindowBase::onTouchEvent(const TouchEvent& ev)
{
    return Base::onTouchEvent(ev);
}

bool WindowBase::onProcessScrollEvent(const ScrollEvent& ev)
{
    if(Base::onProcessScrollEvent(ev))
        return true;

    if (_active)
    {
        if (_active->processEvent(ev))
            return true;
    }

    if (_focusWidget)
    {
        if(_focusWidget->processEvent(ev))
            return true;
    }

    return false;
}

bool WindowBase::onScrollEvent(const ScrollEvent& ev)
{
    return Base::onScrollEvent(ev);
}

void WindowBase::onProcessEnterEvent(const EnterEvent& ev)
{
    Base::onProcessEnterEvent(ev);
}

bool WindowBase::onEnterEvent(const EnterEvent& ev)
{
    //std::clog << "ENTER Form: " << name() << " " << vid() << std::endl;
    return Base::onEnterEvent(ev);
}

void WindowBase::onProcessLeaveEvent(const LeaveEvent& ev)
{
    Base::onProcessLeaveEvent(ev);
}

bool WindowBase::onLeaveEvent(const LeaveEvent& ev )
{
    //std::clog << "LEAVE Form: " << name()  << " " << vid() << std::endl;
    return Base::onLeaveEvent(ev);
}

bool WindowBase::onProcessKeyEvent(const KeyEvent& ev)
{
    // Priorty 1: me
    if (Base::onProcessKeyEvent(ev))
        return true;

    // Priorty 2: shortcut 
    if (ev.isPress())
    {
        std::map<Key, Control*>::iterator s = _shortcuts.find(ev.key());

        if (s != _shortcuts.end() && s->second->acceptsInput())
        {
            if(s->second->processShortcut(s->first))
                return true;
        }
    }

    // Priorty 3: active widget     
    if (_active)
    {
        if(_active->processEvent(ev))
            return true;
    }

    // Priorty 4: mnemonic 
    if (ev.isPress() && ev.key().modifiers() == Key::Alt)
    {
        std::map<Pt::Char, Control*>::iterator m = _mnemonics.find(ev.unicode());
        if (m != _mnemonics.end())
        {
            m->second->processMnemonic(m->first);
            return true;
        }
    }

    // Priorty 5: focus     
    if (ev.key().code() == Key::Tab && ev.isPress())
    {
        bool keepFocus = _focusWidget && _focusWidget->focusPolicy() == KeepFocus;
        if (!keepFocus)
        {
            if (ev.key().modifiers() == Key::Shift)
                focusPrev();
            else
                focusNext();

            return true;
        }
    }
    
    // Priority 6: focused widget
    if (_focusWidget)
    {
        if (_focusWidget->processEvent(ev))
            return true;
    }

    return true;
}

bool WindowBase::onKeyEvent(const KeyEvent& ev)
{
    return Base::onKeyEvent(ev);
}

}}} 

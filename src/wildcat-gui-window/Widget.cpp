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
#include <wildcat/gui/window/Widget.h>
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/Window.h>

namespace wildcat {
namespace gui {
namespace window {

Widget::Widget()
: WidgetBase()
, _doubleClickState(3)
, _focusChanged()
, _beginDrag()
, _draging()
, _drop()
, _font(Application::inst().font())
, _isHighlighted(false)
, _detectDrag(false)
, _isDraging(false)
, _corner(3)
, _colors()
, _hasBackground(false)
, _hasFrame(false)
, _roundedFrame(false)
, _errorState(false)
{

    setName("Widget");


    registerSignal("FocusChanged", _focusChanged, "Cat=Widget");
    registerSignal("BeginDrag", _beginDrag, "Cat=Widget");
    registerSignal("Draging", _draging, "Cat=Widget");
    registerSignal("Drop", _drop, "Cat=Widget");
    registerSignal("DoubleClick", _doubleClickSignal, "Cat=Widget");
    registerSignal("SlowDoubleClick", _slowDoubleClickSignal, "Cat=Widget");
    registerSignal("ControlColorChanged", _controlColorChanged, "Cat=Widget");
    registerSignal("CsontrolDarkColorChanged", _controlDarkColorChanged, "Cat=Widget");
    registerSignal("ControlDarkDarkColorChanged", _controlDarkDarkColorChanged, "Cat=Widget");
    registerSignal("ControlLightColorChanged", _controlLightColorChanged, "Cat=Widget");
    registerSignal("ControlLightLightColorChanged", _controlLightLightColorChanged, "Cat=Widget");
    registerSignal("ControlDisabledColorChanged", _controlDisabledColorChanged, "Cat=Widget");
    registerSignal("HighlightColorChanged", _highlightColorChanged, "Cat=Widget");
    registerSignal("FrameColorChanged", _frameColorChanged, "Cat=Widget");
    registerSignal("FrameDisabledColorChanged", _frameDisabledColorChanged, "Cat=Widget");
    registerSignal("InfoColorChanged", _infoColorChanged, "Cat=Widget");
    registerSignal("InfoTextColorChanged", _infoTextColorChanged, "Cat=Widget");
    registerSignal("TextColorChanged", _textColorChanged, "Cat=Widget");
    registerSignal("TextHighlightColorChanged", _textHighlightColorChanged, "Cat=Widget");
    registerSignal("TextBackgroundColorChanged", _textBackgroundColorChanged, "Cat=Widget");
    registerSignal("TextDisabledColorChanged", _textDisabledColorChanged, "Cat=Widget") ;
    registerSignal("HasBackgroundChanged", _hasBackgroundChanged, "Cat=Widget");
    registerSignal("HasFrameChanged", _hasFrameChanged, "Cat=Widget");
    registerSignal("RoundedFrameChanged", _roundedFrameChanged, "Cat=Widget");
    registerSignal("FontChanged", _fontChanged, "Cat=Widget");
    registerSignal("DetectDragChanged", _detectDragChanged, "Cat=Widget")   ;


    registerProperty("ControlColor",*this,  &Widget::controlColor, &Widget::setControlColor, "Cat=Apparence;Editor=Color;");
    registerProperty("ControlDarkColor", *this, &Widget::controlDarkColor, &Widget::setControlDarkColor, "Cat=Apparence;Editor=Color");
    registerProperty("ControlDarkDarkColor", *this, &Widget::controlDarkDarkColor, &Widget::setControlDarkDarkColor, "Cat=Apparence;Editor=Color");
    registerProperty("ControlLightColor", *this, &Widget::controlLightColor, &Widget::setControlLightColor, "Cat=Apparence;Editor=Color");
    registerProperty("ControlLightLightColor", *this, &Widget::controlLightLightColor, &Widget::setControlLightLightColor, "Cat=Apparence;Editor=Color");
    registerProperty("ControlDisabledColor", *this, &Widget::controlDisabledColor, &Widget::setControlDisabledColor, "Cat=Apparence;Editor=Color");
    registerProperty("HighlightColor", *this, &Widget::highlightColor, &Widget::setHighlightColor, "Cat=Apparence;Editor=Color");
    registerProperty("FrameColor", *this, &Widget::frameColor, &Widget::setFrameColor, "Cat=Apparence;Editor=Color");
    registerProperty("FrameDisabledColor", *this, &Widget::frameDisabledColor, &Widget::setFrameDisabledColor, "Cat=Apparence;Editor=Color");
    registerProperty("InfoColor", *this, &Widget::infoColor, &Widget::setInfoColor, "Cat=Apparence;Editor=Color");
    registerProperty("InfoTextColor", *this, &Widget::infoTextColor, &Widget::setInfoTextColor, "Cat=Apparence;Editor=Color");
    registerProperty("TextColor", *this, &Widget::textColor, &Widget::setTextColor, "Cat=Apparence;Editor=Color");
    registerProperty("TextHighlightColor", *this, &Widget::textHighlightColor, &Widget::setTextHighlightColor, "Cat=Apparence;Editor=Color");
    registerProperty("TextBackgroundColor", *this, &Widget::textBackgroundColor, &Widget::setTextBackgroundColor, "Cat=Apparence;Editor=Color");
    registerProperty("TextDisabledColor", *this, &Widget::textDisabledColor, &Widget::setTextDisabledColor, "Cat=Apparence;Editor=Color");
    registerProperty("Font", *this, &Widget::font, &Widget::setFont, "Cat=Apparence;Editor=Font");
    registerProperty("HasBackground", *this, &Widget::hasBackground, &Widget::setHasBackground, "Cat=Apparence;Editor=Check");
    registerProperty("HasFrame", *this, &Widget::hasFrame, &Widget::setHasFrame, "Cat=Apparence;Editor=Check");
    registerProperty("RoundedFrame", *this, &Widget::roundedFrame, &Widget::setRoundedFrame, "Cat=Apparence;Editor=Check");

}

Widget::~Widget()
{
}



void Widget::setErrorState(bool v, const Pt::String& message)
{
    if( _errorState == v)
        return; 

    _errorState = v;

    if( _errorState)
    {
        _errorMessage = message;
        _toolTipCached = toolTip();
        setToolTip(_errorMessage);

        Application::inst().textCursorSignal() += Pt::slot(*this, &Widget::onErrorStateChanged);
    }
    else
    {
        _errorMessage = "";
        setToolTip(_toolTipCached);
        Application::inst().textCursorSignal() -= Pt::slot(*this, &Widget::onErrorStateChanged);
    }

    invalidate();
}
    

bool Widget::isHighlighted() const
{
    return _isHighlighted;
}

void Widget::onInvalidate()
{
    _colors.setPalleteId(getColorSchemeId());
    _colors.load();
    WidgetBase::onInvalidate();
}

void Widget::onPaint(gfx::Graphics& graphics, const gfx::Rect& rect)
{    

}

void Widget::onLayout(const gfx::Rect& rect)
{
     WidgetBase::onLayout(rect);
     repaint();
}

void Widget::onPaintEvent(const PaintEvent& ev)
{        
    WidgetBase::onPaintEvent(ev);
}

void Widget::onMoveEvent(const MoveEvent& ev)
{
    WidgetBase::onMoveEvent(ev);
}

void Widget::onResizeEvent(const ResizeEvent& ev)
{
    WidgetBase::onResizeEvent(ev);
}


void Widget::onErrorStateChanged()
{
    gfx::ArgbColor c = _colors.aControlFrame();

    if( c == gfx::ArgbColor(255,0, 0))
        c = _colors.controlFrame().getColor();
    else
        c = gfx::ArgbColor(255, 0, 0);

    _colors.setAControlFrame(c);
    _colors.setAControl(c);
    _colors.setATextBackground(c);
    repaint();
}

gfx::Size Widget::onMeasure(const gfx::Size& parentSize)
{
    return WidgetBase::onMeasure(parentSize);
}

void Widget::onFocusEvent(const FocusEvent& ev)
{
    WidgetBase::onFocusEvent(ev);

    Pt::Any a(&ev);
    _focusChanged.send(*this, a);
}

bool Widget::onEnterEvent( const EnterEvent& ev)
{
    WidgetBase::onEnterEvent(ev);

    _isHighlighted = true;
    
    repaint();
    return true;
}

bool Widget::onDoubleClickEvent(const gui::window::MouseEvent& ev)
{
    bool r = false;
    EventSignal evSig(r, ev);

    Pt::Any a(&evSig);
    _doubleClickSignal.send(*this, a);
    return r;
}

bool Widget::onDoubleClickSlowEvent(const gui::window::MouseEvent& ev)
{
    bool r = false;
    EventSignal evSig(r, ev);

    Pt::Any a(&evSig);
    _slowDoubleClickSignal.send(*this, a);
    return r;
}

bool Widget::onMouseEvent(const gui::window::MouseEvent& ev)
{        
    if (_detectDrag)
    {
        _drag.process(ev.position(), ev.isPress(), ev.isReleased());

        if (_drag.isDrag())
        {            
            if (!_isDraging)
            {
                _isDraging = true;
                onBeginDrag(_drag.current());
            }
            else
            {
                onDraging(toGlobal(_drag.current()));
            }
        }

        if (_drag.isReleased())
        {
            _isDraging = false;
            onDrop(toGlobal(_drag.current()));
            _drag.reset();         
        }
    }
    else
    {
        _drag.reset();
    }

    if (ev.isRelease(gui::window::MouseEvent::Left))
    {
        if (evalDoubleClick(ev, _doubleClickState[0]))
            return true;
    }

    if (ev.isRelease(gui::window::MouseEvent::Middle))
    {
        if (evalDoubleClick(ev, _doubleClickState[2]))
            return true;
    }

    if (ev.isRelease(gui::window::MouseEvent::Right))
    {
        if (evalDoubleClick(ev, _doubleClickState[2]))
            return true;
    }

    return WidgetBase::onMouseEvent(ev);
}

void Widget::setFont(const gfx::Font& v)
{
    onSetFont(v);
}

const gfx::Font& Widget::font() const
{
    return onGetFont();
}

bool Widget::evalDoubleClick(const gui::window::MouseEvent& ev, DoubleClickInfo& info)
{
    if (!info.running)
    {
        info.clock.start();
        info.running = true;
    }
    else
    {
        Pt::Timespan sp = info.clock.stop();
        info.running = false;

        if (sp.toMSecs() < Application::inst().doubleClickSpeed())
        {
            return onDoubleClickEvent(ev);
        }
        else if (sp.toMSecs() >= Application::inst().doubleClickSpeed() &&
            sp.toMSecs() < Application::inst().doubleClickSpeedSlow())
        {
            return onDoubleClickSlowEvent(ev);
        }
        else
        {
            info.clock.start();
            info.running = true;
        }
    }

    return false;
}

bool Widget::onLeaveEvent(const LeaveEvent& ev)
{
    WidgetBase::onLeaveEvent(ev);

    _isHighlighted = false;

    repaint();
    return true;
}

}}}

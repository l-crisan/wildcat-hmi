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
#include "FrameBuffer.h"
#include "ApplicationImpl.h"
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/Cursor.h>
#include <wildcat/gui/window/PaintEvent.h>
#include <wildcat/gui/window/ResizeEvent.h>
#include <wildcat/gfx/Graphics.h>
#include <Pt/System/Clock.h>
#include <Pt/System/Logger.h>
#include <algorithm>
#include <cmath>
#include <cstring>

PT_LOG_DEFINE("wildcat.gui.window.Screen")

namespace wildcat {
namespace gui {
namespace window {

class CursorRepaintEvent : public Pt::BasicEvent<CursorRepaintEvent>    
{
    friend class Pt::BasicEvent<CursorRepaintEvent>;

    public:
        CursorRepaintEvent(const gfx::Rect& rect)
         :_rect(rect)    
        {

        }

        virtual ~CursorRepaintEvent()
        {

        }

        const gfx::Rect& rect() const 
        { 
            return _rect; 
        }

    private:

        gfx::Rect _rect;            
};

     
ScreenImpl::ScreenImpl(ApplicationImpl& app)
: _frameBuffer( app.frameBuffer() )
, _parent(0)
, _dpi(96.0)
{   
    Application::instance().loop().eventReceived() += Pt::slot(*this, &ScreenImpl::onCursorUpdate);

    setName("FBScreenImpl");

    _graphics.reset( _frameBuffer.size(),  _frameBuffer.strideSize()); 

    gfx::Rect rect( gfx::Point(0, 0), _graphics.size() );

    _graphics.setBrush( SystemColors::workingspace());
    _graphics.fillRect(rect);

    ScreenImplBase::setGraphics(&_graphics, gfx::Point(0, 0) );    

    repaint( gfx::Rect(gfx::Point(0, 0), _frameBuffer.size()) );    
}

ScreenImpl::~ScreenImpl()
{
    ScreenImplBase::setGraphics(nullptr, gfx::Point(0, 0));
}


void ScreenImpl::onCursorUpdate(const CursorRepaintEvent& ev)
{
    const gfx::Rect& rect = ev.rect();

    if( rect.isNull() )
        return;

    output(rect);
}

void ScreenImpl::setParent(Screen* screen)
{
    _parent = screen;

    if(_parent)
    {
        gfx::Size fs = _frameBuffer.size();
        gfx::Size size( fs.width(), fs.height() );

        size /= scaleFactor();

        _parent->onResize(*this, size);
        _parent->onShow(*this, true);
    }

    onSetParent(_parent);
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
    return ScreenImplBase::windows();
}

WindowManagerBase& ScreenImpl::windowManager()
{
    return *this;
}

Element* ScreenImpl::onHitTest(const gfx::Point& p, HitTestHint hint)
{
    if( ! bounds().contains(p) )
        return 0;

    Element* hit = ScreenImplBase::onHitTest(p, hint);

    if(hit)
        return hit;

    return 0;
}

gfx::Point ScreenImpl::onToParent(const gfx::Point& pos) const
{
    if( ! _parent )
        return pos;

    return _parent->toParent(pos);
}

gfx::Point ScreenImpl::onFromParent(const gfx::Point& pos) const
{
    if( ! _parent )
        return pos;

    return _parent->fromParent(pos);
}

void ScreenImpl::onRequestRepaint(const gfx::Rect& rect)
{
    if(_parent)
        _parent->repaint(rect);
}

void ScreenImpl::setCapture(Element* capture)
{
}

void ScreenImpl::onProcessEnableEvent(const EnableEvent& ev)
{
    ScreenImplBase::onProcessEnableEvent(ev);
}

void ScreenImpl::onEnable(bool e)
{
    Element::onEnable(e);
}

bool ScreenImpl::onProcessMouseEvent(const MouseEvent& ev)
{
    return ScreenImplBase::onProcessMouseEvent(ev);
}

bool ScreenImpl::onMouseEvent(const MouseEvent& ev)
{ 
    return ScreenImplBase::onMouseEvent(ev);
}

bool ScreenImpl::onProcessTouchEvent(const TouchEvent& ev)
{ 
    return ScreenImplBase::onProcessTouchEvent(ev);
}

bool ScreenImpl::onTouchEvent(const TouchEvent& ev)
{ 
    return ScreenImplBase::onTouchEvent(ev);
}

bool ScreenImpl::onProcessScrollEvent(const ScrollEvent& ev)
{
    if( ! isEnabled() )
        return false;
  
    return ScreenImplBase::onProcessScrollEvent(ev);
}

bool ScreenImpl::onScrollEvent(const ScrollEvent& ev)
{
    return ScreenImplBase::onScrollEvent(ev);
}

bool ScreenImpl::onProcessKeyEvent(const KeyEvent& ev)
{
    if( ! isEnabled() )
        return false;
    
    return ScreenImplBase::onProcessKeyEvent(ev);
}

bool ScreenImpl::onKeyEvent(const KeyEvent& ev)
{
    return ScreenImplBase::onKeyEvent(ev);
}

void ScreenImpl::onSetScaleFactor(double v)
{
    ScreenImplBase::onSetScaleFactor(v);

    gfx::Size fs = _frameBuffer.size();
    gfx::Size size(fs.width(), fs.height());

    size /= scaleFactor();

    _graphics.resize(size);
    _graphics.setScaleFactor(scaleFactor());

    _graphics.reset(_frameBuffer.size(),
        _frameBuffer.strideSize());

    if (_parent)
        _parent->onResize(*this, size);
}

void ScreenImpl::onProcessPaintEvent(const PaintEvent& ev)
{
    const gfx::Rect& screenRect = ev.rect();

    if( screenRect.isNull() )
        return;
    
    ScreenImplBase::onProcessPaintEvent(ev);            

    output(ev.rect());
}

void ScreenImpl::onPaintEvent(const PaintEvent& ev)
{    
    ScreenImplBase::onPaintEvent(ev);

    const gfx::Rect& rect = ev.rect();

    onPaint(_graphics, rect);
}

void ScreenImpl::onPaint(gfx::Graphics& painter, const gfx::Rect& rect)
{    
    painter.setCompositionMode(gfx::CompositionMode::SourceCopy);
    painter.setBrush(SystemColors::desktop());
    painter.fillRect(rect);  
}

const gfx::Pixmap& ScreenImpl::image() const
{
    return _graphics.image();    
}

gfx::Pixmap& ScreenImpl::image()
{
    return _graphics.image();
}

void ScreenImpl::setCursorPosition(const gfx::Point& pos)
{        
    const Cursor& cursor = Application::instance().impl()->cursor();

    if( cursor.empty() )
    {
        _cursorRect.clear();
        return;       
    }

    const gfx::Point curPos(pos.x() - cursor.xHotspot(), pos.y() - cursor.yHotspot());
    const gfx::Rect  curRect(curPos, gfx::Size(cursor.width(), cursor.height()));

    _cursorRect = curRect;

    CursorRepaintEvent ev(_cursorRect); 
    Application::instance().loop().commitEvent(ev);   
}

void ScreenImpl::output(const gfx::Rect& rect)
{
    //Output the current  rect.
    _frameBuffer.output(image().data(), rect);
    
    //Clear the old cursor rect.
    if (rect != _oldCursorRect)
     _frameBuffer.output(image().data(), _oldCursorRect);    

    //Clear the new cursor rect.
    if (rect != _cursorRect)
     _frameBuffer.output(image().data(), _cursorRect);   

    //Draw the new cursor.   
    drawCursor();
}

void ScreenImpl::drawCursor()
{    
    const Cursor& cursor = Application::instance().impl()->cursor();

    if( cursor.empty() )    
    {
        _cursorBackBuffer.reset();
        _oldCursorRect.clear();
        return;      
    }
    
    //Back buffer to build the transparent cursor ops
    _cursorBackBuffer = image().toImage(_cursorRect);
        
    assert(_cursorBackBuffer.size() == _cursorRect.size());

    bitBlitCursor(&cursor.andRgb888()[0], _cursorRect.size(), AndOp);
    
    bitBlitCursor(&cursor.xorRgb888()[0], _cursorRect.size(), XorOp);
    
   _frameBuffer.outputAt(_cursorBackBuffer.data(), _cursorRect);
   
   _oldCursorRect = _cursorRect;
}


void ScreenImpl::bitBlitCursor(const uint8_t* cursor, const gfx::Size& size, BlitOp op)
{
    const size_t lineSize = size.width() * 4;

    for (int  y = 0; y < size.height(); ++y)
    {
        const size_t lineOffset = y * lineSize; 

        for (int x = 0; x < size.width(); ++x)
        {
            const size_t pixelOffset = lineOffset + (x * 4);
            
            const uint32_t* src = (uint32_t*)&cursor[pixelOffset];  

            uint32_t* dest = (uint32_t*)&_cursorBackBuffer.data()[pixelOffset];
            
            if(op == AndOp)
                *dest &= *src;
            else                
                *dest ^= *src;                            
        }  
    }
}

}}}

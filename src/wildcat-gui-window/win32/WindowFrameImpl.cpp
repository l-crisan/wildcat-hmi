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
#include "WindowFrameImpl.h"
#include "ScreenImpl.h"

#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/WindowManagerBase.h>
#include <wildcat/gui/window/WindowStateEvent.h>
#include <wildcat/gui/gfx/PngWriter.h>
#include <Pt/Math.h>
#include <cassert>
#include <fstream>
#include <dwmapi.h>

namespace wildcat {
namespace gui {
namespace window {

 WindowFrameImpl::WindowFrameImpl(ScreenImpl& wm, Window& w)
: WindowFrameBase(wm, w)
, _wm(wm)
, _window(w)
, _hwnd(0)
, _iconHandle(0)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
  
    LONG style = 0;
    LONG exStyle = 0;
    
    switch( w.frameType() )
    {
        case WindowType::Borderless:
            style = WS_POPUP;
            exStyle = WS_EX_NOACTIVATE;
            break;

        default:

        case WindowType::Default:
            style = WS_OVERLAPPEDWINDOW;
            exStyle = WS_EX_APPWINDOW;
        break;

        case WindowType::Fixed:
          style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
          exStyle = WS_EX_APPWINDOW;
        break;
    }

    const char* className = w.closeButtonVisible() ? "wildcat-hmi" : "wildcat-hmi-no-close";

    _hwnd = CreateWindowEx(exStyle, className, "", style,
                           0, 0, 10, 10, GetDesktopWindow(), 
                           NULL, hInstance, NULL);


    if (_hwnd) {
        // 20 is the attribute value for DWMWA_USE_IMMERSIVE_DARK_MODE
        BOOL useDarkMode = Application::inst().isSystemDarkMode();
        DwmSetWindowAttribute(_hwnd, 20, &useDarkMode, sizeof(useDarkMode));
    }

    Base::onSetParent(&wm);
    Application::inst().setCurrentWindow(&_window);
}


 WindowFrameImpl::~WindowFrameImpl()
{
    if(Application::inst().currentWindow() == &_window)
        Application::inst().setCurrentWindow(nullptr);
        
    DestroyWindow( _hwnd);

    if(_iconHandle != 0)
        DestroyIcon(_iconHandle);
}


 gfx::Point WindowFrameImpl::onClientOffset() const
 {
     RECT rect;
    GetClientRect(_hwnd, &rect);
    RECT wrect;
    GetWindowRect(_hwnd, &wrect);
    gfx::Point offset(wrect.left - rect.left, wrect.top - rect.top);
    return offset;
 }

void WindowFrameImpl::onInit(Window& w)
{               
}


void WindowFrameImpl::onRelease(Window& w)
{
    
}


void WindowFrameImpl::setType(WindowType type)
{
    LONG style = GetWindowLong(_hwnd, GWL_STYLE);
    LONG exStyle = GetWindowLong(_hwnd, GWL_EXSTYLE);
    
    RECT wrect;
    GetWindowRect(_hwnd, &wrect);

    RECT rect;
    GetClientRect(_hwnd, &rect);

    switch(type)
    {
        case WindowType::Borderless:
            style = WS_POPUP;
            exStyle = WS_EX_NOACTIVATE;
        break;

        default:
        case WindowType::Default:
            style = WS_OVERLAPPEDWINDOW;
            exStyle = WS_EX_APPWINDOW;
        break;

        case WindowType::Fixed:
            style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
            exStyle = WS_EX_APPWINDOW;
        break;
    }

    // changing the border also changes the window area
    AdjustWindowRectEx(&rect, style, FALSE, exStyle);

    LONG w = rect.right - rect.left;
    LONG h = rect.bottom - rect.top;
    LONG x = wrect.left;
    LONG y = wrect.top;

    SetWindowLong(_hwnd, GWL_STYLE, style);
    SetWindowLong(_hwnd, GWL_EXSTYLE, exStyle);
    
    SetWindowPos(_hwnd, 0, x, y, w, h,
                 SWP_FRAMECHANGED|
                 SWP_NOZORDER|SWP_NOOWNERZORDER);
}


gfx::Point WindowFrameImpl::toScreen(const gfx::Point& pos) const
{
    gfx::Point physicalPos = _window.graphics().toPhysical(pos);
  
    POINT p = { lround(physicalPos.x()), 
                lround(physicalPos.y()) };

    ClientToScreen(_hwnd, &p);

    gfx::Point screenPos(p.x, p.y);
    gfx::Point logicalPos = _window.graphics().toLogical(screenPos);
    return logicalPos;
}


gfx::Point WindowFrameImpl::fromScreen(const gfx::Point& pos) const
{
    gfx::Point physicalPos = _window.graphics().toPhysical(pos);

    POINT p = { lround(physicalPos.x()), 
                lround(physicalPos.y()) };

    ScreenToClient(_hwnd, &p);

    gfx::Point clientPos(p.x, p.y);
    gfx::Point logicalPos = _window.graphics().toLogical(clientPos);
    return logicalPos;
}


gfx::Point WindowFrameImpl::onToWindow(const Window& w,
                                       const gfx::Point& pos) const
{
    return pos;
}


gfx::Point WindowFrameImpl::onFromWindow(const Window& w,
                                         const gfx::Point& pos) const
{
    return pos;
}


gfx::Point WindowFrameImpl::onToParent(const gfx::Point& pos) const
{
    return _wm.fromFrame(*this, pos); 
}
     
        
gfx::Point WindowFrameImpl::onFromParent(const gfx::Point& pos) const
{ 
    return _wm.toFrame(*this, pos); 
}


void WindowFrameImpl::paint(const gfx::Rect& rect)
{
    RECT wRect;
    wRect.left   = lround( rect.left() );
    wRect.top    = lround( rect.top() );
    wRect.right  = lround( rect.right() + 1 );
    wRect.bottom = lround( rect.bottom() + 1 );
    
    InvalidateRect(_hwnd, &wRect, FALSE);
}


void WindowFrameImpl::onRepaint(Window& w, const gfx::Rect& rect)
{
    gfx::Point screenPos = toScreen(rect.topLeft());
    gfx::Rect screenRect( screenPos, rect.size() );

    _wm.repaint(screenRect);
}


void WindowFrameImpl::onProcessPaintEvent(const PaintEvent& ev)
{
    PaintEvent rev( _window, ev.rect() );
    _window.processEvent(rev);

    Base::onProcessPaintEvent(ev);
}


void WindowFrameImpl::onPaintEvent(const PaintEvent& ev)
{
    Base::onPaintEvent(ev);

    const double sc = Application::inst().screen().scaleFactor();
    gfx::Rect updateRect = ev.rect();
    updateRect.scale(sc);


    PAINTSTRUCT ps;
    HDC windowContext = BeginPaint(_hwnd, &ps);

    const gfx::Pixmap& image = _window.graphics().image();

    const size_t depth = image.pixelStride() * 8;
    const uint8_t* data = image.data();

    HBITMAP Pixmap = CreateBitmap(image.width(), image.height(), 1, depth, (VOID*)data);

    if (Pixmap == NULL)
    {
        BITMAPINFO bitmapInfo;
        ZeroMemory(&bitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER));

        bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bitmapInfo.bmiHeader.biWidth = image.width();
        bitmapInfo.bmiHeader.biHeight = -(Pt::ssize_t)image.height();  // top-down image
        bitmapInfo.bmiHeader.biPlanes = 1;                         // always 1            
        bitmapInfo.bmiHeader.biBitCount = depth;                   // 32-bit 
        bitmapInfo.bmiHeader.biCompression = BI_RGB;               // uncompressed RGB
        bitmapInfo.bmiHeader.biSizeImage = 0;                      // automatic
        bitmapInfo.bmiHeader.biClrUsed = 0;                        // no color table
        bitmapInfo.bmiHeader.biClrImportant = 0;                   // no color table

        VOID* imageBits = 0;
        Pixmap = CreateDIBSection(windowContext, &bitmapInfo,
                                  DIB_RGB_COLORS, &imageBits, NULL, 0);

        memcpy(imageBits, data, image.width() * image.height() * 4);
    }

    HDC PixmapDC = CreateCompatibleDC(NULL);
    SelectObject(PixmapDC, Pixmap);

    BitBlt(windowContext, updateRect.x(), updateRect.y(), 
           updateRect.width(), updateRect.height(), 
           PixmapDC, updateRect.x(),  updateRect.y(), SRCCOPY);

    DeleteDC(PixmapDC);
    DeleteObject(Pixmap);

    EndPaint(_hwnd, &ps);
}


void WindowFrameImpl::onShow(Window& w, bool visible)
{
    if(visible)
    {
        w.enable(true);

        LONG style = GetWindowLong(_hwnd, GWL_EXSTYLE);

        if(style & WS_EX_NOACTIVATE)
          ShowWindow(_hwnd, SW_SHOWNOACTIVATE);
        else
          ShowWindow(_hwnd, SW_SHOW);
    }
    else
    {
        ShowWindow(_hwnd, SW_HIDE);
    }
}


void WindowFrameImpl::onShowSystemMenu(Window& w, bool v)
{
    LONG_PTR style = GetWindowLongPtr(_hwnd, GWL_STYLE);

    if(v)
        style |= WS_SYSMENU;
    else
        style &= ~WS_SYSMENU;

    SetWindowLongPtr(_hwnd, GWL_STYLE, style);
}

void WindowFrameImpl::onShowCloseButton(Window& w, bool v)
{
    // the class style (CS_NOCLOSE) is shared by all windows of the class and
    // is fixed at creation; toggle the close item of this window's menu instead
    HMENU sysMenu = GetSystemMenu(_hwnd, FALSE);
    if( ! sysMenu )
        return;

    EnableMenuItem(sysMenu, SC_CLOSE, MF_BYCOMMAND | (v ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));

    // redraw the non-client area so the caption button reflects the new state
    SetWindowPos(_hwnd, 0, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
}

void WindowFrameImpl::onShowMinimizeButton(Window& w, bool v)
{
    if( v) 
        SetWindowLong(_hwnd, GWL_STYLE, GetWindowLong(_hwnd, GWL_STYLE) | WS_MINIMIZEBOX);    
    else    
        SetWindowLong(_hwnd, GWL_STYLE, GetWindowLong(_hwnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
}

void WindowFrameImpl::onShowMaximizeButton(Window& w, bool v)
{
    if (v)    
        SetWindowLong(_hwnd, GWL_STYLE, GetWindowLong(_hwnd, GWL_STYLE) | WS_MAXIMIZEBOX);    
    else    
        SetWindowLong(_hwnd, GWL_STYLE, GetWindowLong(_hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);            
}



void WindowFrameImpl::onProcessShowEvent(const ShowEvent& ev)
{
    Base::onProcessShowEvent(ev);

    ShowEvent rev( _window, ev.visible() );
    _window.processEvent(rev);
}


void WindowFrameImpl::onShowEvent(const ShowEvent& ev)
{
    Base::onShowEvent(ev);
}


void WindowFrameImpl::onActivate(Window& w, bool active)
{
    if( ! active )
        return;
    
    SetActiveWindow(_hwnd);

    Application::inst().setCurrentWindow(&_window);
}


void WindowFrameImpl::onProcessActivateEvent(const ActivateEvent& ev)
{
    Base::onProcessActivateEvent(ev);

    ActivateEvent aev( _window, ev.isActive() );
    _window.processEvent(aev);
}


void WindowFrameImpl::onActivateEvent(const ActivateEvent& ev)
{
    Base::onActivateEvent(ev);
}


void WindowFrameImpl::onEnable(Window& w, bool enable)
{
    EnableWindow(_hwnd, enable);
}


void WindowFrameImpl::onProcessEnableEvent(const EnableEvent& ev)
{
    Base::onProcessEnableEvent(ev);

    EnableEvent eev( _window, ev.enabled() );
    _window.processEvent(eev);
}


void WindowFrameImpl::onEnableEvent(const EnableEvent& ev)
{    
    Base::onEnableEvent(ev);
}


void WindowFrameImpl::onMove(Window& w, const gfx::Point& pos)
{
    const double  sc = Application::inst().screen().scaleFactor();
    gfx::Point p = pos;
    

    if (w.scalePosition())
        p.scale(sc);


    SetWindowPos(_hwnd, 0, p.x(), p.y(), 0, 0, 
                 SWP_DRAWFRAME|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
}


void WindowFrameImpl::onProcessMoveEvent(const MoveEvent& ev)
{
    Base::onProcessMoveEvent(ev);

    MoveEvent mev( _window, ev.position() );
    _window.processEvent(mev);
}


void WindowFrameImpl::onSetAbove(Window& w, bool above)
{

    HWND insertBelow = above ? HWND_TOPMOST : HWND_NOTOPMOST;
    
    SetWindowPos(_hwnd, insertBelow, 0, 0, 0, 0, 
                 SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

}


void WindowFrameImpl::onSetState(Window& w, const WindowState& state)
{
    LONG style = GetWindowLong(_hwnd, GWL_STYLE);

    switch(state)
    {
        case WindowState::Normal:
        break;

        case WindowState::Maximized:
            style |= WS_MAXIMIZE;
        break;

        case WindowState::Minimized:
            style |= WS_MINIMIZE;
        break;
    }

    SetWindowLong(_hwnd, GWL_STYLE, style); 
}


void WindowFrameImpl::onProcessWindowStateEvent(const WindowStateEvent& ev)
{
    Base::onProcessWindowStateEvent(ev);

    WindowStateEvent wse( _window, ev.state() );
    Application::instance().processEvent(wse);
}


void WindowFrameImpl::onWindowStateEvent(const WindowStateEvent& ev)
{
}


void WindowFrameImpl::onSetTitle(Window& w, const Pt::String& text)
{
    SetWindowText( _hwnd, text.narrow().c_str() );
}


void WindowFrameImpl::onSetIcon(Window& w, uint32_t iconId)
{
    const gfx::Icon* icon = Application::instance().resourceMng().get<gfx::Icon>(iconId);
    
    if (_iconHandle != 0)
    {
        DestroyIcon(_iconHandle);
        _iconHandle = 0;
    }

    if (icon == nullptr)
        return;


    gfx::Pixmap iconImage = icon->getImage(Application::inst().screen().scaleFactor());   

    if(iconImage.empty())
    {
        SendMessage(_hwnd, WM_SETICON, ICON_SMALL, 0);
        return;
    }

    const size_t planes = 4;
    std::vector<Pt::uint8_t> PixmapBuffer(iconImage.width() * iconImage.height() * planes);
        
    for(size_t y = 0; y < iconImage.height(); ++y)
    {
        const size_t offsetLine = y * (iconImage.width() * planes);     
        for(size_t x = 0; x < iconImage.width(); ++x)
        {
          const size_t index = offsetLine + (x*planes);

          const gfx::Point pixel( x, y);
          const gfx::Color color = iconImage.color(pixel);
                
          PixmapBuffer[index]     = static_cast<unsigned char>(color.blue());    
          PixmapBuffer[index + 1] = static_cast<unsigned char>(color.green());
          PixmapBuffer[index + 2] = static_cast<unsigned char>(color.red());
          PixmapBuffer[index + 3] = static_cast<unsigned char>(color.alpha());
        }        
    }

    HINSTANCE hInstance = GetModuleHandle(NULL);
    _iconHandle = ::CreateIcon(hInstance, iconImage.width(), iconImage.height(),
                               4, 8, 0, (BYTE*)&PixmapBuffer[0]);

    SendMessage(_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)_iconHandle);    
}


void WindowFrameImpl::onSetSizeLimits(Window& w, const gfx::Size& minSize,
                                                const gfx::Size& maxSize)
{
}


void WindowFrameImpl::onResize(Window& w, const gfx::Size& s)
{
    //
    // align to physical pixel grid
    //
    gfx::Size alignedSize = s;

    //
    // maximum width and height
    //
    if( alignedSize.width() > w.maximumSize().width() )
        alignedSize.setWidth( w.maximumSize().width() );

    if( alignedSize.height() > w.maximumSize().height() )
        alignedSize.setHeight( w.maximumSize().height() );

    if( alignedSize.width() < w.minimumSize().width() )
        alignedSize.setWidth( w.minimumSize().width() );

    if( alignedSize.height() < w.minimumSize().height() )
        alignedSize.setHeight( w.minimumSize().height() );

    const double sc = Application::inst().screen().scaleFactor();

    alignedSize.scale(sc);

    
    const gfx::Size size = alignedSize;

    RECT clientRect;
    SetRect(&clientRect, 0, 0, lround(size.width()) - 1, 
                               lround(size.height()) - 1);

    LONG style = GetWindowLong(_hwnd, GWL_STYLE);
    LONG exStyle = GetWindowLong(_hwnd, GWL_EXSTYLE);

    AdjustWindowRectEx(&clientRect, style, FALSE, exStyle);
    
    LONG clientWidth  = clientRect.right  - clientRect.left + 1;
    LONG clientHeight = clientRect.bottom - clientRect.top  + 1;
    SetWindowPos(_hwnd, NULL, 0, 0, clientWidth, clientHeight, 
                 SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
}


void WindowFrameImpl::onProcessResizeEvent(const ResizeEvent& ev)
{
    Base::onProcessResizeEvent(ev);

    ResizeEvent rev( _window, ev.size() );
    _window.processEvent(rev);
}


void WindowFrameImpl::onClose(Window& w)
{
    PostMessage(_hwnd, WM_CLOSE, 0, 0);
    Application::inst().nextEvent();    
}


void WindowFrameImpl::onProcessCloseEvent(const CloseEvent& ev)
{
    onCloseEvent(ev);

    CloseEvent cev(_window);
    _window.processEvent(cev);
}


void WindowFrameImpl::onCloseEvent(const CloseEvent& ev)
{
}

}} }

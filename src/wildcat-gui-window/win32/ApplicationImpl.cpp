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
#include "ApplicationImpl.h"
#include "ScreenImpl.h"
#include "WindowFrameImpl.h"
#include "KeyMap.h"
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/Screen.h>
#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/PaintEvent.h>
#include <wildcat/gui/window/ResizeEvent.h>
#include <wildcat/gui/window/MoveEvent.h>
#include <wildcat/gui/window/CloseEvent.h>
#include <wildcat/gui/window/ActivateEvent.h>
#include <wildcat/gui/window/WindowStateEvent.h>
#include <Pt/System/IOError.h>
#include <Pt/System/Selectable.h>
#include <Pt/String.h>
#include <Pt/Types.h>
using std::max;
using std::min;
#include <WindowsX.h>
#include <Gdiplus.h>
#include <cmath>

#pragma comment (lib, "gdiplus.lib")

namespace {

HBITMAP createImage888(const Pt::uint8_t* data, size_t width, size_t height)
{
    HDC hDC        = ::GetDC(NULL);
    HDC hMainDC    = ::CreateCompatibleDC(hDC); 

    BITMAPINFO bitmapInfo;
    ZeroMemory(&bitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER));

    bitmapInfo.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER); // Size of this struct.
    bitmapInfo.bmiHeader.biWidth       = width;             // Pixmap width.
    bitmapInfo.bmiHeader.biHeight      = -(Pt::ssize_t)height;  // Pixmap height. Negative value = top-down image.
    bitmapInfo.bmiHeader.biPlanes      = 1;                 // Always 1.
    bitmapInfo.bmiHeader.biBitCount    = 32;                // We internally use a 32-bit Pixmap.
    bitmapInfo.bmiHeader.biCompression = BI_RGB;            // Uncompressed (top-down) RGB Pixmap.
    bitmapInfo.bmiHeader.biSizeImage   = 0;                 // 0 = automatic for BI_RGB-images.
    bitmapInfo.bmiHeader.biClrUsed     = 0;                 // 0 = No color table.
    bitmapInfo.bmiHeader.biClrImportant= 0;                 // 0 = No color table.

    VOID* imageBits;
    HBITMAP Pixmap = CreateDIBSection(hMainDC, &bitmapInfo, DIB_RGB_COLORS, &imageBits, NULL, 0);

    memcpy(imageBits, data, width * height * 3);

    ::DeleteDC(hMainDC);
    ::ReleaseDC(NULL,hDC);

    return Pixmap;
}

}

namespace wildcat {

namespace gui {
namespace window {

/////////////////////////////////////////////////////////////////////////////
// Selector
/////////////////////////////////////////////////////////////////////////////

Selector::Selector()
{
}

Selector::~Selector()
{
}

DWORD Selector::waitFor(DWORD numHandles, const HANDLE *handles, 
                        DWORD msecs, bool& isTimeout)
{    
    DWORD result = MsgWaitForMultipleObjects(numHandles, (HANDLE*)handles, 
                                             FALSE, msecs, QS_ALLINPUT);

    if(result == WAIT_FAILED)
        throw Pt::System::IOError("WaitForMultipleObjects");

    if( result == WAIT_TIMEOUT)
    {
        isTimeout = true;
        return 0;
    }

    DWORD offset = result - WAIT_OBJECT_0;

    if(offset == numHandles)
    {
        MSG msg;
        while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }    
    }

    return offset;
}

/////////////////////////////////////////////////////////////////////////////
// ApplicationImpl
/////////////////////////////////////////////////////////////////////////////

ApplicationImpl::ApplicationImpl()
: Pt::System::EventLoop()
, _instanceHandle(NULL)
, _gdiplusToken(0)
, _pointerWindow(NULL)
, _defaultCursorHandle(0)
, _cursorHandle(0)
{
#ifdef NDEBUG  
    FreeConsole();
#endif

    // SetProcessDpiAwareness

    BOOL isDpiAware = SetProcessDPIAware();
    //std::clog << "DPI AWARE: " << isDpiAware << std::endl;

    _instanceHandle = (HINSTANCE) GetModuleHandle(NULL);

    Gdiplus::GdiplusStartupInput startupInput;
    Gdiplus::GdiplusStartup(&_gdiplusToken, &startupInput, NULL);

    WNDCLASS winClass;
    winClass.style         = CS_HREDRAW | CS_VREDRAW;
    winClass.lpfnWndProc   = (WNDPROC)ApplicationImpl::wndProc;
    winClass.cbClsExtra    = 0;
    winClass.cbWndExtra    = 0;
    winClass.hInstance     = _instanceHandle;
    winClass.hIcon         = NULL;
    winClass.hCursor       = NULL;
    winClass.hbrBackground = NULL;
    winClass.lpszMenuName  = NULL;
    winClass.lpszClassName = "wildcat-hmi";
    RegisterClass(&winClass);

    // separate class whose window procedure handles WM_CLOSE on its own
    WNDCLASS closeClass = winClass;
    closeClass.lpfnWndProc   = (WNDPROC)ApplicationImpl::closeWndProc;
    closeClass.lpszClassName = "wildcat-hmi-close";
    RegisterClass(&closeClass);

    // separate class without close button; CS_NOCLOSE is a class style and
    // must not be toggled on the shared class via SetClassLong
    WNDCLASS noCloseClass = winClass;
    noCloseClass.style        |= CS_NOCLOSE;
    noCloseClass.lpszClassName = "wildcat-hmi-no-close";
    RegisterClass(&noCloseClass);

    _defaultCursorHandle = LoadCursor(NULL, IDC_ARROW);

    gfx::PixmapGraphics::setFontDir(Pt::System::Path("C:\\Windows\\Fonts\\"));
 }


ApplicationImpl::~ApplicationImpl()
{
    if( _cursorHandle != 0 )
       DestroyCursor( _cursorHandle );

    DestroyCursor(_defaultCursorHandle);

    Gdiplus::GdiplusShutdown(_gdiplusToken);

    UnregisterClass("wildcat-hmi", _instanceHandle);
    UnregisterClass("wildcat-hmi-close", _instanceHandle);
    UnregisterClass("wildcat-hmi-no-close", _instanceHandle);
}


void ApplicationImpl::setCursor(const Cursor* cursor)
{          
    if(_cursorHandle)
    {
        DestroyCursor(_cursorHandle);
        _cursorHandle = 0;
    }

    if (cursor == 0 || cursor->name() == "ArrowCursor")
    {
        SetCursor(_defaultCursorHandle);
        return;
    }

    if( cursor->empty() )
    {
        SetCursor(0);
        return;
    }

    HBITMAP andMask = createImage888( &cursor->andRgb888()[0], cursor->width(),  cursor->spanHeight() );
    HBITMAP xorMask = createImage888( &cursor->xorRgb888()[0], cursor->width(),  cursor->spanHeight() );

    ICONINFO iconInfo;
    iconInfo.fIcon = false; 
    iconInfo.xHotspot = cursor->xHotspot();
    iconInfo.yHotspot = cursor->yHotspot();
    iconInfo.hbmColor = xorMask;
    iconInfo.hbmMask  = andMask;

    _cursorHandle = CreateIconIndirect(&iconInfo);

    if( _cursorHandle != 0 )
      SetCursor( _cursorHandle );

    DeleteObject( andMask );
    DeleteObject( xorMask );
}


bool ApplicationImpl::isSystemDarkMode() const
{
    HKEY hKey;
    // Pfad zum Personalisierungs-Schlüssel in der Registry
    LPCWSTR subkey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";

    // Schlüssel öffnen
    if (RegOpenKeyExW(HKEY_CURRENT_USER, subkey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD value = 1; // Standardmäßig nehmen wir "Light" (1) an
        DWORD valueSize = sizeof(value);

        // Den Wert "AppsUseLightTheme" abfragen
        LSTATUS status = RegQueryValueExW(hKey, L"AppsUseLightTheme", nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &valueSize);
        RegCloseKey(hKey);

        if (status == ERROR_SUCCESS) {
            // Wenn AppsUseLightTheme == 0 ist, ist der Dark Mode aktiv
            return value == 0;
        }
    }

    // Fallback, falls der Schlüssel nicht existiert (z. B. auf älteren Windows-Versionen)
    return false;
}

void ApplicationImpl::setFontDir(const Pt::System::Path& dir)
{
    gfx::PixmapGraphics::setFontDir(dir);
}


void ApplicationImpl::setDefaultFont(const gfx::Font& fontName)
{
    gfx::PixmapGraphics::setDefaultFont(fontName);
}


Pt::Timespan ApplicationImpl::inactivityTime() const
{
	LASTINPUTINFO info = { 0 };
	info.cbSize = sizeof(info);
	GetLastInputInfo(&info);

  // GetTickCount() overflows like GetLastInputInfo() 
  DWORD ticks = GetTickCount();
  Pt::int64_t msecs = ticks - info.dwTime;
  
  return Pt::Timespan(msecs * 1000);
}	


void ApplicationImpl::sendKeyEvent(const KeyEvent& ev)
{
    HWND hwnd = GetActiveWindow();
    if( ! hwnd )
        return;

    Window* window = findWindow(hwnd);
    if( ! window )
        return;

    KeyEvent kev = ev;
    kev.setElement(window);
    commitEvent(kev);
}


void ApplicationImpl::sendMouseEvent(const MouseEvent& ev)
{
    POINT p = { ev.position().x(), 
                ev.position().y() };

    HWND h = WindowFromPoint(p);

    Window* w = findWindow(h);
    if( ! w )
        return;

    // screen to window coordinates
    gfx::Point pos = w->fromGlobal( ev.position() );

    MouseEvent mev = ev;
    mev.setPosition(pos);
    mev.setElement(w);
    
    //if( ! _pointerInWindow )
    //{
        //Application::instance().setPointerWindow(w);
        //_pointerInWindow = true;
    //}

    // TODO: call Application::processMouseEvent which returns true if the
    //       event was consumed. If it returns false and the event was not
    //       consumed call ApplicationImpl::dispatchMouseEvent


    Application::instance().processEvent(mev);
}


void ApplicationImpl::nextEvent()
{
    waitNext();
}


void ApplicationImpl::onAttachSelectable(Pt::System::Selectable& s)
{ 
    _selector.attach(s); 
}


void ApplicationImpl::onDetachSelectable(Pt::System::Selectable& s)
{ 
    _selector.detach(s); 
}


void ApplicationImpl::onCancel(Pt::System::Selectable& s)
{
    Pt::System::MutexLock lock(_mutex);

    std::vector<Pt::System::Selectable*>::iterator it = _avail.begin();
    while(it != _avail.end())
    {
        if(*it == &s)
            it = _avail.erase(it);
        else
            ++it;
    }
}


void ApplicationImpl::onReady(Pt::System::Selectable& s)
{
    Pt::System::MutexLock lock(_mutex);
    _avail.push_back(&s);
}


void ApplicationImpl::onRun()
{
    while( this->waitNext() )
        ;
}


void ApplicationImpl::onExit()
{
    _eventQueue.exit();
    wake();
}


void ApplicationImpl::onCommitEvent(const Pt::Event& ev)
{ 
    _eventQueue.pushEvent(ev); 
    wake();
}


void ApplicationImpl::onQueueEvent(const Pt::Event& ev)
{ 
    _eventQueue.pushEvent(ev); 
}


void ApplicationImpl::onWake()
{ 
    _selector.wake(); 
}


void ApplicationImpl::onProcessEvents()
{
    _eventQueue.processEvents( this->eventReceived() );
    //_selector.waitForWake(0);
}


void ApplicationImpl::onAttachTimer(Pt::System::Timer& timer)
{ 
    _timerQueue.addTimer(timer); 
}


void ApplicationImpl::onDetachTimer(Pt::System::Timer& timer )
{ 
    _timerQueue.removeTimer(timer); 
}


bool ApplicationImpl::waitNext()
{
    size_t timeout = _timerQueue.processTimers();

    // check all selectables that did not require waiting
    while( true )
    {
        Pt::System::MutexLock lock(_mutex);

        if( _avail.empty() )
            break;

        timeout = 0;
        Pt::System::Selectable* s = _avail.back();
        _avail.pop_back();
        lock.unlock();

        s->run();
    }

    bool isActive = true;
    
    if( _selector.waitForWake(timeout) )
        isActive = _eventQueue.processEvents( this->eventReceived() );

    return isActive;
}


long CALLBACK ApplicationImpl::wndProc(HWND hwnd, UINT msg, 
                                       WPARAM wparam, LPARAM lparam)
{
    gui::window::Application& app = gui::window::Application::instance();

    if(msg == WM_MOUSEACTIVATE)
    {
        Window* w = app.impl()->findWindow(hwnd);
        if(w && w->frameType() == WindowType::Borderless)
            return MA_NOACTIVATE;
    }

    bool handled = app.impl()->processMessage(hwnd, msg, wparam, lparam);
    if( ! handled )
        return DefWindowProc(hwnd, msg, wparam, lparam);

    if(msg == WM_ERASEBKGND)
        return TRUE;



    return handled ? 0 : 1;
}


long CALLBACK ApplicationImpl::closeWndProc(HWND hwnd, UINT msg, 
                                            WPARAM wparam, LPARAM lparam)
{
    if(msg == WM_CLOSE)
    {
        gui::window::Application& app = gui::window::Application::instance();

        Window* w = app.impl()->findWindow(hwnd);
        if( ! w )
            return DefWindowProc(hwnd, msg, wparam, lparam);

        // do not let DefWindowProc destroy the window, the application decides
        WindowFrameBase* frame = w->frame();
        CloseEvent ev(*frame);
        app.impl()->commitEvent(ev);
        return 0;
    }

    return ApplicationImpl::wndProc(hwnd, msg, wparam, lparam);
}


Window* ApplicationImpl::findWindow(HWND hwnd)
{
    const std::vector<Window*>& windows = Application::instance().screen().windows();

    for(size_t i = 0; i < windows.size(); ++i)
    {
        Window* w = windows[i];
        
        WindowFrameImpl* impl = static_cast<WindowFrameImpl*>( w->frame() );
        if( ! impl )
            continue;

        if( impl->hwnd() == hwnd )
            return w;
    }
    
    return 0;
}


bool ApplicationImpl::processMessage(HWND hwnd, UINT msg, 
                                     WPARAM wparam, LPARAM lparam)
{
    bool handled = false;

    Window* w = findWindow(hwnd);
    if( ! w)
        return handled;

    switch(msg)
    {
        case WM_MOUSEMOVE:
        {
          onMouse(*w, msg, wparam, lparam);
          handled = true;
          break;
        }
        
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        {          
           onMouse(*w, msg, wparam, lparam);
           handled = true;
           break;
        }
        
        case WM_MOUSEWHEEL:
        {
            int delta = GET_WHEEL_DELTA_WPARAM(wparam);

            ScrollEvent sev(*w);
            sev.set(ScrollEvent::Vertical, (delta/WHEEL_DELTA));

            //commitEvent(sev);
            Application::instance().processEvent(sev);
            
            handled = true;
            break;
        }
        
        case WM_KEYDOWN:
        case WM_KEYUP:    
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:    
        {
            UINT vkey = wparam;
            UINT scanCode = ((lparam >> 16) & 0xFF);
            bool isPress = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            //UINT repeatCount = lparam & 0xFFFF;

            onKey(*w, vkey, scanCode, isPress);
            handled = true;
            break;
        }

        case WM_SHOWWINDOW:
        {
            onShow(*w, wparam != 0);
            handled = true;
            break;
        }
        
        case WM_ERASEBKGND:
        {
            handled = true;
            break;
        }

        case WM_PAINT:
        {
            onPaint(*w, hwnd);
            handled = true;
            break;
        }
        
        case WM_SIZE:
        {
            onResize(*w, wparam, lparam);
            handled = true;
            break;
        }

        case WM_MOVE:
        {
            onMove(*w, hwnd, lparam);
            handled = true;
            break;
        }

        case WM_DESTROY:
        {                     
            handled = true;
            break;
        }
        
        case WM_CLOSE:
        {
            handled = onClose(*w);
            break;
        }

        case WM_ACTIVATE:
        {
            onActivate(*w, wparam != 0);
            handled = true;
            break;
        }
                
        case WM_ENABLE:
            onEnable(*w, wparam != 0);
            break;

        case WM_KILLFOCUS:
        {
            onActivate(*w,  false);
            handled = true;
            break;
        }        

        case WM_GETMINMAXINFO:
        {
            const double sc = Application::inst().screen().scaleFactor();
            int minWidth = std::round( w->minimumSize().width() * sc);
            int minHeight = std::round( w->minimumSize().height() * sc);
            int maxWidth = std::round( w->maximumSize().width() * sc);
            int maxHeight = std::round( w->maximumSize().height() * sc);
            MINMAXINFO* mmi = (MINMAXINFO*)lparam;
            mmi->ptMaxTrackSize.x = static_cast<LONG>(maxWidth);
            mmi->ptMaxTrackSize.y = static_cast<LONG>(maxHeight); 
            mmi->ptMinTrackSize.x = static_cast<LONG>(minWidth);
            mmi->ptMinTrackSize.y = static_cast<LONG>(minHeight);
            handled = true;  
            break;
        }

        case WM_MOUSELEAVE:            
            const Cursor* cur = Application::instance().resourceMng().get<Cursor>("ArrowCursor");

            Application::instance().setCursor(cur);    
            Application::instance().setPointed(nullptr);
            _pointerWindow = NULL;
           
            handled = true;  
            break;
    }

    return handled;
}


void ApplicationImpl::onShow(Window& w,  bool v)
{
    ShowEvent sev( *w.frame(), v);
    commitEvent(sev);

    // w.invalidate();
}


bool ApplicationImpl::onClose(Window& w)
{  
    WindowFrameBase* frame = w.frame();
    CloseEvent ev(*frame);
    commitEvent(ev);

    //w.close();
    return true;
}


void ApplicationImpl::onActivate(Window& w, bool a)
{
    ActivateEvent aev(*w.frame(), a);
    commitEvent(aev);
}


void ApplicationImpl::onEnable(Window& w, bool e)
{
    EnableEvent eev(*w.frame(), e);
    commitEvent( eev );

    w.invalidate();
}


void ApplicationImpl::onKey(Window& w, UINT vkey, UINT scanCode, bool isPress)
{
    BYTE keyboardState[256] = { 0 };
    GetKeyboardState(keyboardState);

    //std::clog << "KEY: " << std::hex << vkey << std::endl;   

    bool shift = (keyboardState[VK_SHIFT] & 0x80) == 0x80;
    bool control = (keyboardState[VK_CONTROL] & 0x80) == 0x80;
    bool alt = (keyboardState[VK_MENU] & 0x80) == 0x80;
    bool rwin = (keyboardState[VK_RWIN] & 0x80) == 0x80;
    bool lwin = (keyboardState[VK_LWIN] & 0x80) == 0x80;

    Key::Modifiers modifiers;
    if(shift)
        modifiers.add(Key::Shift);

    if(control)
        modifiers.add(Key::Control);

    if(alt)
        modifiers.add(Key::Alt);

    if(rwin || lwin)
        modifiers.add(Key::Meta);

    // remove control modifier for ToUnicode
    if(control)
        keyboardState[VK_CONTROL] = 0;

    wchar_t wc = 0;
    ToUnicode(vkey, scanCode, (BYTE*)keyboardState, &wc, 1, 0); 

    Pt::uint32_t keyCode = Key::NoKey;
    if(vkey < keyMapSize)
    {
        keyCode = keyMap[vkey];
        if(keyCode == 0)
        {
            Pt::Char ch = static_cast<Pt::uint32_t>(wc);
            keyCode = toupper(ch).value();
        }
    }

    Key key(modifiers, keyCode);

    if(isPress)
        _keyEvent.setPress(key, wc);
    else
        _keyEvent.setRelease(key, wc); 

    _keyEvent.setElement(&w);
    //commitEvent(_keyEvent);

    Application::instance().processEvent(_keyEvent);
}


void ApplicationImpl::onMouse(Window& w, unsigned int msg, WPARAM wparam, LPARAM lparam)
{    
    int xPos = GET_X_LPARAM(lparam); 
    int yPos = GET_Y_LPARAM(lparam); 
    
    //std::clog << "MOUSE: " << xPos << ", " << yPos << std::endl;

    switch(msg)
    {
        case WM_LBUTTONDOWN:
            _mouseEvent.setPress(MouseEvent::Left);
        break;
        
        case WM_LBUTTONUP:        
            _mouseEvent.setRelease(MouseEvent::Left);
        break;
                            
        case WM_MBUTTONDOWN:
            _mouseEvent.setPress(MouseEvent::Middle);
        break;
        
        case WM_MBUTTONUP:
            _mouseEvent.setRelease(MouseEvent::Middle);
        break;   
        
        case WM_RBUTTONDOWN:        
            _mouseEvent.setPress(MouseEvent::Right);
        break;
        
        case WM_RBUTTONUP:
            _mouseEvent.setRelease(MouseEvent::Right);
        break; 

        case WM_MOUSEMOVE:
            _mouseEvent.setMove();
        break;
    }
    
    const double sc = Application::inst().screen().scaleFactor();
    gfx::Point pos(xPos, yPos);
    pos.scale(1.0/sc);
        
    _mouseEvent.setPosition( w.toGlobal(pos) );
    _mouseEvent.setElement(&w);

    WindowFrameImpl* impl = static_cast<WindowFrameImpl*>( w.frame() );

    if( _pointerWindow != impl->hwnd() )
    {
        _pointerWindow = impl->hwnd();
                
        // enable WM_MOUSELEAVE
        TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, _pointerWindow, 0 };
        TrackMouseEvent(&tme);
    }

    Application::instance().processEvent(_mouseEvent);
}


void ApplicationImpl::onMove(Window& w, HWND hwnd, LPARAM lParam)
{ 
    RECT info;
    GetWindowRect(hwnd, &info);
    
    int x = info.left;
    int y = info.top;
    
    const double sc = Application::inst().screen().scaleFactor();

    gfx::Point pos(x, y);

    if( w.scalePosition())
        pos.scale(1.0/sc);
    
    MoveEvent ev( *w.frame(), pos );
    
    Application::instance().processEvent(ev);
}


void ApplicationImpl::onResize(Window& w, WPARAM wParam, LPARAM lParam)
{   
    Window::State wstate = w.state();

    switch(wParam)
    {
        case SIZE_MAXHIDE:
        case SIZE_MAXSHOW:
            break;

        case SIZE_MAXIMIZED:
            wstate = WindowState::Maximized;
            break;

        case SIZE_MINIMIZED:
            wstate = WindowState::Minimized;
            break;

        case SIZE_RESTORED:
            wstate = WindowState::Normal;
            break;

        default:
            break;
    }

    if(w.state() != wstate)
    {
        WindowStateEvent wse(*w.frame(), wstate);
        commitEvent(wse);
    }

    int width  = LOWORD(lParam);
    int height = HIWORD(lParam);

    const double sc = Application::inst().screen().scaleFactor();

    gfx::Size to(width,height);
    to.scale(1.0/sc);
    
    ResizeEvent rev(*w.frame(), to);
    Application::instance().processEvent(rev);

    gfx::Rect updateRect(gfx::Point(0, 0), to);
    w.repaint(updateRect);
}

static gfx::Rect fromLTRB(int left, int top, int right, int bottom)
{
    return gfx::Rect(gfx::Point(left, top), gfx::Size(right - left, bottom - top));
}

void ApplicationImpl::onPaint(Window& w, HWND hwnd)
{
    RECT updateRect;
    GetUpdateRect(hwnd, &updateRect, FALSE);

    gfx::Rect winRect = fromLTRB(updateRect.left, updateRect.top, updateRect.right, updateRect.bottom);

    const double sc = Application::inst().screen().scaleFactor();

    winRect.scale(1.0/sc);

    WindowFrameBase* frame = w.frame();

    PaintEvent ev(*frame, winRect);

    frame->processEvent(ev);

    //PAINTSTRUCT ps;
    //HDC windowContext = BeginPaint(hwnd, &ps);

    //WindowImpl* windowImpl = static_cast<WindowImpl*>( w.frame() );
    //HDC PixmapContext = windowImpl->surface().pixmapImpl()->deviceContext();
    //
    //BitBlt(windowContext, updateRect.left, updateRect.top, 
    //       updateRect.right - updateRect.left, updateRect.bottom - updateRect.top, 
    //       PixmapContext, updateRect.left,  updateRect.top, SRCCOPY);
    //
    //EndPaint(hwnd, &ps);
}

}}}

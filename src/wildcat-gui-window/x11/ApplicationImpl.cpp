#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/Screen.h>
#include <wildcat/gfx/Graphics.h>
#include <Pt/IOError.h>
#include <Pt/SourceInfo.h>


#include "ApplicationImpl.h"
#include "ScreenImpl.h"
#include "WindowFrameImpl.h"
#include "KeyHandler.h"



#ifndef _AIX
#include <X11/Xft/Xft.h>
#endif

namespace wildcat {
namespace gui {
namespace window {

ApplicationImpl::ApplicationImpl()
: _display( XOpenDisplay(NULL) )
, _visual(0)
, _depth(0)
, _xfd(_display)
, _cursorHandle(None)
{
    // Open a X11 display connection
    //_display = XOpenDisplay(NULL);
    if( ! _display )
        throw Pt::AccessFailed("X11 display");

    //XSync(_display, false);

    unsigned int screen = XDefaultScreen(_display);
    ::Window root = XDefaultRootWindow(_display);

    XVisualInfo visualInfo = {};
    if( ! XMatchVisualInfo(_display, screen, 24, TrueColor, &visualInfo) )
    {
        assert(false);
        throw std::runtime_error("invalid X11 visual");
    }

    _visual = visualInfo.visual;
    _depth = visualInfo.depth;

    // Set X11 to sync mode. Slow, for debugging only.
    //XSynchronize(_display, True);

    _xfd.setActive(*this);
    _xfd.begin();
    //_xfd.flush();
    _xfd.eventReady() += Pt::slot(*this, &ApplicationImpl::onProcessXEvent);

    _paintGc = XCreateGC(_display, root, 0, NULL);

    _wmProtocols     = XInternAtom(_display, "WM_PROTOCOLS", False);
    _wmDeleteWindow  = XInternAtom(_display, "WM_DELETE_WINDOW", False);
    _wmChangeState   = XInternAtom(_display, "WM_CHANGE_STATE", False);

    _netWmState = XInternAtom(_display, "_NET_WM_STATE", False);
    _netWmStateMaximizedVert = XInternAtom(_display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
    _netWmStateMaximizedHorz = XInternAtom(_display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
    _netWmStateHidden = XInternAtom(_display, "_NET_WM_STATE_HIDDEN", False);
    _netWmStateAbove = XInternAtom(_display, "_NET_WM_STATE_ABOVE", False);
    _netWmActiveWindow = XInternAtom(_display, "_NET_ACTIVE_WINDOW", False);
}

ApplicationImpl::~ApplicationImpl()
{
    if(_cursorHandle != None)
    {
        XFreeCursor(_display, _cursorHandle);
        _cursorHandle = None;
    }

    XFreeGC(_display, _paintGc);
    _paintGc = 0;

    //XSync(_display, true);
    XCloseDisplay(_display);
    _display = 0;
}

void ApplicationImpl::setCursor(const Cursor* cursor)
{
    if(cursor != nullptr && &_cursor == cursor)
        return;

    if(cursor != nullptr && _cursor == *cursor)
        return;

    // Free previous cursor
    if(_cursorHandle != None)
    {
        XFreeCursor(_display, _cursorHandle);
        _cursorHandle = None;
    }

    if(cursor == nullptr || cursor->empty())
    {
        _cursor.clear();

        // Reset to default cursor on all windows
        const std::vector<Window*>& windows = Application::instance().screen().windows();
        for(size_t i = 0; i < windows.size(); ++i)
        {
            WindowFrameImpl* frame = static_cast<WindowFrameImpl*>(windows[i]->frame());
            if(frame)
                XUndefineCursor(_display, frame->nativeWindow());
        }

        XFlush(_display);
        return;
    }

    _cursor = *cursor;

    // Create X11 cursor from pixmap ARGB data
    const gfx::Pixmap& pixmap = _cursor.pixmap();
    int w = pixmap.width();
    int h = pixmap.height();

    if(w <= 0 || h <= 0)
        return;

    XcursorImage* xcImage = XcursorImageCreate(w, h);
    if( ! xcImage)
        return;

    xcImage->xhot = _cursor.xHotspot();
    xcImage->yhot = _cursor.yHotspot();

    // Pixmap stores pixels as ARGB32 (uint32_t: A<<24 | R<<16 | G<<8 | B)
    // XcursorImage expects the same ARGB32 layout
    const uint32_t* src = reinterpret_cast<const uint32_t*>(pixmap.data());
    XcursorPixel*   dst = xcImage->pixels;

    for(int y = 0; y < h; ++y)
    {
        for(int x = 0; x < w; ++x)
        {
            dst[y * w + x] = src[y * w + x];
        }
    }

    _cursorHandle = XcursorImageLoadCursor(_display, xcImage);
    XcursorImageDestroy(xcImage);

    if(_cursorHandle == None)
        return;

    // Apply cursor to all windows
    const std::vector<Window*>& windows = Application::instance().screen().windows();
    for(size_t i = 0; i < windows.size(); ++i)
    {
        WindowFrameImpl* frame = static_cast<WindowFrameImpl*>(windows[i]->frame());
        if(frame)
            XDefineCursor(_display, frame->nativeWindow(), _cursorHandle);
    }

    XFlush(_display);
}

void ApplicationImpl::setFontDir(const Pt::System::Path& dir)
{
    gfx::Graphics::setFontDir(dir);
}

void ApplicationImpl::setDefaultFont(const gfx::Font& font)
{
    gfx::Graphics::setDefaultFont(font);
}

Pt::Timespan ApplicationImpl::inactivityTime() const
{
  return Pt::Timespan(0);
}	

void ApplicationImpl::sendKeyEvent(const KeyEvent& ev)
{

}

void ApplicationImpl::sendMouseEvent(const MouseEvent& ev)
{
    
}

void ApplicationImpl::nextEvent()
{
    MainLoop::waitNext();
}

void ApplicationImpl::onRun()
{
    _xfd.processEvents();

    while( MainLoop::waitNext() )
    {
        _xfd.processEvents();
    }
}

Window* ApplicationImpl::findWindow(::Window window)
{
    const std::vector<Window*>& windows = Application::instance().screen().windows();

    for(size_t i = 0; i < windows.size(); ++i)
    {
        Window* w = windows[i];

        if( ! w->frame() )
            continue;

        WindowFrameImpl* windowImpl = static_cast<WindowFrameImpl*>( w->frame() );

        if( windowImpl->nativeWindow() == window )
            return w;
    }

    return 0;
}

void ApplicationImpl::onProcessXEvent(XEvent& ev)
{
    Window* w = findWindow(ev.xany.window);
    if( ! w )
        return;

    if (!w->frame())
        return;

    switch( ev.xany.type )
    {
        case ClientMessage:
            onClientMessage(*w, ev);
            break;

        case MapNotify:
            onShow(*w, true);
            break;

        case UnmapNotify:
            onShow(*w, false);
            break;

        case MotionNotify:
            onMotionNotify(*w, ev);
            break;

        case ButtonPress:
            onButtonPress(*w, ev);
            break;

        case ButtonRelease:
            onButtonRelease(*w, ev);
            break;

        case Expose:
            onExpose(*w, ev);
            break;

        case NoExpose:
            break;

        case ConfigureNotify:   
            onConfigureNotify(*w, ev);
            break;

        case KeyPress:
        case KeyRelease:
            onKeyEvent(*w, ev);
            break;

        // case FocusOut:
        //     if( _forceTopMost )
        //         bringWindowToTop();
        // break;

        case EnterNotify:
            onEnterNotify(*w, ev);
            break;

        case LeaveNotify:
            onLeaveNotify(*w, ev);
            break;

        case FocusIn:
            onFocusIn(*w, ev);
            break;

        case FocusOut:
            onFocusOut(*w, ev);
            break;

        default:
            break;
    }
}

void ApplicationImpl::onEnterNotify(Window& window, XEvent& xev)
{
}

void ApplicationImpl::onLeaveNotify(Window& window, XEvent& xev)
{
}

void ApplicationImpl::onExpose(Window& window, XEvent& xev)
{
    int width = xev.xexpose.width;
    int height = xev.xexpose.height;
    int x = xev.xexpose.x;
    int y = xev.xexpose.y;

    gfx::Point pos(x, y);
    gfx::Size size(width, height);
    gfx::Rect rect(pos, size);

    rect = gfx::Rect( rect.topLeft() / window.scaleFactor(), 
                       rect.size() / window.scaleFactor() );

    WindowFrameBase* frame = window.frame();
    PaintEvent pev(*frame, rect);
    frame->processEvent(pev);

    WindowFrameImpl* windowImpl = static_cast<WindowFrameImpl*>( window.frame() );
    const gfx::Pixmap& image = windowImpl->window().graphics().image();
    char* data = reinterpret_cast<char*>( const_cast<Pt::uint8_t*>(image.data()) );

    XImage* ximage = XCreateImage(_display, _visual, _depth, ZPixmap, 0, 
                                  data, image.width(), image.height(), 
                                  _depth == 24 ? 32 : _depth, 0);
    
    unsigned int screen = DefaultScreen(_display);
    ::Window drawable = windowImpl->nativeWindow();
    GC gc = DefaultGC(_display, screen);
    
    XPutImage( _display, drawable, gc, ximage, x, y, x, y, width, height);
    
    ximage->data = NULL;
    XDestroyImage(ximage); 
}

void ApplicationImpl::onClientMessage(Window& window, XEvent& xev)
{
    if( xev.xclient.message_type == _wmProtocols )
    {
        if( (Atom) xev.xclient.data.l[0] == _wmDeleteWindow )
        {
            WindowFrameBase* frame = window.frame();

            CloseEvent ev(*frame);
            Application::instance().processEvent(ev);
        }
    }
}

void ApplicationImpl::onShow(Window& w, bool v)
{
    ShowEvent sev(*w.frame(), v);
    commitEvent( sev );
}

void ApplicationImpl::onMotionNotify(Window& window, XEvent& xev)
{
    int x = xev.xmotion.x;
    int y = xev.xmotion.y;

    const double scaling = window.scaleFactor();

    gfx::Point pos(x/scaling, y/scaling);

    _mouseEvent.setPosition( window.toGlobal(pos) );
    _mouseEvent.setElement(&window);
    _mouseEvent.setMove();
    
    Application::instance().processEvent(_mouseEvent);
}

void ApplicationImpl::onButtonPress(Window& window, XEvent& xev)
{
    int x = xev.xbutton.x;
    int y = xev.xbutton.y;
    MouseEvent::Button button = MouseEvent::Left;

    switch(xev.xbutton.button)
    {
        case Button1:
            button = MouseEvent::Left;
            break;

        case Button2:
            button = MouseEvent::Middle;
            break;

        case Button3:
            button = MouseEvent::Right;
            break;

        default:
            return;
    }
   
    const double scaling = window.scaleFactor();

    const gfx::Point pos(x/scaling, y/scaling);

    _mouseEvent.setPosition( window.toGlobal(pos) );
    _mouseEvent.setPress(button);
    _mouseEvent.setElement(&window);    
    Application::instance().processEvent(_mouseEvent);
}

void ApplicationImpl::onButtonRelease(Window& window, XEvent& xev)
{
    int x = xev.xbutton.x;
    int y = xev.xbutton.y;

    MouseEvent::Button button = MouseEvent::Left;    

    switch(xev.xbutton.button)
    {
        case Button1:
            button = MouseEvent::Left;
            break;

        case Button2:
            button = MouseEvent::Middle;
            break;

        case Button3:
            button = MouseEvent::Right;
            break;

        case Button4: // wheel up
        {
            ScrollEvent sev(window);
            sev.set(ScrollEvent::Vertical, 20);
            Application::instance().processEvent(sev);
            return;
        }

        case Button5: // wheel down;
        {
            ScrollEvent sev(window);
            sev.set(ScrollEvent::Vertical, -20);
            Application::instance().processEvent(sev);
            return;
        }

        default:
            return;
    }

    const double scaling = window.scaleFactor();

    gfx::Point pos(x/scaling, y/scaling);
    _mouseEvent.setPosition( window.toGlobal(pos) );
    _mouseEvent.setRelease(button);
    _mouseEvent.setElement(&window);

    Application::instance().processEvent(_mouseEvent);
}

void ApplicationImpl::onKeyEvent(Window& window, XEvent& xev)
{
    bool isPress = KeyPress == xev.xkey.type;

    KeySym keySym = 0;
    char buffer[20];
    XLookupString(&xev.xkey, buffer, sizeof(buffer), &keySym, NULL);

    Key::Modifiers modifiers;

    if(xev.xkey.state & ShiftMask)
    {
        modifiers.add(Key::Shift);
    }
    if(xev.xkey.state & ControlMask)
    {
        modifiers.add(Key::Control);
    }
    if(xev.xkey.state & Mod1Mask)
    {
        modifiers.add(Key::Alt);
    }
    if(xev.xkey.state & Mod5Mask)
    {
        modifiers.add(Key::Alt);
    }
    if(xev.xkey.state & Mod4Mask)
    {
        modifiers.add(Key::Meta);
    }

    Pt::Char ch = KeyHandler::keySymToUtf(keySym);
    Pt::uint32_t keyCode = KeyHandler::keySymToCode(keySym);

    Key key(modifiers, keyCode);

    if(isPress)
        _keyEvent.setPress(key, ch);
    else
        _keyEvent.setRelease(key, ch);

    _keyEvent.setElement(&window);
    
    Application::instance().processEvent(_keyEvent);
}

void ApplicationImpl::onConfigureNotify(Window& window, XEvent& xev)
{
    WindowFrameImpl* windowImpl = static_cast<WindowFrameImpl*>( window.frame() );

    const int width  = xev.xconfigure.width;
    const int height = xev.xconfigure.height;
    const int x      = xev.xconfigure.x;
    const int y      = xev.xconfigure.y;

    if( windowImpl->width() != width || windowImpl->height() != height )
    {
        windowImpl->setSize(width, height);

        gfx::Size to(width, height);
        to = window.graphics().toLogical(to);
    
        ResizeEvent rev(*window.frame(), to);
        Application::instance().processEvent(rev);

        gfx::Rect updateRect(gfx::Point(0,0), to);
        window.repaint(updateRect);
    }

    gfx::Point currentPos = window.graphics().toPhysical( window.position() );

    if( x != currentPos.x() || y != currentPos.y() )
    {
        gfx::Point to(x, y);
        to = window.graphics().toLogical(to);

        MoveEvent ev(*window.frame(), to);
        Application::instance().processEvent(ev);
    }
}

void ApplicationImpl::onFocusIn(Window& window, XEvent& xev)
{
    // Ignore focus events from grabs (e.g. pointer grabs)
    if(xev.xfocus.mode == NotifyGrab || xev.xfocus.mode == NotifyUngrab)
        return;

    
    WindowFrameBase* frame = window.frame();
    if(frame)
    {
        ActivateEvent ev(*frame, true);
        Application::instance().processEvent(ev);
    }
}

void ApplicationImpl::onFocusOut(Window& window, XEvent& xev)
{
    // Ignore focus events from grabs (e.g. pointer grabs)
    if(xev.xfocus.mode == NotifyGrab || xev.xfocus.mode == NotifyUngrab)
        return;

    
    WindowFrameBase* frame = window.frame();
    if(frame)
    {
        ActivateEvent ev(*frame, false);
        Application::instance().processEvent(ev);
    }
}

}}}


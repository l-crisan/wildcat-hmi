#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/WindowManager.h>
#include <wildcat/gui/window/WindowStateEvent.h>
#include "WindowFrameImpl.h"
#include "ApplicationImpl.h"
#include "ScreenImpl.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace wildcat {
namespace gui {
namespace window {

WindowFrameImpl::WindowFrameImpl(ScreenImpl& wm, Window& w)
: WindowFrameBase(wm, w)
, _wm(wm)
, _client(w)
, _window(None)
, _display(0)
, _hasFirstShow(false)
, _width(240)
, _height(160)
, _showClose(true)
, _showMinimize(true)
, _showMaximize(true)
, _showSystemMenu(true)
{
  _display = Application::instance().impl()->display();

  create( w.frameType() );
}

WindowFrameImpl::~WindowFrameImpl()
{
    destroy();
}

void WindowFrameImpl::create(WindowType type)
{
    // Display and Screen are inited in Application
    unsigned int screen = XDefaultScreen(_display);
    ::Window root       = XRootWindow(_display, screen);
    ::Visual* visual = Application::instance().impl()->visual();
    int depth = Application::instance().impl()->depth();

    XSetWindowAttributes wattr;
    memset(&wattr, 0, sizeof(wattr));
    wattr.colormap = XDefaultColormap(_display, screen);

    // The events we want to receive
    wattr.event_mask = StructureNotifyMask|ExposureMask|
                       PropertyChangeMask|EnterWindowMask|
                       LeaveWindowMask|KeyPressMask|
                       KeyReleaseMask|KeymapStateMask|
                       ButtonPressMask|ButtonReleaseMask|
                       PointerMotionMask|FocusChangeMask|
                       SubstructureNotifyMask|VisibilityChangeMask|
                       FocusChangeMask|EnterWindowMask|
                       LeaveWindowMask;

    wattr.do_not_propagate_mask = NoEventMask;/*KeyPressMask|KeyReleaseMask|
                                  ButtonPressMask| ButtonReleaseMask|
                                  PointerMotionMask|ButtonMotionMask;*/

    // Border
    wattr.border_pixel = 0; // Needed for OpenGL
    wattr.border_pixmap = CopyFromParent;

    // Background
    wattr.background_pixmap = None;
    //wattr.background_pixel = XWhitePixel(display, screen);

    // Backing store
    wattr.backing_store = NotUseful; // Always;
    wattr.save_under = False;   // True;

    // Region to be retained on resize
    wattr.bit_gravity = ForgetGravity;

    // How to to reposition when parent resizes
    wattr.win_gravity = NorthWestGravity;

    // None means parents cursor
    wattr.cursor = None;

    // no WM interaction if True
    wattr.override_redirect = type == WindowType::Borderless ? True : False;

    // Determines which fields from XSetWindowAttributes are used
    unsigned long winMask = CWWinGravity|CWBitGravity|
                            CWBorderPixmap|CWBorderPixel|
                            CWEventMask|CWDontPropagate|
                            CWCursor|CWOverrideRedirect|
                            CWColormap|CWBackingStore|
                            CWSaveUnder|CWBackPixmap;

    unsigned int borderWidth = 0;

    
    // Create the X11 window
    _window = XCreateWindow(_display,
                            root,
                            0, 0,
                            _width, _height,
                            borderWidth,
                            depth,
                            InputOutput,
                            visual,
                            winMask,
                            &wattr);


    Atom atomDeleteWindow = Application::instance().impl()->wmDeleteWindow();
    XSetWMProtocols(_display, _window, &atomDeleteWindow, 1);

    Base::onSetParent(&_wm);
}

gfx::Point WindowFrameImpl::onClientOffset() const
{
    return gfx::Point(7, 20);
}

void WindowFrameImpl::destroy()
{
    if( _window == 0)
        return;
    
    XSync(_display, False);

    XDestroyWindow(_display, _window);
    _window = 0;
}

void WindowFrameImpl::onInit(Window& w)
{    
    
}

void WindowFrameImpl::onRelease(Window& w)
{
    
}

void WindowFrameImpl::setType(WindowType type)
{
    XSetWindowAttributes swattr;
    swattr.override_redirect = (type == WindowType::Borderless) ? True : False;

    XChangeWindowAttributes(_display, _window, CWOverrideRedirect, &swattr);

    XWindowAttributes wattr;
    XGetWindowAttributes(_display, _window, &wattr);

    if(wattr.map_state != IsUnmapped)
    {
        XUnmapWindow(_display, _window);
        XMapWindow(_display,_window);
    }
}

gfx::Point WindowFrameImpl::toScreen(const gfx::Point& pos) const
{
    gfx::Point physicalPos = window().graphics().toPhysical(pos);

    ::Window root = DefaultRootWindow(_display);
    int windowX = lround( physicalPos.x() );
    int windowY = lround( physicalPos.y() );
    int screenX = 0;
    int screenY = 0;
    ::Window child = 0;

    XTranslateCoordinates(_display, _window, root,
                          windowX, windowY, &screenX, &screenY, &child);

    gfx::Point screenPos(screenX, screenY);
    gfx::Point logicalPos = window().graphics().toLogical(screenPos);
    return logicalPos;
}

gfx::Point WindowFrameImpl::fromScreen(const gfx::Point& pos) const
{
    gfx::Point physicalPos = window().graphics().toPhysical(pos);

    ::Window root = DefaultRootWindow(_display);
    int screenX = lround( physicalPos.x() );
    int screenY = lround( physicalPos.y() );
    int windowX = 0;
    int windowY = 0;
    ::Window child = 0;

    XTranslateCoordinates(_display, root, _window,
                          screenX, screenY, &windowX, &windowY, &child);

    gfx::Point clientPos(windowX, windowY);
    gfx::Point logicalPos = window().graphics().toLogical(clientPos);
    return logicalPos;
}

gfx::Point WindowFrameImpl::onToWindow(const Window& w,
                                       const gfx::Point& pos) const
{
    return pos;
}

gfx::Point WindowFrameImpl::onFromWindow(const Window& w, const gfx::Point& pos) const
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
    if( rect.isNull() )
      return;

    XClearArea(_display, _window,
               static_cast<int>( rect.x() ),
               static_cast<int>( rect.y() ),
               static_cast<int>( rect.width() ),
               static_cast<int>( rect.height() ),
               True);
}

void WindowFrameImpl::onRepaint(Window& w, const gfx::Rect& rect)
{
    const gfx::Point screenPos = toScreen( rect.topLeft() );
    const gfx::Rect screenRect( screenPos, rect.size() );

    _wm.repaint(screenRect);
}

void WindowFrameImpl::onProcessPaintEvent(const PaintEvent& ev)
{
    Base::onProcessPaintEvent(ev);

    PaintEvent rev( _client, ev.rect() );
    _client.processEvent(rev);
}

void WindowFrameImpl::onPaintEvent(const PaintEvent& ev)
{
    Base::onPaintEvent(ev);
}

void WindowFrameImpl::onShow(Window& w, bool visible)
{
    if (visible)
    {
        // Set transient-for and modal hints if this window has a logical parent
        
        if (!_hasFirstShow &&  w.isModal())
        {
            _hasFirstShow = true;

            WindowBase* logicalParent = w.logicalParent();
            if (logicalParent)
            {
                // Find the native X11 window of the logical parent
                Window* parentWindow = dynamic_cast<Window*>(logicalParent);
                if (parentWindow && parentWindow->frame())
                {
                    WindowFrameImpl* parentFrame =
                        static_cast<WindowFrameImpl*>(parentWindow->frame());

                    // Tell the WM this window belongs to the parent
                    XSetTransientForHint(_display, _window,
                        parentFrame->nativeWindow());

                    // Set _NET_WM_STATE_MODAL to make it application-modal
                    Atom netWmStateModal = XInternAtom(_display,
                        "_NET_WM_STATE_MODAL",
                        False);
                    Atom netWmState = Application::instance().impl()->netWmState();

                    XChangeProperty(_display, _window, netWmState, XA_ATOM, 32,
                        PropModeAppend,
                        reinterpret_cast<const unsigned char*>(&netWmStateModal),
                        1);
                }
            }
        }

        XMapWindow(_display, _window);

        XClearArea(_display, _window, 0, 0, _width, _height, True);
    }
    else
    {
        XUnmapWindow(_display, _window);
    }

    XFlush(_display);
}

void WindowFrameImpl::onProcessShowEvent(const ShowEvent& ev)
{
    Base::onProcessShowEvent(ev);

    ShowEvent rev( _client, ev.visible() );
    _client.processEvent(rev);
}

void WindowFrameImpl::onShowEvent(const ShowEvent& ev)
{
    Base::onShowEvent(ev);
}

void WindowFrameImpl::onActivate(Window& w, bool active)
{
    if(  active )
    {
        XEvent xev;
        memset(&xev, 0, sizeof(xev));
        xev.type = ClientMessage;
        xev.xclient.display      = _display;
        xev.xclient.window       = _window;
        xev.xclient.message_type = Application::instance().impl()->netWmActiveWindow();
        xev.xclient.format       = 32;
        xev.xclient.data.l[0]    = 2L; /* 2 == Message from a window pager */
        xev.xclient.data.l[1]    = CurrentTime;

        XWindowAttributes wattr;
        XGetWindowAttributes(_display, _window, &wattr);

        XSendEvent(_display, XDefaultRootWindow(_display),
                   False, SubstructureNotifyMask|SubstructureRedirectMask, &xev);
    }
}

void WindowFrameImpl::onProcessActivateEvent(const ActivateEvent& ev)
{
    Base::onProcessActivateEvent(ev);

    ActivateEvent aev( _client, ev.isActive() );
    _client.processEvent(aev);
}

void WindowFrameImpl::onActivateEvent(const ActivateEvent& ev)
{
    Base::onActivateEvent(ev);
}

void WindowFrameImpl::onEnable(Window& w, bool enable)
{
    XSetWindowAttributes wattr;

    if(enable)
    {
        wattr.event_mask = StructureNotifyMask|ExposureMask|
                           PropertyChangeMask|EnterWindowMask|
                           LeaveWindowMask|KeyPressMask|
                           KeyReleaseMask|KeymapStateMask|
                           ButtonPressMask|ButtonReleaseMask|
                           PointerMotionMask|FocusChangeMask|
                           SubstructureNotifyMask|VisibilityChangeMask;

        XChangeWindowAttributes(_display, _window, CWEventMask, &wattr);

        // Restore size limits from the window
        onSetSizeLimits(w, w.minimumSize(), w.maximumSize());

        // Restore Motif WM hints (buttons)
        updateMotifWmHints();
    }
    else
    {
        wattr.event_mask = StructureNotifyMask|ExposureMask|
                           FocusChangeMask;

        XChangeWindowAttributes(_display, _window, CWEventMask, &wattr);

        // Lock current size to prevent WM resize/maximize
        XSizeHints hints;
        memset(&hints, 0, sizeof(hints));

        hints.flags = PMinSize | PMaxSize;
        hints.min_width  = _width;
        hints.min_height = _height;
        hints.max_width  = _width;
        hints.max_height = _height;

        XSetWMNormalHints(_display, _window, &hints);

        // Remove all WM functions (resize, minimize, maximize, close)
        struct MotifWmHints
        {
            unsigned long flags;
            unsigned long functions;
            unsigned long decorations;
            long          inputMode;
            unsigned long status;
        };

        Atom motifWmHintsAtom = XInternAtom(_display, "_MOTIF_WM_HINTS", False);

        MotifWmHints mwmHints;
        memset(&mwmHints, 0, sizeof(mwmHints));
        mwmHints.flags       = (1ul << 0); // MWM_HINTS_FUNCTIONS
        mwmHints.functions   = (1ul << 2); // MWM_FUNC_MOVE only

        XChangeProperty(_display, _window, motifWmHintsAtom, motifWmHintsAtom, 32,
                        PropModeReplace,
                        reinterpret_cast<const unsigned char*>(&mwmHints),
                        sizeof(mwmHints) / sizeof(unsigned long));

        XFlush(_display);
    }
}

void WindowFrameImpl::onProcessEnableEvent(const EnableEvent& ev)
{
    Base::onProcessEnableEvent(ev);

    EnableEvent eev( _client, ev.enabled() );
    _client.processEvent(eev);
}

void WindowFrameImpl::onEnableEvent(const EnableEvent& ev)
{    
    Base::onEnableEvent(ev);
}

void WindowFrameImpl::onMove(Window& w, const gfx::Point& pos)
{
   const gfx::Point p = w.graphics().toPhysical(pos);

   XMoveWindow(_display, _window, p.x(), p.y());
}

void WindowFrameImpl::onProcessMoveEvent(const MoveEvent& ev)
{
    Base::onProcessMoveEvent(ev);

    MoveEvent mev( _client, ev.position() );
    _client.processEvent(mev);
}


void WindowFrameImpl::onResize(Window& w, const gfx::Size& s)
{    
    gfx::Size alignedSize = s;

    
    // maximum width and height
    
    if (alignedSize.width() > w.maximumSize().width())
        alignedSize.setWidth(w.maximumSize().width());

    if (alignedSize.height() > w.maximumSize().height())
        alignedSize.setHeight(w.maximumSize().height());

    if (alignedSize.width() < w.minimumSize().width())
        alignedSize.setWidth(w.minimumSize().width());

    if (alignedSize.height() < w.minimumSize().height())
        alignedSize.setHeight(w.minimumSize().height());

    const gfx::Size size = w.graphics().toPhysical(alignedSize);

    // X11 requires width and height to be at least 1
    const unsigned int finalWidth = std::max(1, static_cast<int>(size.width()));
    const unsigned int finalHeight = std::max(1, static_cast<int>(size.height()));

    XResizeWindow(_display, _window, finalWidth, finalHeight);
}

void WindowFrameImpl::onProcessResizeEvent(const ResizeEvent& ev)
{
    Base::onProcessResizeEvent(ev);

    ResizeEvent rev( _client, ev.size() );
    _client.processEvent(rev);
}

void WindowFrameImpl::onClose(Window& w)
{
    WindowFrameImpl* frame = static_cast<WindowFrameImpl*>( w.frame() );
    if(frame)
    {
        XEvent ev;
        memset(&ev, 0, sizeof (ev));

        ev.xclient.type         = ClientMessage;
        ev.xclient.window       = frame->nativeWindow();
        ev.xclient.message_type = Application::instance().impl()->wmProtocols();
        ev.xclient.format       = 32;
        ev.xclient.data.l[0]    = Application::instance().impl()->wmDeleteWindow();
        ev.xclient.data.l[1]    = CurrentTime;
        XSendEvent(_display, _window, False, NoEventMask, &ev);
    }
}

void WindowFrameImpl::onProcessCloseEvent(const CloseEvent& ev)
{
    onCloseEvent(ev);

    CloseEvent cev(_client);
    _client.processEvent(cev);
}

void WindowFrameImpl::onCloseEvent(const CloseEvent& ev)
{
}

void WindowFrameImpl::onSetAbove(Window& w, bool above)
{
    enum
    {
        _NET_WM_STATE_REMOVE = 0,
        _NET_WM_STATE_ADD    = 1,
        _NET_WM_STATE_TOGGLE = 2
    };

    Atom netWmState = Application::instance().impl()->netWmState();
    Atom netWmStateAbove = Application::instance().impl()->netWmStateAbove();

    // Check if the window is already mapped (visible)
    XWindowAttributes wattr;
    XGetWindowAttributes(_display, _window, &wattr);

    if(wattr.map_state == IsUnmapped)
    {
        // Window not yet visible: set/remove _NET_WM_STATE property directly.
        // The WM will read this when the window is mapped.
        if(above)
        {
            XChangeProperty(_display, _window, netWmState, XA_ATOM, 32,
                            PropModeAppend,
                            reinterpret_cast<const unsigned char*>(&netWmStateAbove),
                            1);
        }
        else
        {
            // Read current state and rebuild without _ABOVE
            Atom actual_type;
            int actual_format;
            unsigned long num_items, bytes_after;
            Atom* atoms = 0;

            XGetWindowProperty(_display, _window, netWmState,
                               0, 1024, False, XA_ATOM,
                               &actual_type, &actual_format,
                               &num_items, &bytes_after,
                               reinterpret_cast<unsigned char**>(&atoms));

            std::vector<Atom> newState;
            for(unsigned long i = 0; i < num_items; ++i)
            {
                if(atoms[i] != netWmStateAbove)
                    newState.push_back(atoms[i]);
            }

            if(atoms)
                XFree(atoms);

            if(newState.empty())
                XDeleteProperty(_display, _window, netWmState);
            else
                XChangeProperty(_display, _window, netWmState, XA_ATOM, 32,
                                PropModeReplace,
                                reinterpret_cast<const unsigned char*>(newState.data()),
                                static_cast<int>(newState.size()));
        }
    }
    else
    {
        // Window is visible: use ClientMessage to request the WM to change state
        XEvent ev;
        memset(&ev, 0, sizeof(ev));
        ev.xclient.type = ClientMessage;
        ev.xclient.serial = 0;
        ev.xclient.send_event = True;
        ev.xclient.display = _display;
        ev.xclient.window = _window;
        ev.xclient.message_type = netWmState;
        ev.xclient.format = 32;
        ev.xclient.data.l[0] = above ? _NET_WM_STATE_ADD
                                      : _NET_WM_STATE_REMOVE;
        ev.xclient.data.l[1] = netWmStateAbove;
        ev.xclient.data.l[2] = 0;
        ev.xclient.data.l[3] = 0;
        ev.xclient.data.l[4] = 0;

        XSendEvent(_display, XDefaultRootWindow(_display), False,
                   SubstructureRedirectMask | SubstructureNotifyMask, &ev);
    }

    XFlush(_display);
}

void WindowFrameImpl::onSetState(Window& w, const WindowState& s)
{
    //std::clog  << "setState: " << s << std::endl;

    XClientMessageEvent ev;
    ev.type       = ClientMessage;
    ev.serial     = 0;
    ev.window     = _window;
    ev.format     = 32;
    ev.send_event = True;

    switch(s)
    {
        default:
        case WindowState::Normal:
            ev.message_type = Application::instance().impl()->wmChangeState();
            ev.data.l[0] = NormalState;
            break;

        case WindowState::Minimized:
            ev.message_type = Application::instance().impl()->wmChangeState();
            ev.data.l[0] = IconicState;
            break;

        case WindowState::Maximized:
            ev.message_type = Application::instance().impl()->netWmState();
            ev.data.l[0]  = 1ul;
            ev.data.l[1]  = Application::instance().impl()->netWmStateMaximizedVert();
            ev.data.l[2]  = Application::instance().impl()->netWmStateMaximizedHorz();
            break;
    }

    XSendEvent(_display, XDefaultRootWindow(_display), False,
               SubstructureRedirectMask, (XEvent*)&ev);

    //XFlush(_display);
}

void WindowFrameImpl::onProcessWindowStateEvent(const WindowStateEvent& ev)
{
    Base::onProcessWindowStateEvent(ev);

    WindowStateEvent wse( _client, ev.state() );
    Application::instance().processEvent(wse);
}

void WindowFrameImpl::onWindowStateEvent(const WindowStateEvent& ev)
{
}

void WindowFrameImpl::onSetTitle(Window& w, const std::string& text)
{
    XStoreName(_display, _window, text.c_str());
}

void WindowFrameImpl::onSetIcon(Window& w, uint32_t iconId)
{
    const gfx::Pixmap* icon = Application::instance().resourceMng().get<gfx::Pixmap>(iconId);

    if( icon == nullptr )
        return; 

    Atom netWmIcon = XInternAtom(_display, "_NET_WM_ICON", False);
    Atom cardinal  = XInternAtom(_display, "CARDINAL", False);

    if(icon.empty())
    {
        XDeleteProperty(_display, _window, netWmIcon);
        XFlush(_display);
        return;
    }

    const int imgWidth  = static_cast<int>(icon->width());
    const int imgHeight = static_cast<int>(icon->height());
    const size_t pixelCount = static_cast<size_t>(imgWidth) * imgHeight;

    // _NET_WM_ICON format: [width, height, ARGB pixels...]
    // Each element is an unsigned long (may be 64-bit on x86_64)
    const size_t dataSize = 2 + pixelCount;
    std::vector<unsigned long> iconData(dataSize);

    iconData[0] = static_cast<unsigned long>(imgWidth);
    iconData[1] = static_cast<unsigned long>(imgHeight);

    // Copy pixels row by row to handle possible padding in the Pixmap
    const size_t stride = icon->width() * 4 + icon->padding();

    for(int y = 0; y < imgHeight; ++y)
    {
        const uint32_t* row = reinterpret_cast<const uint32_t*>(icon->data() + y * stride);

        for(int x = 0; x < imgWidth; ++x)
        {
            iconData[2 + y * imgWidth + x] = static_cast<unsigned long>(row[x]);
        }
    }

    XChangeProperty(_display, _window, netWmIcon, cardinal, 32,
                    PropModeReplace,
                    reinterpret_cast<const unsigned char*>(iconData.data()),
                    static_cast<int>(dataSize));

    // Also set the legacy WM_HINTS icon pixmap for WMs that don't support _NET_WM_ICON
    unsigned int screen = XDefaultScreen(_display);
    int depth = Application::instance().impl()->depth();
    ::Window root = XRootWindow(_display, screen);

    Pixmap iconPixmap = XCreatePixmap(_display, root, imgWidth, imgHeight, depth);
    Pixmap maskPixmap = XCreatePixmap(_display, root, imgWidth, imgHeight, 1);

    GC gc = XCreateGC(_display, iconPixmap, 0, NULL);
    GC maskGc = XCreateGC(_display, maskPixmap, 0, NULL);

    XImage* ximage = XCreateImage(_display,
                                  Application::instance().impl()->visual(),
                                  depth, ZPixmap, 0, NULL,
                                  imgWidth, imgHeight, 32, 0);

    ximage->data = static_cast<char*>(malloc(ximage->bytes_per_line * imgHeight));

    for(int y = 0; y < imgHeight; ++y)
    {
        const uint32_t* row = reinterpret_cast<const uint32_t*>(icon->data() + y * stride);

        for(int x = 0; x < imgWidth; ++x)
        {
            uint32_t pixel = row[x];
            XPutPixel(ximage, x, y, pixel & 0x00FFFFFF);
        }
    }

    XPutImage(_display, iconPixmap, gc, ximage, 0, 0, 0, 0, imgWidth, imgHeight);

    ximage->data = NULL;
    XDestroyImage(ximage);
    XFreeGC(_display, gc);

    // Build 1-bit mask from alpha channel
    for(int y = 0; y < imgHeight; ++y)
    {
        const uint32_t* row = reinterpret_cast<const uint32_t*>(icon->data() + y * stride);

        for(int x = 0; x < imgWidth; ++x)
        {
            uint32_t alpha = row[x] >> 24;
            XSetForeground(_display, maskGc, alpha >= 128 ? 1 : 0);
            XDrawPoint(_display, maskPixmap, maskGc, x, y);
        }
    }

    XFreeGC(_display, maskGc);

    XWMHints* wmHints = XAllocWMHints();
    wmHints->flags = IconPixmapHint | IconMaskHint;
    wmHints->icon_pixmap = iconPixmap;
    wmHints->icon_mask = maskPixmap;
    XSetWMHints(_display, _window, wmHints);
    XFree(wmHints);

    XFlush(_display);
}

void WindowFrameImpl::onSetSizeLimits(Window& w, const gfx::Size& minSize, const gfx::Size& maxSize)
{
    XSizeHints hints;
    memset(&hints, 0, sizeof(hints));

    hints.flags = PMinSize | PMaxSize;
    hints.min_width  = lround( minSize.width() );
    hints.min_height = lround( minSize.height() );
    hints.max_width  = lround( maxSize.width() );
    hints.max_height = lround( maxSize.height() );

    XSetWMNormalHints(_display, _window, &hints);
}

bool WindowFrameImpl::isMinimized()
{
    Atom actual_type;
    int actual_format;
    unsigned long num_items, bytes_after;
    Atom* atoms = 0;
    Atom requestAtom = Application::instance().impl()->netWmState();
    Atom compareAtom = Application::instance().impl()->netWmStateHidden();

    XGetWindowProperty(_display, _window,
                       requestAtom, 0, 1024, False, XA_ATOM,
                       &actual_type, &actual_format,
                       &num_items, &bytes_after,
                       (unsigned char**)&atoms);

    for(unsigned long i = 0; i < num_items; ++i)
    {
        if(atoms[i] == compareAtom)
        {
            XFree(atoms);
            return true;
        }
    }

    XFree(atoms);
    return false;
}

bool WindowFrameImpl::isMaximized()
{
    Atom actual_type;
    int actual_format;
    unsigned long num_items, bytes_after;
    Atom* atoms = 0;
    Atom requestAtom = Application::instance().impl()->netWmState();
    Atom compareAtom = Application::instance().impl()->netWmStateMaximizedHorz();

    XGetWindowProperty(_display, _window,
                       requestAtom, 0, 1024, False, XA_ATOM,
                       &actual_type, &actual_format,
                       &num_items, &bytes_after,
                       (unsigned char**)&atoms);

    for(unsigned long i = 0; i < num_items; ++i)
    {
        if( atoms[i] == compareAtom )
        {
            XFree(atoms);
            return true;
        }
    }

    XFree(atoms);
    return 0;
}

void WindowFrameImpl::onShowSystemMenu(Window& w, bool v)
{
    _showSystemMenu = v;

    updateMotifWmHints();
}

void WindowFrameImpl::onShowCloseButton(Window& w, bool v)
{
    _showClose = v;
    
    updateMotifWmHints();
}

void WindowFrameImpl::onShowMinimizeButton(Window& w, bool v)
{
    _showMinimize = v;
    
    updateMotifWmHints();
}

void WindowFrameImpl::onShowMaximizeButton(Window& w, bool v)
{
    _showMaximize = v;
    
    updateMotifWmHints();
}

void WindowFrameImpl::updateMotifWmHints()
{
    // Motif WM hints structure
    struct MotifWmHints
    {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long          inputMode;
        unsigned long status;
    };

    enum MotifFlags
    {
        MWM_HINTS_FUNCTIONS   = 1ul << 0,
        MWM_HINTS_DECORATIONS = 1ul << 1
    };

    enum MotifFunctions
    {
        MWM_FUNC_ALL      = 1ul << 0,
        MWM_FUNC_RESIZE   = 1ul << 1,
        MWM_FUNC_MOVE     = 1ul << 2,
        MWM_FUNC_MINIMIZE = 1ul << 3,
        MWM_FUNC_MAXIMIZE = 1ul << 4,
        MWM_FUNC_CLOSE    = 1ul << 5
    };

    enum MotifDecorations
    {
        MWM_DECOR_ALL      = 1ul << 0,
        MWM_DECOR_BORDER   = 1ul << 1,
        MWM_DECOR_RESIZEH  = 1ul << 2,
        MWM_DECOR_TITLE    = 1ul << 3,
        MWM_DECOR_MENU     = 1ul << 4,
        MWM_DECOR_MINIMIZE = 1ul << 5,
        MWM_DECOR_MAXIMIZE = 1ul << 6
    };

    // 1) Set _MOTIF_WM_HINTS (works on KDE/KWin, XFCE/xfwm4)
    Atom motifWmHintsAtom = XInternAtom(_display, "_MOTIF_WM_HINTS", False);

    unsigned long functions   = MWM_FUNC_MOVE | MWM_FUNC_RESIZE;
    unsigned long decorations = MWM_DECOR_BORDER | MWM_DECOR_RESIZEH
                              | MWM_DECOR_TITLE;

    if(_showSystemMenu)
    {
        decorations |= MWM_DECOR_MENU;
    }

    if(_showMinimize)
    {
        functions   |= MWM_FUNC_MINIMIZE;
        decorations |= MWM_DECOR_MINIMIZE;
    }

    if(_showMaximize)
    {
        functions   |= MWM_FUNC_MAXIMIZE;
        decorations |= MWM_DECOR_MAXIMIZE;
    }

    if(_showClose)
    {
        functions |= MWM_FUNC_CLOSE;
    }

    MotifWmHints mwmHints;
    memset(&mwmHints, 0, sizeof(mwmHints));
    mwmHints.flags       = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
    mwmHints.functions   = functions;
    mwmHints.decorations = decorations;

    XChangeProperty(_display, _window, motifWmHintsAtom, motifWmHintsAtom, 32,
                    PropModeReplace,
                    reinterpret_cast<const unsigned char*>(&mwmHints),
                    sizeof(mwmHints) / sizeof(unsigned long));

    // 2) Update WM_PROTOCOLS: selectively add/remove WM_DELETE_WINDOW
    //    GNOME/Mutter shows the close button based on WM_DELETE_WINDOW presence
    Atom wmProtocols = Application::instance().impl()->wmProtocols();
    Atom wmDeleteWindow = Application::instance().impl()->wmDeleteWindow();

    // Read current protocols
    Atom* currentProtocols = NULL;
    int protocolCount = 0;
    XGetWMProtocols(_display, _window, &currentProtocols, &protocolCount);

    // Rebuild the protocol list
    std::vector<Atom> newProtocols;

    for(int i = 0; i < protocolCount; ++i)
    {
        if(currentProtocols[i] != wmDeleteWindow)
            newProtocols.push_back(currentProtocols[i]);
    }

    if(currentProtocols)
        XFree(currentProtocols);

    if(_showClose)
        newProtocols.push_back(wmDeleteWindow);

    XSetWMProtocols(_display, _window, newProtocols.data(),
                    static_cast<int>(newProtocols.size()));

    XFlush(_display);
}

}}}


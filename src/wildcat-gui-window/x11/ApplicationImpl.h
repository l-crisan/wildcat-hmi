#ifndef WILDCAT_GUI_WINDOW_APPLICATIONIMPL_H
#define WILDCAT_GUI_WINDOW_APPLICATIONIMPL_H


#include <wildcat/gui/window/Element.h>
#include <wildcat/gui/window/MouseEvent.h>
#include <wildcat/gui/window/KeyEvent.h>
#include <wildcat/gfx/Transform.h>
#include <wildcat/gfx/Font.h>
#include <Pt/System/MainLoop.h>
#include <Pt/Timespan.h>

#include "X11Fd.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <X11/Xcursor/Xcursor.h>

namespace wildcat {
namespace gui {
namespace window {
//sudo apt install libxcursor-dev
class ApplicationImpl : public Pt::System::MainLoop
{
    public:
        ApplicationImpl();

        virtual ~ApplicationImpl();

        void setCursor(const Cursor* cursor);

        const Cursor* cursor() const;
        
        void setFontDir(const Pt::System::Path& dir);

        void setDefaultFont(const gfx::Font& font);

        Pt::Timespan inactivityTime() const;

        void sendKeyEvent(const KeyEvent& ev);

        void sendMouseEvent(const MouseEvent& ev);

        void nextEvent();

    public:
        void processEvent(XEvent& xev)
        {
            onProcessXEvent(xev);
        }

        ::Display* display() const
        {
            return _display;
        }

        ::Visual* visual() const
        {
            return _visual;
        }

        int depth() const
        {
            return _depth;
        }

        Atom wmProtocols() const
        { return _wmProtocols; }

        Atom wmDeleteWindow() const
        { return _wmDeleteWindow; }

        Atom wmChangeState() const
        { return _wmChangeState; }

        Atom netWmState() const
        { return _netWmState; }

        Atom netWmStateMaximizedVert() const
        { return _netWmStateMaximizedVert; }

        Atom netWmStateMaximizedHorz() const
        { return _netWmStateMaximizedHorz; }

        Atom netWmStateHidden() const
        { return _netWmStateHidden; }

        Atom netWmStateAbove() const
        { return _netWmStateAbove; }

        Atom netWmActiveWindow() const
        { return _netWmActiveWindow; }

    protected:
        virtual void onRun();

    private:
        Window* findWindow(::Window w);

        void onProcessXEvent(XEvent& ev);

        void onEnterNotify(Window& window, XEvent& xev);

        void onLeaveNotify(Window& window, XEvent& xev);

        void onFocusIn(Window& window, XEvent& xev);

        void onFocusOut(Window& window, XEvent& xev);

        void onExpose(Window& window, XEvent& xev);

        void onClientMessage(Window& window, XEvent& xev);

        void onShow(Window& w, bool v);

        void onMotionNotify(Window& window, XEvent& xev);

        void onButtonPress(Window& window, XEvent& xev);

        void onButtonRelease(Window& window, XEvent& xev);

        void onKeyEvent(Window& window, XEvent& xev);

        void onConfigureNotify(Window& window, XEvent& xev);

    private:
        ::Display* _display;
        ::Visual*  _visual;
        int        _depth;
        X11Fd      _xfd;
        Atom       _wmProtocols;
        Atom       _wmDeleteWindow;
        Atom       _wmChangeState;
        Atom       _netWmState;
        Atom       _netWmStateMaximizedVert;
        Atom       _netWmStateMaximizedHorz;
        Atom       _netWmStateHidden;
        Atom       _netWmStateAbove;
        Atom       _netWmActiveWindow;

        GC         _paintGc;
        MouseEvent _mouseEvent;
        KeyEvent   _keyEvent;
        Cursor     _cursor;
        ::Cursor   _cursorHandle;
};

}}}

#endif

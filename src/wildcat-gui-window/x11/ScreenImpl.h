
#ifndef WILDCAT_GUI_WINDOW_SCREENIMPL_H    
#define WILDCAT_GUI_WINDOW_SCREENIMPL_H

#include <wildcat/gui/window/Screen.h>
#include <wildcat/gui/window/WindowManagerBase.h>
#include <wildcat/gui/window/LayoutEvent.h> // RescaleEvent
#include <wildcat/gfx/Size.h>
#include <wildcat/gfx/Point.h>
#include <wildcat/gfx/Rect.h>
#include <Pt/Signal.h>

//#include <X11/X.h>
#include <X11/Xlib.h>
//#include <X11/Xatom.h>

#include <vector>

namespace wildcat {
namespace gui {
namespace window {

class ApplicationImpl;
class WindowFrameImpl;
class Window;
class MouseEvent;
class TouchEvent;
class ScrollEvent;

class ScreenImpl : public WindowManagerBase
{
    typedef WindowManagerBase Base;

    friend class WindowFrameImpl;

    public:
        ScreenImpl(ApplicationImpl& app);

        virtual ~ScreenImpl();

        void setParent(Screen* screen);

        void addWindow(Window& w);

        void removeWindow(Window& w);

        const std::vector<Window*>& windows() const;

        WindowManagerBase& windowManager();

        void setCapture(Element* capture);

    // WindowFrame
    protected:
        gfx::Point toFrame(const WindowFrameImpl& w,
                            const gfx::Point& pos) const;

        gfx::Point fromFrame(const WindowFrameImpl& w,
                              const gfx::Point& pos) const;

    // Responder
    protected:
        virtual bool onMouseEvent(const MouseEvent& ev);

        virtual bool onTouchEvent(const TouchEvent& ev);

        virtual bool onScrollEvent(const ScrollEvent& ev);

        virtual bool onKeyEvent(const KeyEvent& ev);

    // Element
    protected:
        virtual Element* onHitTest(const gfx::Point& pos, HitTestHint hint);

        virtual gfx::Point onToParent(const gfx::Point& pos) const;

        virtual gfx::Point onFromParent(const gfx::Point& pos) const;

        virtual bool onProcessEvent(const Pt::Event& ev);

        virtual void onRequestRepaint(const gfx::Rect& rect);

    // WindowManager
    protected:
        virtual WindowFrameBase* onAttach(Window& w);

        virtual void onDetach(WindowFrameBase& w);

        virtual void onInit(WindowFrameBase& w);

        virtual void onRelease(WindowFrameBase& w);

    // scaling
    protected:

    // painting
    protected:
        virtual void onProcessPaintEvent(const PaintEvent& ev);

        virtual void onPaintEvent(const PaintEvent& ev);

        virtual void onPaint(const gfx::Rect& rect);

    // enable
    protected:
        virtual void onProcessEnableEvent(const EnableEvent& ev);

        virtual void onEnableEvent(const EnableEvent& ev);

        virtual void onEnable(bool e);

    // input
    protected:
        bool onProcessMouseEvent(const MouseEvent& ev);

        bool onProcessTouchEvent(const TouchEvent& ev);

        bool onProcessScrollEvent(const ScrollEvent& ev);

        bool onProcessKeyEvent(const KeyEvent& ev);

    private:
        Screen*                 _parent;
        std::vector<Window*>    _windows;
        ::Display*              _display;        
        double                  _screenScaling;
};

}}} 

#endif

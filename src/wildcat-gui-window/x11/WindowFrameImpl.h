#ifndef WILDCAT_GUI_WINDOW_WINDOWIMPL_H    
#define WILDCAT_GUI_WINDOW_WINDOWIMPL_H

#include <wildcat/gui/window/WindowFrameBase.h>
#include <wildcat/gfx/Pixmap.h>
#include <wildcat/gfx/Point.h>
#include <wildcat/gfx/Size.h>


#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

namespace wildcat {
namespace gui {
namespace window {

class ScreenImpl;

class WindowFrameImpl : public WindowFrameBase
{
    typedef WindowFrameBase Base;

    friend class ScreenImpl;

    public:
        WindowFrameImpl(ScreenImpl& wm,  Window& w);

        virtual ~WindowFrameImpl();

        void setType(WindowType type);

        gfx::Point toScreen(const gfx::Point& pos) const;

        gfx::Point fromScreen(const gfx::Point& pos) const;

        void paint(const gfx::Rect& rect);

     //Nativ stuff
    public:
        ::Window& nativeWindow()
        {
            return _window;
        }

        int width() const
        {
           return _width;
        }

        int height() const
        {
           return _height;
        }

        void setSize(int w, int h)
        {
           _width = w;
           _height = h;
        }

    protected:
        virtual void onInit(Window& w);

        virtual void onRelease(Window& w);
        
        gfx::Point onClientOffset() const;

        virtual gfx::Point onToWindow(const Window& w, 
                                       const gfx::Point& pos) const;

        virtual gfx::Point onFromWindow(const Window& w, 
                                         const gfx::Point& pos) const;
        
        virtual void onSetTitle(Window& w, const std::string& text);

        virtual void onSetIcon(Window& w,uint32_t icon);

        virtual void onSetState(Window& w, const WindowState& state);

        virtual void onSetAbove(Window& w, bool above);

        virtual void onSetSizeLimits(Window& w, const gfx::Size& minSize, 
                                                const gfx::Size& maxSize);

        virtual void onRepaint(Window& w, const gfx::Rect& rect);

        virtual void onShow(Window& w, bool visible);

        virtual void onActivate(Window& w, bool active);

        virtual void onEnable(Window& w, bool enable);

        virtual void onMove(Window& w, const gfx::Point& to);

        virtual void onResize(Window& w, const gfx::Size& s);

        virtual void onClose(Window& w);

    protected:
        virtual gfx::Point onToParent(const gfx::Point& pos) const;
        
        virtual gfx::Point onFromParent(const gfx::Point& pos) const;

        
        virtual void onProcessPaintEvent(const PaintEvent& ev);

        virtual void onPaintEvent(const PaintEvent& ev);


        virtual void onProcessShowEvent(const ShowEvent& ev);

        virtual void onShowEvent(const ShowEvent& ev);


        virtual void onProcessEnableEvent(const EnableEvent& ev);

        virtual void onEnableEvent(const EnableEvent& ev);


        virtual void onProcessActivateEvent(const ActivateEvent& ev);

        virtual void onActivateEvent(const ActivateEvent& ev);


        virtual void onProcessMoveEvent(const MoveEvent& ev);

        virtual void onProcessResizeEvent(const ResizeEvent& ev);

       
        
        virtual void onProcessWindowStateEvent(const WindowStateEvent& ev);

        virtual void onWindowStateEvent(const WindowStateEvent& ev);


        virtual void onProcessCloseEvent(const CloseEvent& ev);

        virtual void onCloseEvent(const CloseEvent& ev);


        virtual void onShowSystemMenu(Window& w, bool v);

        virtual void onShowCloseButton(Window& w, bool v);

        virtual void onShowMinimizeButton(Window& w, bool v);

        virtual void onShowMaximizeButton(Window& w, bool v);

    //Helper
    private:
        void create(WindowType type);

        void destroy();

        bool isMinimized();

        bool isMaximized();

        void updateMotifWmHints();

     //Member
    private:
        ScreenImpl&    _wm;
        Window&        _client;
        ::Window       _window;
        ::Display*     _display;
        bool           _hasFirstShow;
        int            _width;
        int            _height;
        bool           _showClose;
        bool           _showMinimize;
        bool           _showMaximize;
        bool           _showSystemMenu;
};

}}}

#endif

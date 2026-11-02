#include "ScreenImpl.h"
#include "ApplicationImpl.h"
#include "WindowFrameImpl.h"

#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/PaintEvent.h>

namespace wildcat {
namespace gui {
namespace window {

ScreenImpl::ScreenImpl(ApplicationImpl& app)
: _parent(0)
, _display(app.display())
, _screenScaling(1.0)
{
}


ScreenImpl::~ScreenImpl()
{
    while( ! _windows.empty() )
        _windows.back()->unparent();

    setParent(0);
}


void ScreenImpl::setParent(Screen* screen)
{
    _parent = screen;

    if(_parent)
    {
        ::Screen * screen = XDefaultScreenOfDisplay(_display);
        int w = WidthOfScreen(screen);
        int h = HeightOfScreen(screen);
    
        gfx::Size size(w, h);
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
    return _windows;
}


WindowManagerBase& ScreenImpl::windowManager()
{
    return *this;
}


gfx::Point ScreenImpl::toFrame(const WindowFrameImpl& frame,
                                const gfx::Point& pos) const
{
    return frame.fromScreen(pos);
}


gfx::Point ScreenImpl::fromFrame(const WindowFrameImpl& frame,
                                  const gfx::Point& pos) const
{
    return frame.toScreen(pos);
}

///////////////////////////////////////////////////////////////////////
// Element
///////////////////////////////////////////////////////////////////////

Element* ScreenImpl::onHitTest(const gfx::Point& p, HitTestHint hint)
{
    // TODO: stacking order, visibility...
    //       XQueryPointer

    std::vector<Window*>::reverse_iterator it;
    for(it = _windows.rbegin(); it != _windows.rend(); ++it)
    {
        Window* win = *it;
        WindowFrameImpl* frame = static_cast<WindowFrameImpl*>( win->frame() );

        gfx::Point pos = toFrame(*frame, p);

        Element* hit = win->hitTest(pos);
        if(hit)
            return hit;
    }

    return 0;
}


gfx::Point ScreenImpl::onToParent(const gfx::Point& pos) const
{
    return pos + position();;
}


gfx::Point ScreenImpl::onFromParent(const gfx::Point& pos) const
{
    return pos - position();
}


bool ScreenImpl::onProcessEvent(const Pt::Event& ev)
{
    return Base::onProcessEvent(ev);
}


void ScreenImpl::onRequestRepaint(const gfx::Rect& rect)
{
    if(_parent)
        _parent->repaint(rect);
}

///////////////////////////////////////////////////////////////////////
// WindowManager
///////////////////////////////////////////////////////////////////////

WindowFrameBase* ScreenImpl::onAttach(Window& w)
{
    WindowFrameImpl* frame = new WindowFrameImpl(*this, w);

    _windows.push_back(&w);

    return frame;
}


void ScreenImpl::onDetach(WindowFrameBase& frame)
{
    Window& w = frame.window();

    std::vector<Window*>::iterator it;
    it = std::remove(_windows.begin(), _windows.end(), &w);
    _windows.erase(it, _windows.end());
}


void ScreenImpl::onInit(WindowFrameBase& frame)
{
    
}


void ScreenImpl::onRelease(WindowFrameBase& frame)
{
}

void ScreenImpl::setCapture(Element* capture)
{
    //std::clog << "RELEASE POINTER " << std::endl;
    Display* display = Application::instance().impl()->display();
    ::XUngrabPointer(display, CurrentTime);

    if( ! capture )
        return;

    std::vector<Window*>::iterator wit;
    for(wit = _windows.begin(); wit != _windows.end(); ++wit)
    {
        Window* window = *wit;

        if( capture == window || capture->isDescendantOf(*window) )
        {
            WindowFrameImpl* impl = static_cast<WindowFrameImpl*>( window->frame() );

            Display* display = Application::instance().impl()->display();

            XGrabPointer(display, impl->nativeWindow(), False,
                         ButtonPressMask|ButtonReleaseMask|
                         ButtonMotionMask|PointerMotionMask,
                         GrabModeAsync, GrabModeAsync,
                         None, None, CurrentTime);

            //std::clog << "GRAB POINTER: " << typeid(*capture).name() << " " << impl->window() << std::endl;
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////

void ScreenImpl::onSetScaleFactor(double v)
{
    Base::onSetScaleFactor(v);

    ::Screen * screen = XDefaultScreenOfDisplay(_display);
    int w = WidthOfScreen(screen);
    int h = HeightOfScreen(screen);
    
    gfx::Size size(w, h);
    size /= scaleFactor();

    _parent->onResize(*this, size);
}


//void ScreenImpl::onRepaint(Window& w, const gfx::Rect& rect)
//{
//    gfx::Point screenPos = onFromWindow( w, rect.topLeft() );
//    gfx::Rect screenRect( screenPos, rect.size() );
//
//    repaint(screenRect);
//}


void ScreenImpl::onProcessPaintEvent(const PaintEvent& ev)
{
    const gfx::Rect& screenRect = ev.rect();

    Base::onProcessPaintEvent(ev);

    //
    // paint child windows
    //
    std::vector<Window*>::iterator it;
    for(it = _windows.begin(); it != _windows.end(); ++it)
    {
        Window* window = *it;
        WindowFrameImpl* frame = static_cast<WindowFrameImpl*>( window->frame() );

        gfx::Point winPos = toFrame( *frame, screenRect.topLeft() );
        gfx::Rect winRect( winPos, screenRect.size() );

        winRect = winRect.intersect( gfx::Rect( window->size() ) );

        // send (native) paint event to window
        winRect = gfx::Rect( winRect.topLeft() * window->scaleFactor(),
                              winRect.size() * window->scaleFactor());

        frame->paint(winRect);
    }
}


void ScreenImpl::onPaintEvent(const PaintEvent& ev)
{
    Base::onPaintEvent(ev);

    const gfx::Rect& rect = ev.rect();
    onPaint(rect);
}


void ScreenImpl::onPaint(const gfx::Rect& rect)
{
}


void ScreenImpl::onProcessEnableEvent(const EnableEvent& ev)
{
    Base::onProcessEnableEvent(ev);

    for( size_t i = 0; i < _windows.size(); ++i)
    {
        Window* w = _windows[i];
        WindowFrameImpl* frame = static_cast<WindowFrameImpl*>( w->frame() );

        frame->onEnable(*w, ev.enabled() );
    }
}


void ScreenImpl::onEnableEvent(const EnableEvent& ev)
{    
    Base::onEnableEvent(ev);
}


void ScreenImpl::onEnable(bool e)
{
    Base::onEnable(e);
}


bool ScreenImpl::onProcessMouseEvent(const MouseEvent& ev)
{
    return ev.element()->processEvent(ev);
}


bool ScreenImpl::onMouseEvent(const MouseEvent& ev)
{
    // TODO: possibly pass on to application
    return Base::onMouseEvent(ev);
}


bool ScreenImpl::onProcessTouchEvent(const TouchEvent& ev)
{
    return ev.element()->processEvent(ev);
}


bool ScreenImpl::onTouchEvent(const TouchEvent& ev)
{
    // TODO: possibly pass on to application
    return Base::onTouchEvent(ev);
}


bool ScreenImpl::onProcessScrollEvent(const ScrollEvent& ev)
{
    return ev.element()->processEvent(ev);
}


bool ScreenImpl::onScrollEvent(const ScrollEvent& ev)
{
    // TODO: possibly pass on to application
    return Base::onScrollEvent(ev);
}


bool ScreenImpl::onProcessKeyEvent(const KeyEvent& ev)
{
    Element* visual = ev.element();
    if(visual)
        return visual->processEvent(ev);

    return false;
}
 

bool ScreenImpl::onKeyEvent(const KeyEvent& ev)
{
    // TODO: possibly pass on to application
    return Base::onKeyEvent(ev); 
}

}}}

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
#ifndef WILDCAT_GUI_WINDOW_SCREENIMPL_H
#define WILDCAT_GUI_WINDOW_SCREENIMPL_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/Screen.h>
#include <wildcat/gui/window/WindowManagerBase.h>
#include <wildcat/gui/window/LayoutEvent.h> 
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Rect.h>
#include <Pt/Signal.h>

#include <vector>
#include <Windows.h>

namespace wildcat {
namespace gui {
namespace window {

class ApplicationImpl;
class WindowFrameImpl;
class Window;
class MouseEvent;
class TouchEvent;
class ScrollEvent;
class KeyEvent;

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

        void setScaleFactor(double v)
        {
            _scaleFactor = v;
            updateScaling();

        }

        double scaleFactor() const
        {
            return _scaleFactor;
        }
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

        virtual void onUpdateScaling();

    // WindowManager
    protected:
        virtual WindowFrameBase* onAttach(Window& w);
    
        virtual void onDetach(WindowFrameBase& w);

        virtual void onInit(WindowFrameBase& w);

        virtual void onRelease(WindowFrameBase& w);

    // painting
    protected:
        void onProcessPaintEvent(const PaintEvent& ev);

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
        Screen*               _parent;
        std::vector<Window*>  _windows;
        double                _screenScaling;
        double                _scaleFactor;
};

}}}

#endif

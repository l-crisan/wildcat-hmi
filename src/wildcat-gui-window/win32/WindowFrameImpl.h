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
#ifndef WILDCAT_GUI_WINDOW_WIN32_WINDOWFRAMEIMPL_H
#define WILDCAT_GUI_WINDOW_WIN32_WINDOWFRAMEIMPL_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/WindowFrameBase.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Size.h>
#include <Windows.h>

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

        ~WindowFrameImpl();

        void setType(WindowType type);

        gfx::Point toScreen(const gfx::Point& pos) const;

        gfx::Point fromScreen(const gfx::Point& pos) const;

        void paint(const gfx::Rect& rect);

    //Nativ stuff
    public:
        HWND hwnd()
        {
            return _hwnd;
        }

    protected:
        virtual void onInit(Window& w);

        virtual gfx::Point onClientOffset() const;

        virtual void onRelease(Window& w);

        virtual gfx::Point onToWindow(const Window& w, 
                                       const gfx::Point& pos) const;

        virtual gfx::Point onFromWindow(const Window& w, 
                                         const gfx::Point& pos) const;

        virtual void onSetTitle(Window& w, const Pt::String& text);

        virtual void onSetIcon(Window& w, uint32_t icon);

        virtual void onSetState(Window& w, const WindowState& state);


        virtual void onShowSystemMenu(Window& w, bool v);

        virtual void onShowCloseButton(Window& w, bool v);

        virtual void onShowMinimizeButton(Window& w, bool v);

        virtual void onShowMaximizeButton(Window& w, bool v);

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

    //Helper
    private:

    //Member
    private:
        ScreenImpl&    _wm;
        Window&        _window;
        HWND           _hwnd;
        HICON          _iconHandle;
};

}}}

#endif

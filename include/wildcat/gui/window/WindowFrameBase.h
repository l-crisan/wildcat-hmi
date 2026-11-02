/*
   Copyright (C) 2017 Marc Boris Duerner
   Copyright (C) 2010-2026 by Laurentiu-Gheorghe Crisan
   Copyright (C) 2010-2026 by ATESiON GmbH

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
#ifndef WILDCAT_GUI_WINDOW_WINDOWFRAMEBASE_H
#define WILDCAT_GUI_WINDOW_WINDOWFRAMEBASE_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/Element.h>
#include <wildcat/gui/window/WindowType.h>

namespace wildcat {
namespace gui {
namespace window {

class WindowManagerBase;
class Window;

/** @internal @brief Window implementation base class.
*/
class WILDCAT_GUI_WINDOW_API WindowFrameBase : public Element
{
    typedef Element Base;

    friend class Window;

    public:
        WindowFrameBase(WindowManagerBase& wm, Window& window);

        virtual ~WindowFrameBase();

        Window& window();

        const Window& window() const;

        bool isActive() const
        {
            return _active;
        }

        virtual gfx::Point clientOffset() const;

    protected:
        
        virtual void onProcessActivateEvent(const ActivateEvent& ev);

        virtual void onActivateEvent(const ActivateEvent& ev);


        virtual void onProcessWindowStateEvent(const WindowStateEvent& ev);

        virtual void onWindowStateEvent(const WindowStateEvent& ev);

        
        virtual void onProcessCloseEvent(const CloseEvent& ev);

        virtual void onCloseEvent(const CloseEvent& ev);

    protected:
        virtual void onInit(Window& w) = 0;

        virtual void onRelease(Window& w) = 0;

        virtual gfx::Point onToWindow(const Window& w, 
                                       const gfx::Point& pos) const = 0;

        virtual gfx::Point onFromWindow(const Window& w, 
                                         const gfx::Point& pos) const = 0;

        virtual void onSetTitle(Window& w, const Pt::String& text) = 0;

        virtual void onSetIcon(Window& w, uint32_t icon) = 0;

        virtual void onSetState(Window& w, const WindowState& state) = 0;

        virtual gfx::Point onClientOffset() const = 0;

        virtual void onShowSystemMenu(Window& w, bool v) = 0;

        virtual void onShowCloseButton(Window& w, bool v) = 0;

        virtual void onShowMinimizeButton(Window& w, bool v) = 0;

        virtual void onShowMaximizeButton(Window& w, bool v) = 0;


        virtual void onSetAbove(Window& w, bool above) = 0;

        virtual void onSetSizeLimits(Window& w, const gfx::Size& minSize, 
                                                const gfx::Size& maxSize) = 0;

        virtual void onRepaint(Window& w, const gfx::Rect& rect) = 0;

        virtual void onShow(Window& w, bool visible) = 0;

        virtual void onActivate(Window& w, bool active) = 0;

        virtual void onEnable(Window& w, bool enable) = 0;

        virtual void onMove(Window& w, const gfx::Point& to) = 0;

        virtual void onResize(Window& w, const gfx::Size& s) = 0;

        virtual void onClose(Window& w) = 0;

    private:
        WindowManagerBase& _wm;
        Window&        _window;
        bool _active;
};

}}}

#endif

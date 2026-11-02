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
#ifndef WILDCAT_GUI_WINDOW_WINDOWMANAGER_H
#define WILDCAT_GUI_WINDOW_WINDOWMANAGER_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/WindowManagerBase.h>
#include <wildcat/gui/window/Element.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Rect.h>

namespace wildcat {
namespace gui {
namespace window {

class WindowFrame;
class MdiArea;

class WindowManager : public WindowManagerBase
{
    friend class WindowFrame;

    public:
        WindowManager();

        virtual ~WindowManager();

        void setParent(MdiArea* shell);

        virtual WindowBase* parentWindow();
        
        gfx::Graphics& graphics();

        const gfx::Graphics& graphics() const;

        void setGraphics(gfx::Graphics* graphics, const gfx::Point& pos);

        bool processMouseEvent(const MouseEvent& ev);

        bool processTouchEvent(const TouchEvent& ev);
    
    public:
        Window* activeWindow();

        const std::vector<Window*>& windows() const;

        int borderWidth() const
        {
            return _borderWidth;
        }

        int titleHeight()  const
        {
            return _titleHeight;
        }

        const gfx::Color& inactiveColor() const
        {
            return _inactiveColor;
        }

        const gfx::Color& activeColor() const
        {
            return _activeColor;
        }

        const gfx::Color& textColor() const
        {
            return _textColor;
        }

        const gfx::Color& inactiveTextColor() const
        {
            return _inactiveTextColor;
        }

        // WindowManagerase
    protected:
        virtual WindowFrameBase* onAttach(Window& w);

        virtual void onDetach(WindowFrameBase& w);

        virtual void onInit(WindowFrameBase& w);

        virtual void onRelease(WindowFrameBase& w);

    protected:
        gfx::Point toFrame(const WindowFrame& w,
                            const gfx::Point& pos) const;

        gfx::Point fromFrame(const WindowFrame& w,
                              const gfx::Point& pos) const;

        virtual void onSetAbove(WindowFrame& w, bool above);

        virtual void onSetSizeLimits(WindowFrame& w,
                                     const gfx::Size& minSize, 
                                     const gfx::Size& maxSize);

        virtual void onShow(WindowFrame& w, bool visible);

        virtual void onActivate(WindowFrame& w, bool active);

        virtual void onEnable(WindowFrame& w, bool enable);

        virtual void onMove(WindowFrame& w, const gfx::Point& to);

        virtual void onResize(WindowFrame& w, const gfx::Size& to);

        virtual void onClose(WindowFrame& w);
    
        virtual void onUpdateScaling();

    // Element    
    protected:
        virtual gfx::Point onToParent(const gfx::Point& pos) const;

        virtual gfx::Point onFromParent(const gfx::Point& pos) const;

        virtual Element* onHitTest(const gfx::Point& p, HitTestHint hint);

        
        virtual void onRequestRepaint(const gfx::Rect& rect);

        virtual void onRequestActivate(bool active);
        
        virtual void onRequestCapture(bool capture);


    
    // Implementation    
    protected:
        virtual void onProcessPaintEvent(const PaintEvent& ev);
        
        virtual void onProcessEnableEvent(const EnableEvent& ev);

        virtual void onResizeEvent(const ResizeEvent& ev);
        
        virtual void onMoveEvent(const MoveEvent& ev);

        virtual bool onProcessMouseEvent(const MouseEvent& ev);

        virtual bool onProcessTouchEvent(const TouchEvent& ev);

        virtual bool onProcessScrollEvent(const ScrollEvent& ev);

        virtual void onProcessEnterEvent(const EnterEvent& ev);

        virtual void onProcessLeaveEvent(const LeaveEvent& ev);

        virtual bool onProcessKeyEvent(const KeyEvent& ev);

    private:
        MdiArea*              _parent;
        gfx::RegionGraphics   _graphics;
        std::vector<Window*>  _windowList;
        Window*               _activeWindow;
        Window*               _topMostWindow;
        gfx::Size             _parentSize;
        int                   _borderWidth;
        int                   _titleHeight;
        gfx::Color            _activeColor;
        gfx::Color            _inactiveColor;
        gfx::Color            _textColor;
        gfx::Color            _inactiveTextColor;  
};

}}}

#endif

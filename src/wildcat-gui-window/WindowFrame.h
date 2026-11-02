/*
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
#ifndef WILDCAT_GUI_WINDOW_WINDOWFRAME_H
#define WILDCAT_GUI_WINDOW_WINDOWFRAME_H

#include "FrameButton.h"
#include <wildcat/gui/window/WindowFrameBase.h>
#include <wildcat/gui/window/Cursor.h>
#include <wildcat/gui/window/SystemColors.h>
#include <wildcat/gui/gfx/Graphics.h>
#include <wildcat/gui/gfx/RegionGraphics.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Rect.h>
#include <Pt/Signal.h>
#include <vector>

namespace wildcat {
namespace gui {
namespace window {

class Window;
class WindowManager;
class WindowFrame;
class Application;
class MouseEvent;
class KeyEvent;
class EnterEvent;
class LeaveEvent;
class PaintEvent;
class MoveEvent;
class ResizeEvent;

//ToDo: Maximize,Minimize double click on bar=> maximize
class WindowFrame : public WindowFrameBase
{
    typedef WindowFrameBase Base;

    enum FrameItem
    {
        OnNone,
        OnTitle,
        OnButton,
        OnFrameLeft,
        OnFrameTopLeft,
        OnFrameTop,
        OnFrameTopRight,
        OnFrameRight,
        OnFrameBottomRight,
        OnFrameBottom,
        OnFrameBottomLeft
    };

    public:
        WindowFrame(WindowManager& wm, Window& window);

        virtual ~WindowFrame();

        //virtual gfx::Point clientPos() const;

        const gfx::Point& restorePosition() const;

        const gfx::Size& restoreSize() const;
        
        void setRestore(const gfx::Point& pos, const gfx::Size& size);

        const gfx::Rect& clientRect() const;

        const gfx::Rect& frameRect() const;

        void setFrame(int bw, int th);

        const SystemColors& colors() const;

    protected: //Element API
        virtual bool onProcessMouseEvent(const MouseEvent& mev);

        virtual  bool onProcessTouchEvent(const TouchEvent& tev);

    protected: //Frame base api
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

        virtual void onSetAbove(Window& w, bool above);

        virtual void onSetSizeLimits(Window& w, const gfx::Size& minSize, 
                                                const gfx::Size& maxSize);

        virtual void onRepaint(Window& w, const gfx::Rect& rect);

        virtual void onShow(Window& w, bool visible);

        virtual void onActivate(Window& w, bool visible);

        virtual void onEnable(Window& w, bool enable);

        virtual void onMove(Window& w, const gfx::Point& to);

        virtual void onResize(Window& w, const gfx::Size& s);

        virtual void onShowSystemMenu(Window& w, bool v);

        virtual void onShowCloseButton(Window& w, bool v);

        virtual void onShowMinimizeButton(Window& w, bool v);

        virtual void onShowMaximizeButton(Window& w, bool v);

        virtual void onClose(Window& w);

        virtual void onInvalidate();

    protected:
        virtual Element* onHitTest(const gfx::Point& pos, HitTestHint hint);

        virtual gfx::Point onToParent(const gfx::Point& pos) const;
        
        virtual gfx::Point onFromParent(const gfx::Point& pos) const;

        virtual void onRequestRepaint(const gfx::Rect& rect);

        virtual void onRequestMove(const gfx::Point& pos);

        virtual void onRequestResize(const gfx::Size& size);
        
        virtual bool onProcessEvent(const Pt::Event& ev);

        
    protected:
        virtual void onProcessPaintEvent(const PaintEvent& ev);

        virtual void onPaintEvent(const PaintEvent& ev);
        

        virtual void onProcessShowEvent(const ShowEvent& ev);

        virtual void onShowEvent(const ShowEvent& ev);

        
        virtual void onProcessEnableEvent(const EnableEvent& ev);

        virtual void onEnableEvent(const EnableEvent& ev);


        virtual void onProcessActivateEvent(const ActivateEvent& ev);

        virtual void onActivateEvent(const ActivateEvent& ev);


        virtual void onProcessMoveEvent(const MoveEvent& ev);

        virtual void onMoveEvent(const MoveEvent& ev);

        
        virtual void onProcessResizeEvent(const ResizeEvent& ev);

        virtual void onResizeEvent(const ResizeEvent& ev);

        
        virtual void onProcessWindowStateEvent(const WindowStateEvent& ev);

        virtual void onWindowStateEvent(const WindowStateEvent& ev);

        
        virtual void onProcessCloseEvent(const CloseEvent& ev);

        virtual void onCloseEvent(const CloseEvent& ev);

    protected:
        virtual bool onMouseEvent(const MouseEvent& mev);

        virtual bool onTouchEvent(const TouchEvent& tev);

        virtual bool onEnterEvent( const EnterEvent& ev);

        virtual bool onLeaveEvent(const LeaveEvent& ev);

    protected:
        void onLayout();

        void onMenu();

        void onMinimize();

        void onMaximize();

        void onCloseButton();

    private:
        bool isTitle(const gfx::Point& p) const;

        bool isLeftBorder(const gfx::Point& p) const;
        
        bool isRightBorder(const gfx::Point& p) const;
        
        bool isTopBorder(const gfx::Point& p) const;
        
        bool isBottomBorder(const gfx::Point& p) const;

        Window* checkWindow(const gfx::Point& pos);

        FrameButton* checkButton(const gfx::Point& pos);

        //! @brief Returns true if window was grabbed for moving or resizing.
        bool checkMove(const gfx::Point& pos, bool isDrag, bool isPress);

        //! @brief Returns true if window was grabbed for moving or resizing.
        bool checkResize(const gfx::Point& pos, bool isDrag, bool isPress);

        void setCurrentFrameItem(FrameItem item);
              

    private:
        WindowManager*       _wm;
        Window*        _window;
        int         _borderWidth;
        int         _titleHeight;
        gfx::Rect     _frameRect;
        gfx::Rect     _frameBounds;
        gfx::Rect     _clientRect;
        gfx::Rect     _clientBounds;
        gfx::Point    _restorePos;
        gfx::Size     _restoreSize;
        WindowState    _state;

        gfx::Point    _lastPointer;
        bool           _isClient;
        bool           _isMoving;
        bool           _isLeftResizing;
        bool           _isRightResizing;
        bool           _isTopResizing;
        bool           _isBottomResizing;

        FrameItem      _currentFrameItem;

        std::vector<FrameButton*> _buttons;

        MaximizeButton _maximizeButton;
        MinimizeButton _minimizeButton;  
        CloseButton    _closeButton;
        MenuButton     _menuButton;
        gfx::Rect _titleArea;

};

}}}

#endif
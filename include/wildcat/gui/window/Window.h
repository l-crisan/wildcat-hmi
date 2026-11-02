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
#ifndef WILDCAT_GUI_WINDOW_WINDOW_H
#define WILDCAT_GUI_WINDOW_WINDOW_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/WindowBase.h>
#include <wildcat/gui/window/WindowType.h>
#include <wildcat/gui/window/ActivateEvent.h>
#include <wildcat/gui/window/CloseEvent.h>
#include <wildcat/gui/window/ResizeEvent.h>
#include <wildcat/gui/window/KeyEvent.h>
#include <wildcat/gui/window/MouseEvent.h>
#include <wildcat/gui/window/TouchEvent.h>
#include <wildcat/gui/window/ScrollEvent.h>
#include <wildcat/gui/window/MoveEvent.h>
#include <wildcat/gui/window/EnterEvent.h>
#include <wildcat/gui/window/LeaveEvent.h>
#include <wildcat/gui/window/ShowEvent.h>
#include <wildcat/gui/window/EnableEvent.h>
#include <wildcat/gui/window/InvalidateEvent.h>

#include <wildcat/gui/base/TypeFactory.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <Pt/Signal.h>

namespace wildcat {
namespace gui {
namespace window {

class WindowFrameBase;
class WindowManagerBase;
class WindowStateEvent;
class PaintEvent;

/** @brief Window base class.
*/
class WILDCAT_GUI_WINDOW_API Window : public WindowBase
{
    public:
        typedef WindowBase Base;
        typedef WindowType Type;
        typedef WindowState State;

    public:
        explicit Window(WindowManagerBase* parent = 0,
                        WindowType type = WindowType::Default);

        virtual ~Window();

    public:
        void setParent(WindowManagerBase& parent);

        void unparent();

        WindowManagerBase* windowManager();

        const WindowManagerBase* windowManager() const;

        WindowFrameBase* frame();

        const WindowFrameBase* frame() const;

    public:
        gfx::Pixmap getImage() const;

        bool isModal() const
        {
            return _isModal;
        }
    public:        
        

        WindowState state() const;

        void setState(const WindowState& s);

        WindowPositon startPostion() const
        {
            return _startPostion;
        }

        void setStartPosition(WindowPositon v)
        {
            _startPostion = v;
        }

        Type frameType() const
        {
            return _type;
        }

        void setFrameType(Type v)
        {
            _type = v;
            invalidate();
        }

        bool systemMenuVisible() const
        {
            return _showSystemMenu;
        }

        void setShowSystemMenu(bool v);



        bool closeButtonVisible() const
        {
            return _showCloseButton;
        }
        void setShowCloseButton(bool v);



        bool minimizeButtonVisible() const
        {
            return _showMinimizeButton;
        }
        void setShowMinimizeButton(bool v);


        bool maximizeButtonVisible() const
        {
            return _showMaximizeButton;
        }

        void setShowMaximizeButton(bool v);


        bool isAbove() const;

        void setAbove(bool top);


  
        bool isActive() const;


        virtual int showModal();

        bool isClosed() const;

        void close();

        bool acceptsInput() const;

    public:
        wildcat::gui::base::Signal& closeSignal()
        {
            return _closeSignal;
        }   

    public:
        const gfx::Color& background() const
        {
            return colors().control();
        }

        void setBackground(const gfx::Color& b)
        {
            colors().setControl(b);
            invalidate();
        }

    protected:

        virtual WindowBase* onTopParentWindow();
    //
    // Element
    //
    protected:
        Element* onHitTest(const gfx::Point& p, HitTestHint hint);

        virtual gfx::Point onToParent(const gfx::Point& pos) const;

        virtual gfx::Point onFromParent(const gfx::Point& pos) const;

        virtual bool onProcessEvent(const Pt::Event& ev);

        virtual void onRequestRepaint(const gfx::Rect& rect);

        virtual void onRequestEnable(bool isEnable);

        virtual void onRequestActivate(bool active);

        virtual void onRequestShow(bool isShown);

        virtual void onRequestMove(const gfx::Point& pos);

        virtual void onRequestResize(const gfx::Size& s);

        virtual void onSetSizeLimits(const gfx::Size& minSize,
                                     const gfx::Size& maxSize);

        virtual void onUpdateScaling(); 
    //
    // invalidation
    //
    protected:
        virtual void onInvalidateEvent(const InvalidateEvent& ev);

        virtual void onInvalidate();

    //
    // painting
    //
    protected:
        virtual void onProcessPaintEvent(const PaintEvent& ev);

        virtual void onPaintEvent(const PaintEvent& ev);
    

    //
    // geometry
    //
    protected:
        virtual void onProcessMoveEvent(const MoveEvent& ev);

        virtual void onMoveEvent(const MoveEvent& ev);

        virtual void onProcessResizeEvent(const ResizeEvent& ev);

        virtual void onResizeEvent(const ResizeEvent& ev);

    //
    // visibility
    //
    protected:
        virtual void onProcessShowEvent(const ShowEvent& ev);

        virtual void onShowEvent(const ShowEvent& ev);

        virtual void onShow(bool visible);

    //
    // enabling
    //
    protected:
        virtual void onProcessEnableEvent(const EnableEvent& ev);

        virtual void onEnableEvent(const EnableEvent& ev);

        virtual void onEnable(bool e);

        virtual void onSetEnabledState(bool v);

    //
    // activation
    //
    protected:
        virtual void onProcessActivateEvent(const ActivateEvent& ev);

        virtual void onActivateEvent(const ActivateEvent& ev);

    //
    // window state
    //
    protected:
        virtual void onProcessWindowStateEvent(const WindowStateEvent& ev);

        virtual void onWindowStateEvent(const WindowStateEvent& ev);

    //
    // closing
    //
    protected:
        virtual void onProcessCloseEvent(const CloseEvent& ev);

        virtual void onCloseEvent(const CloseEvent& ev);

    //
    // input
    //
    protected:
        virtual bool onProcessMouseEvent(const MouseEvent& ev);

        virtual bool onProcessTouchEvent(const TouchEvent& ev);

        virtual bool onProcessScrollEvent(const ScrollEvent& ev);

        virtual void onProcessEnterEvent(const EnterEvent& ev);

        virtual void onProcessLeaveEvent(const LeaveEvent& ev);

        virtual bool onProcessKeyEvent(const KeyEvent& ev);
    
    protected:
        virtual bool onMouseEvent(const MouseEvent& ev);

        virtual bool onTouchEvent( const TouchEvent& ev );
    
        virtual bool onScrollEvent(const ScrollEvent& ev);

        virtual bool onKeyEvent(const KeyEvent& ev);

        virtual bool onEnterEvent(const EnterEvent& ev);

        virtual bool onLeaveEvent(const LeaveEvent& ev);

        virtual void onSetIcon(uint32_t icon);

        

    private:
        void onTitleChanged(Object& obj, Pt::Any& param);  

    private:


        int stateI() const
        {
            return (int)state();
        }

        void setStateI(int s)
        {
            WindowState ss((WindowState::State)s);
            setState(ss);
        }


        int  startPostionI() const
        {
            return (int)startPostion();
        }

        void setStartPositionI(int v)
        {
            WindowPositon ss((WindowPositon::Position)v);
            setStartPosition(ss);
        }

        int frameTypeI() const
        {
            return (int)frameType();
        }

        void setFrameTypeI(int v)
        {
            setFrameType((WindowType::Type)v);
        }

    private:
        WindowFrameBase*             _frame;
        WindowManagerBase*           _wm;

        bool                         _show; 
        bool                         _isActive;
        bool                         _requestEnabled; 
        bool                         _isClosed; 

        bool                         _autoSize;

        Type                         _type;
        State                        _state;
        bool                         _isAbove;
           
        WindowPositon                _startPostion;
        bool                         _showSystemMenu;
        bool                         _showCloseButton;
        bool                         _showMinimizeButton;
        bool                         _showMaximizeButton;
        bool                        _isModal;   

        wildcat::gui::base::Signal  _closeSignal;

    public:
        static base::TypeInit<Window> _typeInit;
        
};



}}}

#endif

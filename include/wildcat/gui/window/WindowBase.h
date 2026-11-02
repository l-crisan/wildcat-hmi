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
#ifndef WILDCAT_GUI_WINDOW_WINDOWBASE_H
#define WILDCAT_GUI_WINDOW_WINDOWBASE_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/Element.h>
#include <wildcat/gui/window/Widget.h>
#include <wildcat/gui/gfx/PixmapGraphics.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Rect.h>

namespace wildcat {
namespace gui {
namespace window {

class WILDCAT_GUI_WINDOW_API WindowBase : public View
{
    friend class Control;

    typedef View Base;

    public:
        virtual ~WindowBase();

    public:
        Control* content();

        const Control* content()  const;

        void setContent(Control* widget);       

        bool scalePosition() const
        {
            return _scalePosition;
        }

        void setScalePosition(bool v)
        {
            _scalePosition = v;
        }


        gfx::Graphics& graphics();

        const gfx::Graphics& graphics() const;

        WindowBase* topParentWindow()
        {
            return onTopParentWindow();
        }


        SystemColors& colors()
        {
            return _colors;
        }   

        const SystemColors& colors() const
        {
            return _colors;
        }
    public:
        Control* focusWidget();

        void focusNext();

        void focusPrev();

    protected:
        WindowBase();

        void relayout();

    protected:

        virtual gfx::Size onMeasure();

        virtual void onProcessLayoutEvent(const LayoutEvent& ev);
        
        virtual void onLayoutEvent(const LayoutEvent& ev);        

        void onUpdateScaling();

    protected:
        virtual void onAddElement(Control& widget);

        virtual void onRemoveElement(Control& widget);

        virtual void onSetFocusPolicy(Control& w, FocusPolicy policy);

        virtual void onSetFocusIndex(Control& w, unsigned index);

        virtual void onSetFocus(Control& w);

        virtual void onSetShortcut(Control& w, const std::vector<Key>& keys);

        virtual void onSetMnemonic(Control& w, const std::vector<Pt::Char>& chs);

        virtual WindowBase* onTopParentWindow() = 0;
    
        void onSetResourceManager(base::ResourceManager& v);

    //
    // View
    //
    protected:

        virtual void onAttach(Control& widget);

        virtual void onDetach(Control& widget);

        virtual void onInit(Control& widget);

        virtual void onRelease(Control& widget);

        virtual gfx::Point onToControl(const Control& widget, 
                                        const gfx::Point& pos) const;

        virtual gfx::Point onFromControl(const Control& widget, 
                                          const gfx::Point& pos) const;

    protected:
        virtual void onRepaintRequest(Control& widget, const gfx::Rect& rect);

        virtual void onRelayoutRequest(Control& widget);

        virtual void onEnableRequest(Control& widget, bool isEnable);

        virtual void onActivateRequest(Control& w, bool active);

        virtual void onShowRequest(Control& widget, bool isShown);

        virtual void onMoveRequest(Control& widget, const gfx::Point& pos);

        virtual void onResizeRequest(Control& widget, const gfx::Size& size);

        virtual void onRaiseRequest(Control& widget);
        

    //
    // Element
    //
    protected:
        virtual Element* onHitTest(const gfx::Point& pos, HitTestHint hint);

        virtual void onRequestCapture(bool capture);

    protected:
        virtual bool onProcessEvent(const Pt::Event& ev);

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
    // enabling
    //
    protected:
        virtual void onProcessEnableEvent(const EnableEvent& ev);

        virtual void onEnableEvent(const EnableEvent& ev);

        virtual void onSetEnabledState(bool v);

        virtual void onEnable(bool e);

    //
    // visibility
    //
    protected:
        virtual void onProcessShowEvent(const ShowEvent& ev);

        virtual void onShowEvent(const ShowEvent& ev);

        virtual void onShow(bool visible);

    //
    // geometry
    //
    protected:
        virtual void onProcessMoveEvent(const MoveEvent& ev);

        virtual void onMoveEvent(const MoveEvent& ev);

        virtual void onProcessResizeEvent(const ResizeEvent& ev);
        
        virtual void onResizeEvent(const ResizeEvent& ev);
    //
    // input
    //
    protected:
        virtual bool onProcessMouseEvent(const MouseEvent& ev);
        
        virtual bool onProcessTouchEvent(const TouchEvent& ev);

        virtual bool onProcessScrollEvent(const ScrollEvent& sev);

        virtual void onProcessEnterEvent(const EnterEvent& ev);

        virtual void onProcessLeaveEvent(const LeaveEvent& ev);

        virtual bool onProcessKeyEvent(const KeyEvent& ev);

    //
    // Responder
    //
    protected:
        virtual bool onMouseEvent(const MouseEvent& ev);
        
        virtual bool onTouchEvent(const TouchEvent& ev);
        
        virtual bool onScrollEvent(const ScrollEvent& ev);

        virtual bool onEnterEvent(const EnterEvent& ev);

        virtual bool onLeaveEvent(const LeaveEvent& ev);

        virtual bool onKeyEvent(const KeyEvent& ev);
    
    private:
        template <typename Iter>
        void moveFocus(Iter begin, Iter end);

        Object* contentI() const
        {
            const Control* w = content();

            return const_cast<Control*>(w);
        }

        void setContentI(Object* w)
        {
            setContent((Control*)w);
        }



    protected:

        const std::map<Key, Control*>& shortcuts() const
        {
            return _shortcuts;
        }

        const std::map<Pt::Char, Control*>& mnemonics() const
        {
            return _mnemonics;
        }


    private:
        gfx::PixmapGraphics           _graphics;
        Control*                      _mainWidget;
        int                          _layouts;
        Control*                      _active;
        std::vector<Control*>         _focusList;
        Control*                      _focusWidget ;
        std::map<Key, Control*>       _shortcuts;
        std::map<Pt::Char, Control*>  _mnemonics;
        SystemColors                  _colors;  
        bool                          _scalePosition;


};

}}}

#endif

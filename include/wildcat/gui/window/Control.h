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
#ifndef WILDCAT_GUI_WINDOW_CONTROL_H
#define WILDCAT_GUI_WINDOW_CONTROL_H

#include <wildcat/gui/window/View.h>
#include <wildcat/gui/window/Spacing.h>
#include <wildcat/gui/window/MouseEvent.h>
#include <wildcat/gui/window/TouchEvent.h>
#include <wildcat/gui/window/ScrollEvent.h>
#include <wildcat/gui/window/KeyEvent.h>
#include <wildcat/gui/window/ResizeEvent.h>
#include <wildcat/gui/window/MoveEvent.h>
#include <wildcat/gui/window/PaintEvent.h>
#include <wildcat/gui/window/EnterEvent.h>
#include <wildcat/gui/window/LeaveEvent.h>
#include <wildcat/gui/window/EnableEvent.h>
#include <wildcat/gui/window/InvalidateEvent.h>
#include <wildcat/gui/window/LayoutEvent.h>
#include <wildcat/gui/window/ShowEvent.h>
#include <wildcat/gui/window/FocusEvent.h>
#include <wildcat/gui/gfx/RegionGraphics.h>
#include <wildcat/gui/base/Signal.h>
#include <wildcat/gui/base/ResourceManager.h>
#include <Pt/Signal.h>
#include <Pt/Delegate.h>
#include <Pt/String.h>
#include <vector>

namespace wildcat {
namespace gui {
namespace window {


constexpr char* DockModeProperty = "DockPosition";

class WindowBase;
class Key;

typedef View ControlBase;

class WILDCAT_GUI_WINDOW_API Control : public ControlBase
{
    friend class WindowBase;
    
    public:
        enum DockPosition
        {            
            DockPosLeft,
            DockPosRight,
            DockPosTop,            
            DockPosBottom,
            DockPosCenter,
            DockPosEnd,
            DockPosNone = -1,            
        };


        enum DockPermision
        {
            DockPerNone = 0,
            DockPerLeft = 1,
            DockPerRight = 2,
            DockPerTop = 4,
            DockPerBottom = 8,
            DockPerCenter = 16,
            DockPerAll = 0x1F   
        };

    public:
        Control();

        virtual ~Control();
        
        DockPosition dockPosition() const
        {
            return _dockPosition;
        }

        void setDockPosition(DockPosition v);
        

        bool contains(const Control* control) const;    


        const Pt::String& toolTip() const
        {
            return onGetToolTip();
        }

        const Pt::String& trToolTip() const
        {
            return _trToolTip;
        }

        void setToolTip(const Pt::String& text);
  
        void showToolTip(const gfx::Point& pos);    

        void closeToolTip();    


        int dockPermission() const
        {
            return _dockPermission;
        }   

        void setDockPermission(int p);
        

        void addDockPermission(DockPermision p)
        {
            _dockPermission |= (int)p;
        }

        void removeDockPermission(DockPermision p)
        {
            _dockPermission &= (int)~p;
        }

        bool hasDockPermission(DockPermision p)
        {
            return (_dockPermission & (int)p) != 0;
        }   

        void bringToFront(Control& w);  

        void bringToBack(Control& w);


      
    public:
        //Items

        bool empty() const
        {
            return onIsEmpty();
        }


        uint32_t childrenCount() const
        {
            return onChildrenCount();
        }

        Control* childAt(size_t n)
        {
            return onChildAt(n);
        }

        const Control* childAt(size_t n) const
        {
            return onChildAt(n);
        }

    protected:

        virtual bool onIsEmpty() const
        {
            return childrenCount() == 0;
        }

        virtual uint32_t onChildrenCount() const;

        virtual Control* onChildAt(size_t n);

        virtual const Control* onChildAt(size_t n) const;

        virtual void onUpdateScaling();

        virtual const Pt::String& onGetToolTip() const;

        virtual void onSetToolTip(const Pt::String& text);  
    public:
        void setParent(View& parent);

        void unparent();
        
        void add(Control& widget);
       
        void remove(Control& widget);

        void remove(uint32_t n)
        {
            Control* w = childAt(n) ;

            if (w)
                remove(*w);
        }

        void removeAll();

        int indexOf(Control& w) const
        {
            std::vector<wildcat::gui::window::Control*>::const_iterator it;
            it = std::find(children().begin(), children().end(), &w);

            return (int)(it != children().end() ? std::distance(children().begin(), it) : -1);
        }

        const std::vector<Control*>& children() const;

    public:
        gfx::Graphics& graphics();

        const gfx::Graphics& graphics() const;

        void setGraphics(gfx::Graphics* graphics, const gfx::Point& pos);

        const WindowBase* window() const
        {
            return _form;
        }

        WindowBase* window()
        {
            return _form;
        }

        WindowBase* topParentWindow();
       

    public:
        // Focus handling
        FocusPolicy focusPolicy() const;

        void setFocusPolicy(FocusPolicy f);

        size_t focusIndex() const;

        void setFocusIndex(size_t index);  

        bool hasFocus() const;

        void focus();

        // Keyboard input
        Key actionKey() const;

        void setActionKey(Key ak);

        const Key& shortcut() const;

        void setShortcut(const Key& k);

        const Pt::Char& mnemonic() const; 

        void setMnemonic(const Pt::Char& ch);

        Pt::String setMnemonic(const Pt::String& text);

        void setMnemonicWidget(Control* w);

        bool processShortcut(const Key& key);

        void processMnemonic(Pt::Char m);

        void updateSize();

        // Layouting    
        const gfx::Rect geometry() const;

        gfx::Size preferredSize() const;

        gfx::Size measure(const gfx::Size& parentSize);

        void relayout();

        //Margin
        const Spacing& margin() const;

        void setMargin(const Spacing& s);

        void setMargin(int n);

        void setMargin(int horiz, int vertical);

        //Padding
        const Spacing& padding() const;

        void setPadding(const Spacing& p);

        void setPadding(int n);

        void setPadding(int horiz, int vertical);

      public:
        // TODO: obsolete when processEvent return true/false if consumed
        bool acceptsInput() const;


        void raise();

    public://Signals
        wildcat::gui::base::Signal& layoutSignal()
        {
            return _layoutSignal;
        }

        wildcat::gui::base::Signal& paddingChanged()
        {
            return _paddingChanged;        
        }

        wildcat::gui::base::Signal& marginChanged()
        {
            return _marginChanged;
        }

        wildcat::gui::base::Signal& dockPositionChanged()
        {
            return _dockPositionChanged;
        }

        wildcat::gui::base::Signal& dockPermissionChanged()
        {
            return _dockPermissionChanged;
        }


        wildcat::gui::base::Signal& toolTipChanged()
        {
            return _toolTipChanged;
        }

        wildcat::gui::base::Signal& textChanged()
        {
            return _textChanged;
        }   

        wildcat::gui::base::Signal& focusPolicyChanged()
        {
            return _focusPolicyChanged;
        }

        wildcat::gui::base::Signal& focusIndexChanged()
        {
            return _focusIndexChanged;
        }

        wildcat::gui::base::Signal& actionKeyChanged()
        {
            return _actionKeyChanged;
        }

        wildcat::gui::base::Signal& shortcutKeyChanged()
        {
            return _shortcutKeyChanged;
        }

        wildcat::gui::base::Signal& mnemonicChanged()
        {
            return _mnemonicChanged;
        }

    // Invalidation
    protected:
        virtual void onInvalidateEvent(const InvalidateEvent& ev);
    
        virtual void onInvalidate();
    
    //ToolTip
    protected:
         virtual void onShowToolTip(const gfx::Point& pos);

         virtual void onCloseToolTip();

    // Painting    
    protected:
        virtual void onProcessPaintEvent(const PaintEvent& ev);

        virtual void onPaintEvent(const PaintEvent& ev);
        
        virtual void onPaint(gfx::Graphics& surface, const gfx::Rect& rect);

    
    protected:
        virtual gfx::Size onMeasure(const gfx::Size& parentSize);
       
        virtual void onProcessLayoutEvent(const LayoutEvent& ev);

        virtual void onLayoutEvent(const LayoutEvent& ev);

        virtual void onLayout(const gfx::Rect& rect);

    protected:
        virtual void onSetSizeLimits(const gfx::Size& minSize,
                                     const gfx::Size& maxSize);

    protected:
        virtual void onSetGraphics(gfx::Graphics* graphics, const gfx::Point& pos);

        virtual void onAddChild(Control& w);

        virtual void onRemoveChild(Control& w);


        virtual void onActionKey(const KeyEvent& kev);

        virtual void onSetActionKey(Key ak);

        virtual Key onGetActionKey() const;



        virtual bool onShortcut(const Key& kev);

        virtual void onSetShortcut(const Key& k);
        
        virtual const Key& onGetShortcut() const;

        virtual void onMnemonic(Pt::Char m);

        virtual void onSetMnemonic(const Pt::Char& m);

        virtual const Pt::Char& onGetMnemonic() const;

        // Element
    protected:
        virtual Element* onHitTest(const gfx::Point& p, HitTestHint hint) ;

        virtual gfx::Point onToParent(const gfx::Point& pos) const;

        virtual gfx::Point onFromParent(const gfx::Point& pos) const;
        
        
        virtual void onRequestRepaint(const gfx::Rect& rect);

        virtual void onRequestShow(bool isShown);

        virtual void onRequestEnable(bool isEnable);

        virtual void onRequestActivate(bool active);

        virtual void onRequestMove(const gfx::Point& pos);

        virtual void onRequestResize(const gfx::Size& s);

        
        virtual bool onProcessEvent(const Pt::Event& ev);


        virtual void onProcessEnableEvent(const EnableEvent& ev);

        virtual void onEnableEvent(const EnableEvent& ev);
        
        virtual void onEnable(bool isEnable);


        virtual void onProcessShowEvent(const ShowEvent& ev);

        virtual void onShowEvent(const ShowEvent& ev);

        virtual void onShow(bool visible);


        virtual void onProcessFocusEvent(const FocusEvent& ev);

        virtual void onFocusEvent(const FocusEvent& ev);


        virtual void onProcessMoveEvent(const MoveEvent& ev);

        virtual void onMoveEvent(const MoveEvent& ev);

        virtual void onProcessResizeEvent(const ResizeEvent& ev);

        virtual void onResizeEvent(const ResizeEvent& ev);

        virtual bool onProcessMouseEvent(const MouseEvent& ev);

        virtual bool onProcessTouchEvent(const TouchEvent& ev);

        virtual bool onProcessScrollEvent(const ScrollEvent& ev);

        virtual void onProcessEnterEvent(const EnterEvent& ev);

        virtual void onProcessLeaveEvent(const LeaveEvent& ev);

        virtual bool onProcessKeyEvent(const KeyEvent& ev);

    // Element - Event handling
    protected:
        virtual bool onMouseEvent(const MouseEvent& ev);

        virtual bool onTouchEvent(const TouchEvent& ev);

        virtual bool onScrollEvent( const ScrollEvent& ev);

        virtual bool onKeyEvent(const KeyEvent& ev);

        virtual bool onEnterEvent( const EnterEvent& ev);

        virtual bool onLeaveEvent(const LeaveEvent& ev);

    // Widget Handler
    protected:
        virtual gfx::Point onToControl(const Control& widget, 
                                       const gfx::Point& pos) const;

        virtual gfx::Point onFromControl(const Control& widget, 
                                         const gfx::Point& pos) const;

        virtual void onAttach(Control& widget);

        virtual void onDetach(Control& widget);

        virtual void onInit(Control& widget);

        virtual void onRelease(Control& widget);

    protected:
        virtual void onRepaintRequest(Control& widget, const gfx::Rect& rect);

        virtual void onRelayoutRequest(Control& widget);

        virtual void onEnableRequest(Control& widget, bool isEnable);

        virtual void onActivateRequest(Control& w, bool active);

        virtual void onShowRequest(Control& widget, bool isShown);

        virtual void onMoveRequest(Control& widget, const gfx::Point& pos);

        virtual void onResizeRequest(Control& widget, const gfx::Size& size);

        virtual void onRaiseRequest(Control& w);

        virtual const std::vector<Key> onGetShortcuts();

        virtual const std::vector<Pt::Char> onGetMnemonics();

        virtual void onSetEnabledState(bool v);

        
        virtual void onSetResourceManager(wildcat::gui::base::ResourceManager& v);


        

    public:
        base::Object* objectAt(Pt::uint32_t index) const;

        void addObject(Pt::uint32_t index, base::Object* w);

    private:
        void setForm(WindowBase* form);

        void setWindow(WindowBase* form)
        {
            setForm(form);
        }
            
        int focusPolicyI() const;

        void setFocusPolicyI(int f);

        int dockModeI() const
        {
            return dockPosition();
        }

        void setDockModeI(int v)
        {
            setDockPosition((DockPosition)v);
        }


    private:
        gfx::RegionGraphics          _graphics;
        View*                        _parent;
        std::vector<Control*>        _children;
        WindowBase*                  _form;
        bool                         _show;
        bool                         _enabled;
        gfx::Size                    _preferredSize;
        bool                         _hasFocus;
        Pt::Signal<Pt::Char>         _mnemonicEntered;
        

    private: //Properties
        Spacing      _padding;
        Spacing      _margin;        
        DockPosition _dockPosition;
        int          _dockPermission;
        Pt::String   _toolTip;
        Pt::String   _trToolTip;
        FocusPolicy  _focusPolicy;
        size_t       _focusIndex;
        Key          _actionKey;
        Key          _shortcutKey;
        Pt::Char     _mnemonic;

    private://Signals    
        wildcat::gui::base::Signal _layoutSignal;    
        wildcat::gui::base::Signal _paddingChanged;
        wildcat::gui::base::Signal _marginChanged;
        wildcat::gui::base::Signal _dockPositionChanged;
        wildcat::gui::base::Signal _dockPermissionChanged;
        wildcat::gui::base::Signal _toolTipChanged;
        wildcat::gui::base::Signal _textChanged;
        wildcat::gui::base::Signal _focusPolicyChanged;
        wildcat::gui::base::Signal _focusIndexChanged;
        wildcat::gui::base::Signal _actionKeyChanged;
        wildcat::gui::base::Signal _shortcutKeyChanged;
        wildcat::gui::base::Signal _mnemonicChanged;
};

}}}

#endif

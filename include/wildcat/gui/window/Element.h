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
#ifndef WILDCAT_GUI_WINDOW_ELEMENT_H
#define WILDCAT_GUI_WINDOW_ELEMENT_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/Spacing.h>
#include <wildcat/gui/window/Cursor.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Rect.h>
#include <wildcat/gui/base/Object.h>
#include <wildcat/gui/base/Signal.h>
#include <wildcat/gui/base/ResourceManager.h>
#include <Pt/String.h>
#include <Pt/Event.h>
#include <Pt/Connectable.h>
#include <Pt/Signal.h>
#include <Pt/Types.h>
#include <string>
#include <cmath>

namespace wildcat {
namespace gui {
namespace window {

enum class HitTestHint
{
    HitAll = 1,
    HitSelf = 2
};

class EventSignal
{ 
    public:
        EventSignal(bool& r, const Pt::Event& ev)
        : _result(r)
        , _event(ev)   
        {

        }

        void setResult(bool r)
        {
            _result = r;
        }

        bool result() const
        {
            return _result;
        }   

        const Pt::Event& event() const
        {
            return _event;  
        }

        template <typename  T>
        const T* eventTo() const
        {
            return dynamic_cast<const T*>(&_event);
        }

    private:
        bool& _result;
        const Pt::Event& _event; 

};

class WILDCAT_GUI_WINDOW_API Element : public base::Object
{
    protected:
        Element();

    public:
        virtual ~Element();
        
     //Properties
    public:

        //Visible    
        bool isVisible() const;

        virtual void show(bool b = true);

        //Enable
        bool isEnabled() const;

        virtual void enable(bool isEnable = true);


        //Position        
        const gfx::Point& position() const;

        void setPosition(const gfx::Point& p);

        virtual void move(const gfx::Point& pos);

        //Size 
        const gfx::Size& size() const;

        void setSize(const gfx::Size& s);        

        virtual void resize(const gfx::Size& s);

        //Activate
        void activate(bool active = true);
        bool isActivate() const;

        //Maximum size
        const gfx::Size& maximumSize() const;
        void setMaximumSize(const gfx::Size& s);        
        void setMaximumSize(int w, int h);


        //Minimum size               
        const gfx::Size& minimumSize() const;               
        void setMinimumSize(const gfx::Size& s);
        void setMinimumSize(int w, int h);

        bool acceptScrollEvent() const
        {
            return onGetAcceptScrollEvent() ;
        }

        void setAcceptScrollEvent(bool v);
        
        const Cursor& cursor() const;

        void setCursor(const std::string& name);    

        uint32_t cursorId() const;

        void setCursorId(uint32_t id);
        
        void setLangId(uint32_t id)
        {
            _langId = id;
            invalidate();
        }

        uint32_t langId() const
        {
            return _langId;
        }

        uint32_t getLangId() const;

        void setColorSchemeId(uint32_t id)
        {
            _colorSchemeId = id;      
            invalidate();   
        }

        uint32_t colorSchemeId() const
        {
            return _colorSchemeId;
        }

        uint32_t getColorSchemeId() const;


        void updateScaling()
        {
            onUpdateScaling();
        }   

    public:
        Element* parent();

        const Element* parent() const;
     
        Element* logicalParent();

        const Element* logicalParent() const;

        void setLogicalParent(Element* parent); 

        bool isDescendantOf(const Element& top) const;

        bool isAncestorOf(const Element& child) const;

        Element* hitTest(const gfx::Point& pos, HitTestHint hint);

        gfx::Point toParent(const gfx::Point& pos) const;

        gfx::Point fromParent(const gfx::Point& pos) const;

        gfx::Point toGlobal(const gfx::Point& pos) const;
        
        gfx::Point fromGlobal(const gfx::Point& pos) const;

    public: //Event Handling    

        bool mouseEvent(const MouseEvent& ev);

        bool touchEvent(const TouchEvent& ev);

        bool scrollEvent(const ScrollEvent& ev);

        bool enterEvent(const EnterEvent& ev);

        bool leaveEvent(const LeaveEvent& ev);

        bool keyEvent(const KeyEvent& ev);

    public:
        void invalidate();
        
    public:
        virtual void repaint(const gfx::Rect& rect);
        
        virtual void repaint();

        void setMinimumWidth(int w);

        void setMaximumWidth(int w);

        void setMinimumHeight(int h);

        void setMaximumHeight(int h);


    public:  

        void setEnabledState(bool v);

        bool isEnabledState() const;    
        

    public:
        const gfx::Rect bounds() const
        {
            return gfx::Rect(gfx::Point(0,0), size());
        }

        void rs(int width, int height)
        {
            resize(gfx::Size(width, height));
        }

        void mv(int x, int y)
        {
            move(gfx::Point(x, y));
        }

    public:
        void setCapture(bool capture);

        bool hasCapture() const;    

    public:
        bool processEvent(const Pt::Event& ev);
        
        Pt::Signal<const Pt::Event&>& eventReceived();
    
        const Pt::String& tr(const Pt::String& key) const;

    public:

        template <typename T>
        T* getRecource(uint32_t id)
        {
            T* res = resourceMng().get<T>(id);

            if (res != nullptr)
                return res;

            return dynamic_cast<T*>(appResource(id));
        }

        template <typename T>
        T* getRecource(const std::string& n)
        {
            return getRecource<T>(base::ResourceManager::id(n));
        }

        template <typename T>
        const T* getRecource(uint32_t id) const
        {
            const T* res = resourceMng().get<T>(id);

            if (res != nullptr)
                return res;

            return dynamic_cast<const T*>(appResource(id));
        }

        template <typename T>
        const T* getRecource(const std::string& n) const
        {
            return getRecource<T>(base::ResourceManager::id(n));
        }
        
    public://Action signals 
        wildcat::gui::base::Signal& invalidateEvent()
        {
            return _invalidateSignal;
        }

        wildcat::gui::base::Signal& mousePressSignal()
        {
            return _mousePressSignal;
        }

        wildcat::gui::base::Signal& mouseReleaseSignal()
        {
            return _mouseReleaseSignal; 
        }

        wildcat::gui::base::Signal& mouseMoveSignal()
        {
            return _mouseMoveSignal;
        }   

        wildcat::gui::base::Signal& paintSignal()
        {
            return _paintSignal;
        }   

        wildcat::gui::base::Signal& invalidateSignal()
        {
            return _invalidateSignal;
        }

        wildcat::gui::base::Signal& showSignal()
        {
            return _showSignal; 
        }

        wildcat::gui::base::Signal& enableSignal()
        {
            return _enableSignal;   
        }

        wildcat::gui::base::Signal& mouseSignal()
        {
            return _mouseSignal;    
        }

        wildcat::gui::base::Signal& touchSignal()
        {
            return _touchSignal;    
        }

        wildcat::gui::base::Signal& scrollSignal()
        {
            return _scrollSignal;   
        }

        wildcat::gui::base::Signal& keySignal()
        {
            return _keySignal;  
        }   

        wildcat::gui::base::Signal& enterSignal()
        {
            return _enterSignal;  
        }   

        wildcat::gui::base::Signal& leaveSignal()
        {
            return _leaveSignal;
        }


    public://Property signals

        wildcat::gui::base::Signal& positionChanged()
        {
            return _positionChanged;    
        }
        

        wildcat::gui::base::Signal& sizeChanged()
        {
            return _sizeChanged;
        }

        wildcat::gui::base::Signal& minimumSizeChanged()
        {
            return _minimumSizeChanged;
        }

        wildcat::gui::base::Signal& maximumSizeChanged()
        {
            return _maximumSizeChanged;
        }

        wildcat::gui::base::Signal& cursorChanged()
        {
            return _cursorChanged;
        }
        

    private:
        const wildcat::gui::base::Resource* appResource(uint32_t id) const;

        wildcat::gui::base::Resource* appResource(uint32_t id);
 


    protected:        
        virtual void onSetParent(Element* visual);

        virtual Element* onHitTest(const gfx::Point& pos, HitTestHint hint);

        virtual gfx::Point onToParent(const gfx::Point& pos) const = 0;

        virtual gfx::Point onFromParent(const gfx::Point& pos) const = 0;

        virtual gfx::Point onToGlobal(const gfx::Point& pos) const;

        virtual gfx::Point onFromGlobal(const gfx::Point& pos) const;

    protected:
        virtual void onRequestRepaint(const gfx::Rect& rect);

        virtual void onRequestShow(bool e);

        virtual void onRequestEnable(bool isEnable);

        virtual void onRequestActivate(bool active);

        virtual void onRequestMove(const gfx::Point& pos);

        virtual void onSetSizeLimits(const gfx::Size& minSize, const gfx::Size& maxSize);

        virtual void onRequestResize(const gfx::Size& s);

        virtual void onRequestCapture(bool capture);

        virtual void onSetEnabledState(bool v);

        virtual void onUpdateScaling();

    protected:
        virtual bool onMousePress(const MouseEvent& ev);

        virtual bool onMouseRelease(const MouseEvent& ev);

        virtual bool onMouseMove(const MouseEvent& ev);

    protected:
        virtual bool onProcessEvent(const Pt::Event& ev);

    protected:
        virtual void onProcessInvalidateEvent(const InvalidateEvent& ev);

        virtual void onInvalidateEvent(const InvalidateEvent& ev);
    
        virtual void onInvalidate();

    protected:
        virtual void onProcessPaintEvent(const PaintEvent& ev);

        virtual void onPaintEvent(const PaintEvent& ev);


    protected:
        virtual void onProcessShowEvent(const ShowEvent& ev);

        virtual void onShowEvent(const ShowEvent& ev);

        virtual void onShow(bool visible);

    protected:
        virtual void onProcessEnableEvent(const EnableEvent& ev);

        virtual void onEnableEvent(const EnableEvent& ev);

        virtual void onEnable(bool e);

    protected:
        virtual void onProcessMoveEvent(const MoveEvent& ev);

        virtual void onMoveEvent(const MoveEvent& ev);

        virtual void onProcessResizeEvent(const ResizeEvent& ev);

        virtual void onResizeEvent(const ResizeEvent& ev);

    protected:
        virtual bool onProcessMouseEvent(const MouseEvent& ev);

        virtual bool onProcessTouchEvent(const TouchEvent& ev);

        virtual bool onProcessScrollEvent(const ScrollEvent& ev);

        virtual void onProcessEnterEvent(const EnterEvent& ev);

        virtual void onProcessLeaveEvent(const LeaveEvent& ev);

        virtual bool onProcessKeyEvent(const KeyEvent& ev);

    protected: // Event handling
        virtual bool onMouseEvent(const MouseEvent& ev);

        virtual bool onTouchEvent(const TouchEvent& ev);

        virtual bool onScrollEvent( const ScrollEvent& ev);

        virtual bool onEnterEvent( const EnterEvent& ev);

        virtual bool onLeaveEvent(const LeaveEvent& ev);

        virtual bool onKeyEvent(const KeyEvent& ev);

    protected:
        virtual bool onGetAcceptScrollEvent() const
        {
            return _acceptScrollEvent;
        }   

        virtual void onSetAcceptScrollEvent(bool v);
        
    private: 
        Pt::Signal<const Pt::Event&> _dispatcher;
        Element*             _logicalParent;
        Element*             _parent;
        int                  _invalidates;
        uint32_t          _cursor;

    private://Properties
        bool                 _enabled;
        bool                 _enabledByParent;
        bool                 _isVisible;
        gfx::Point           _pos;
        gfx::Size            _size;
        gfx::Size            _minimumSize;
        gfx::Size            _maximumSize;
        bool                 _acceptScrollEvent;
        uint32_t     _langId;
        uint32_t     _colorSchemeId;

    private:
        //Action signals
        wildcat::gui::base::Signal _mousePressSignal;
        wildcat::gui::base::Signal _mouseReleaseSignal;
        wildcat::gui::base::Signal _mouseMoveSignal;
        wildcat::gui::base::Signal _paintSignal;
        wildcat::gui::base::Signal _invalidateSignal;
        wildcat::gui::base::Signal _showSignal;
        wildcat::gui::base::Signal _enableSignal;
        wildcat::gui::base::Signal _mouseSignal;
        wildcat::gui::base::Signal _touchSignal;
        wildcat::gui::base::Signal _scrollSignal;    
        wildcat::gui::base::Signal _keySignal;
        wildcat::gui::base::Signal _enterSignal;
        wildcat::gui::base::Signal _leaveSignal;         
    private:
        //Property signals
        wildcat::gui::base::Signal _positionChanged; 
        wildcat::gui::base::Signal _sizeChanged;
        wildcat::gui::base::Signal _minimumSizeChanged;
        wildcat::gui::base::Signal _maximumSizeChanged;;
        wildcat::gui::base::Signal _acceptScrollEventChanged;
        wildcat::gui::base::Signal _cursorChanged;

};

}}}

#endif
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
#ifndef WILDCAT_GUI_WINDOW_SCREEN_H
#define WILDCAT_GUI_WINDOW_SCREEN_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/Element.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Point.h>
#include <Pt/System/Clock.h>
#include <Pt/Signal.h>

#include <vector>

namespace wildcat {
namespace gui {
namespace window {

class ScreenImpl;
class ApplicationImpl;
class Window;
class WindowManagerBase;
class RescaleEvent;

class WILDCAT_GUI_WINDOW_API Screen : public Element
{
    friend class ScreenImpl;

    typedef Element Base;

    public:
        Screen(ApplicationImpl& app);

        virtual ~Screen();

        
        void addWindow(Window& w);

        void removeWindow(Window& w);

        const std::vector<Window*>& windows() const;

        WindowManagerBase& windowManager();
                      
        void setScaleFactor(double v);

        double scaleFactor() const; 

    public:
        ScreenImpl* impl();
    
    protected:
        virtual void onInit(ScreenImpl& s);

        virtual void onRelease(ScreenImpl& s);

        virtual void onResize(ScreenImpl& s, const gfx::Size& size);

        virtual void onShow(ScreenImpl& s, bool isShow);


    //
    // Element
    //
    protected:
        virtual Element* onHitTest(const gfx::Point& pos, HitTestHint hint);

        virtual gfx::Point onToParent(const gfx::Point& pos) const;

        virtual gfx::Point onFromParent(const gfx::Point& pos) const;

        virtual gfx::Point onToGlobal(const gfx::Point& pos) const;

        virtual gfx::Point onFromGlobal(const gfx::Point& pos) const;

        virtual bool onProcessEvent( const Pt::Event& ev );

        virtual void onRequestRepaint(const gfx::Rect& rect);

        virtual void onUpdateScaling();

    protected:
        virtual void onProcessPaintEvent(const PaintEvent& ev);

        virtual void onPaintEvent(const PaintEvent& ev);

        virtual void onPaint(const gfx::Rect& rect);
        
    protected:
        virtual void onProcessResizeEvent(const ResizeEvent& ev);

        virtual void onResizeEvent(const ResizeEvent& ev);

    protected:
        virtual bool onProcessMouseEvent(const MouseEvent& ev);

        virtual bool onProcessTouchEvent(const TouchEvent& ev);

        virtual bool onProcessScrollEvent(const ScrollEvent& ev);

        virtual bool onProcessKeyEvent(const KeyEvent& ev);

    //
    // Responder
    //
    protected:
        virtual bool onMouseEvent(const MouseEvent& ev);

        virtual bool onTouchEvent(const TouchEvent& ev);

        virtual bool onScrollEvent(const ScrollEvent& ev);

        virtual bool onKeyEvent(const KeyEvent& ev);

    private:
        ScreenImpl*         _impl;
        gfx::Rect          _updateRect;
        int                 _updates;
        Pt::System::Clock   _clock;
};

}}}

#endif

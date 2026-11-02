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
#ifndef WILDCAT_GUI_WINDOW_APPLICATIONIMPL_H
#define WILDCAT_GUI_WINDOW_APPLICATIONIMPL_H

#include "Selector.h"

#include <wildcat/gui/window/MouseEvent.h>
#include <wildcat/gui/window/KeyEvent.h>
#include <wildcat/gui/window/Cursor.h>
#include <wildcat/gui/gfx/Font.h>
#include <Pt/System/EventLoop.h>
#include <Pt/Timespan.h>
#include <vector>
#include <windows.h>

namespace wildcat {
namespace gui {
namespace window {

class Window;
class Control;
class Element;

class Selector : public Pt::System::Selector
{
  public:
      Selector();
        
      ~Selector();

  protected:
      virtual DWORD waitFor(DWORD numHandles, const HANDLE *handles, DWORD msecs, bool& isTimeout);
};

class ApplicationImpl : public Pt::System::EventLoop
{
    public:
        ApplicationImpl();

        virtual ~ApplicationImpl();

        Window* findWindow(HWND h);

        void setCursor(const Cursor* cursor);

        void setFontDir(const Pt::System::Path& dir);

        void setDefaultFont(const gfx::Font& fontName);

        Pt::Timespan inactivityTime() const;

        void sendKeyEvent(const KeyEvent& ev);

        void sendMouseEvent(const MouseEvent& ev);

        void nextEvent();

        bool isSystemDarkMode() const;

    public:
        virtual Pt::System::Selector& selector()
        { 
            return _selector; 
        }

    protected:
        virtual void onAttachSelectable(Pt::System::Selectable&);

        virtual void onDetachSelectable(Pt::System::Selectable&);

        virtual void onCancel(Pt::System::Selectable& s);

        virtual void onReady(Pt::System::Selectable& s);

        virtual void onRun();

        virtual void onExit();

        virtual void onCommitEvent(const Pt::Event& event);

        virtual void onQueueEvent(const Pt::Event& event);

        virtual void onWake();

        virtual void onProcessEvents();

        virtual void onAttachTimer(Pt::System::Timer& timer);

        virtual void onDetachTimer(Pt::System::Timer& timer);

    private:
        bool waitNext();

        bool processMessage(HWND hwnd, UINT msg, 
                            WPARAM wparam, LPARAM lparam);
  
        static long CALLBACK wndProc(HWND hwnd, UINT message, 
                                     WPARAM wparam, LPARAM lparam);

        //! @brief Window procedure for windows which handle WM_CLOSE separately.
        static long CALLBACK closeWndProc(HWND hwnd, UINT message, 
                                          WPARAM wparam, LPARAM lparam);

        void onPaint(Window& w, HWND hwnd);
        
        void onResize(Window& w, WPARAM wparam, LPARAM lparam);

        void onMouse(Window& w, unsigned int msg,  WPARAM wparam, LPARAM lparam);
        
        void onKey(Window& w, UINT vkey, UINT scanCode, bool isPress);
        
        void onMove(Window& w, HWND hwnd, LPARAM lparam);
 
        bool onClose(Window& w);
        
        void onActivate(Window& w, bool f);
        
        void onEnable(Window& w, bool e);
        
        void onShow(Window& w, bool s);

    private:
        Pt::System::Mutex                    _mutex;
        Pt::System::TimerQueue               _timerQueue;
        Pt::System::EventQueue               _eventQueue;
        gui::window::Selector                _selector;
        std::vector<Pt::System::Selectable*> _avail;
        HINSTANCE                        _instanceHandle;
        ULONG_PTR                        _gdiplusToken;
        MouseEvent                       _mouseEvent;
        KeyEvent                         _keyEvent;
        HWND                             _pointerWindow;
        HCURSOR                          _defaultCursorHandle;
        HCURSOR                          _cursorHandle;
};

}}}

#endif

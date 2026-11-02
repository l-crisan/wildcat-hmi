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
#ifndef WILDCAT_GUI_WINDOW_FORMS_APPLICATIONIMPL_H 
#define WILDCAT_GUI_WINDOW_FORMS_APPLICATIONIMPL_H

#include "Selector.h"
#include <wildcat/gui/window/MouseEvent.h>
#include <wildcat/gui/window/KeyEvent.h>
#include <wildcat/gui/window/Cursor.h>
#include <wildcat/gui/gfx/Font.h>
#include <wildcat/gui/window/ScrollEvent.h>
#include <Pt/System/EventLoop.h>
#include <Pt/System/Condition.h>
#include <Pt/DateTime.h>
#include <Pt/Timespan.h>
#include <emscripten/html5.h>
#include <thread>

namespace wildcat {
namespace gui {
namespace window {


class Selector : public Pt::System::Selector
{
public:
        Selector()
        {
        }

        virtual ~Selector()
        {
        }

        virtual void cancel(Pt::System::IOHandle& h)
        {
        }

        // used by IONotifier
        virtual void beginWait(Pt::System::IOHandle* h, int flags)
        {
        }

        // used by IONotifier
        virtual int endWait(Pt::System::IOHandle* h)
        {
            return 0;
        }

        virtual void beginRead(Pt::System::IOHandle* h)
        {
        }

        virtual void endRead(Pt::System::IOHandle* h)
        {
        }

        virtual void beginWrite(Pt::System::IOHandle* h)
        {
        }

        virtual void endWrite(Pt::System::IOHandle* h)
        {
        }

        virtual bool isReadable(Pt::System::IOHandle* h)
        {
            return false;
        }

        virtual bool isWritable(Pt::System::IOHandle* h)
        {
            return false;
        }

        virtual bool isError(Pt::System::IOHandle* h)
        {
            return false;
        }

        // used by IONotifier
        virtual bool isReady(Pt::System::IOHandle* h)
        {
            return false;
        }
};

class ApplicationImpl : public Pt::System::EventLoop
{
    public:
        ApplicationImpl();

        virtual ~ApplicationImpl();

        const Cursor& cursor()
        {
            return _cursor;
        }

        void setCursor(const Cursor* cursor);

        void setFontDir(const Pt::System::Path& dir);

        void setDefaultFont(const gfx::Font& font);

        Pt::Timespan inactivityTime() const;

        void sendKeyEvent(const KeyEvent& ev);

        void sendMouseEvent(const MouseEvent& ev);

        void nextEvent();


        virtual Pt::System::Selector& selector()
        {
            return _selector;
        }

        bool isSystemDarkMode() const;
        
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
        // single wait/dispatch step, shared by onRun() and nextEvent()
        bool waitNext();

        // entry point of the real OS thread that runs the blocking event loop
        void run();

        void dispatchWheelEvent(const EmscriptenWheelEvent& e, int eventType);

        void dispatchKeyEvent(const EmscriptenKeyboardEvent& e, bool press);

        void dispatchMouseEvent(const EmscriptenMouseEvent& e, int eventType);


    private:
        static EM_BOOL onKeyDown(int eventType, const EmscriptenKeyboardEvent* e, void* userData);

        static EM_BOOL onKeyUp(int eventType, const EmscriptenKeyboardEvent* e, void* userData);

        static EM_BOOL onMouseEvent(int eventType, const EmscriptenMouseEvent* e, void* userData);

        static EM_BOOL onWheelEvent(int eventType, const EmscriptenWheelEvent* e, void* userData);


    private:
        Selector               _selector;
        Pt::System::EventQueue     _eventQueue;
        Pt::System::Mutex          _wakeMutex;
        Pt::System::Condition      _wakeCondition;
        Pt::System::TimerQueue     _timerQueue;
        bool                   _exiting;
        Pt::DateTime           _lastActivityTime;
        MouseEvent             _mev;
        KeyEvent               _keyEvent;
        Cursor                 _cursor;
        bool _wakePending;
        ScrollEvent _sev;
};

}}} 

#endif

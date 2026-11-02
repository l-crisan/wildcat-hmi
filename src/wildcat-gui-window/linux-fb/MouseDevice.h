/*
  Copyright (C) 2015 Marc Boris Duerner 
  Copyright (C) 2015 Laurentiu-Gheorghe Crisan

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
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
  MA 02110-1301 USA
*/

#ifndef WILDCAT_GUI_WINDOW_MOUSEDEVICE_H
#define WILDCAT_GUI_WINDOW_MOUSEDEVICE_H

#include "Selector.h"
#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/KeyEvent.h>
#include <wildcat/gui/window/MouseEvent.h>
#include <wildcat/gui/window/TouchEvent.h>
#include <wildcat/gui/gfx/Size.h>
#include <Pt/System/Selectable.h>
#include <Pt/Signal.h>
#include <unistd.h>

struct input_event;

namespace wildcat {
namespace gui {
namespace window {

class MouseDevice : public Pt::System::Selectable
{
    public:
        MouseDevice(const char* deviceName);
        
        MouseDevice();

        ~MouseDevice();

        void open(const char* deviceName);

        void close();

        void begin();

        void setScreenLimit(const gfx::Size& size)
        {
            _screenSize = size;
        }

        Pt::Signal<const Pt::Event&>& eventReady()
        {
            return _eventReady;
        }

    protected:
        void onAttach(Pt::System::EventLoop& loop);

        void onDetach(Pt::System::EventLoop& loop);  

        virtual bool onRun();

        virtual void onCancel();

    protected:
        void onKey(const input_event& ev);

        void onRelative(const input_event& ev);

        void onAbsolute(const input_event& ev);

    private:
        Pt::System::IOHandle _ioh;
        Pt::System::EventLoop* _loop;
        gfx::Size _screenSize;
        bool _shift;
        bool _control;
        bool _alt;
        bool _meta;
        Key::Modifiers _modifiers;
        KeyEvent _keyEvent;
        MouseEvent _mouseEvent;        
        int _touchMove;
        TouchEvent _touchEvent;
        Pt::Signal<const Pt::Event&> _eventReady;
        double _mouseResWidth;
        double _mouseResHeigh;
};

}}}

#endif

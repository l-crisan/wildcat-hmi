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

#include "InputDevice.h"
#include "MouseDevice.h"
#include "FrameBuffer.h"

#include <wildcat/gui/window/Element.h>
#include <wildcat/gfx/Transform.h>
#include <Pt/System/MainLoop.h>
#include <Pt/DateTime.h>
#include <Pt/Timespan.h>

namespace wildcat {
namespace gui {
namespace window {

class ApplicationImpl : public Pt::System::MainLoop
{
    public:
        ApplicationImpl();

        virtual ~ApplicationImpl();

        const Cursor& cursor()
        { 
            return _cursor; 
        }

        void setCursor(const Cursor* cursor );

        void setFontDir(const Pt::System::Path& dir);

        void setDefaultFont(const gfx::Font& font);

        Pt::Timespan inactivityTime() const;

        void sendKeyEvent(const KeyEvent& ev);

        void sendMouseEvent(const MouseEvent& ev);
    
        void nextEvent();

    public:
        FrameBuffer& frameBuffer()
        {
            return _frameBuffer;
        }

    private:
        void onMouseEvent(const MouseEvent& ev);

        void onScrollEvent(const ScrollEvent& ev);

        void onTouchEvent(const TouchEvent& ev);

        void onKeyEvent(const KeyEvent& ev);

        void showConsole(bool s);

        void openInputDevice(const std::string& device);

    private:
        FrameBuffer                  _frameBuffer; 
        std::vector<InputDevice*>    _inputDevices;
        MouseDevice*                 _mouseDevice;
        gfx::Transform               _touchTransform;      
        Cursor                       _cursor;
        Pt::DateTime                 _lastActivityTime;
};

}}}

#endif


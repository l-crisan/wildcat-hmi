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
#ifndef WILDCAT_GUI_WINDOW_TOOLTIP_H
#define WILDCAT_GUI_WINDOW_TOOLTIP_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/Control.h>
#include <Pt/System/Timer.h>

namespace wildcat {
namespace gui {
namespace window {

class WILDCAT_GUI_WINDOW_API ToolTip : public Pt::Connectable
{
    public:
        ToolTip();

        virtual ~ToolTip();

        int autoPopDelay() const
        {
            return _autoPopDelay;
        }   

        void setAutoPopDelay(int ms)
        {
            _autoPopDelay = ms;
        }   

        int initialDelay() const
        {
            return _initialDelay;
        }   

        void setInitialDelay(int ms)
        {
            _initialDelay = ms;
        }       

        int reshowDelay() const
        {
            return _reshowDelay;
        }   

        void setReshowDelay(int ms)
        {
            _reshowDelay = ms;
        }   

        void start(Control& ctrl);
        

        void stop(Control& ctrl);


        void setEnabled(Pt::System::EventLoop* loop)
        {
            if (loop == nullptr)    
                _timer.detach();    
            else
                _timer.setActive(*loop);    
        }   

        void setMouse(const gfx::Point& pos)
        {
            _mousePos = pos + 10;
        }   

    private:

        void onTimeout();  
        
        void showToolTip(const gfx::Point& pos);

        void closeToolTip();
    private:

        enum State
        {
            Idle,
            PopDelay,
            Showing
        };  

   private:
       int _autoPopDelay;   
       int _initialDelay;   
       int _reshowDelay;    
       State _state;    
       Pt::System::Timer _timer;
       Control* _ctrl;
       gfx::Point _mousePos;
};

}}}

#endif

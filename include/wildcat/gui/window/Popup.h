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
#ifndef WILDCAT_GUI_WINDOW_POPUP_H
#define WILDCAT_GUI_WINDOW_POPUP_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/Window.h>

namespace wildcat {
namespace gui {
namespace window {

class WILDCAT_GUI_WINDOW_API Popup : public Window
{
    typedef Window Base;

    public:
        Popup();
		
        virtual ~Popup();

        bool isCaptureEnabled() const
        {
            return _enableCapture;
        }   


        void setCaptureEnabled(bool v)
        {
            _enableCapture = v;
        }   


    protected:        
        virtual bool onMouseEvent(const MouseEvent& ev);

        virtual bool onTouchEvent(const TouchEvent& ev);

        virtual void onShow(bool isShown);  

        virtual void onCloseEvent(const CloseEvent& ev); 

        virtual void onShowEvent(const ShowEvent& ev);  

    private:        
        bool _enableCapture;

    private:
        static base::TypeInit<Popup> _typeInit;
};

}}}

#endif

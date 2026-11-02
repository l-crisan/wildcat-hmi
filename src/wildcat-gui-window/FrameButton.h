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
#ifndef WILDCAT_GUI_WINDOW_FRAMEBUTTON_H
#define WILDCAT_GUI_WINDOW_FRAMEBUTTON_H

#include <wildcat/gui/window/MouseEvent.h>
#include <wildcat/gui/window/ResizeEvent.h>
#include <wildcat/gui/window/MoveEvent.h>
#include <wildcat/gui/window/EnterEvent.h>
#include <wildcat/gui/window/LeaveEvent.h>
#include <wildcat/gui/window/TouchEvent.h>
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/gfx/Graphics.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Rect.h>
#include <wildcat/gui/base/ResourceManager.h>
#include <Pt/Signal.h>
#include <vector>

namespace wildcat {
namespace gui {
namespace window {

class WindowFrame;

class FrameButton
{
    public:
        FrameButton();

        virtual ~FrameButton();

        WindowFrame* parent()
        { 
            return _frame; 
        }

        void setParent(WindowFrame& frame)
        { 
            _frame  = &frame;
        }

        const gfx::Rect&  geometry() const
        { 
            return _geometry; 
        }


        Pt::Signal<>& clicked()
        { 
            return _clicked; 
        }

        bool isVisible() const
        {
            return _visible;
        }

        void setVisible(bool v)
        {
            _visible = v;
        }

        bool isHighlight() const
        {
            return _isHighlight;
        }


        void setHightlight(bool v)
        {
            _isHighlight = v;
        }

        void update();

        virtual void moveEvent(const MoveEvent& mev);

        virtual void resizeEvent(const ResizeEvent& rev);

        virtual void enterEvent(const EnterEvent& eev);

        virtual void leaveEvent(const LeaveEvent& lev);

        virtual void mouseEvent(const MouseEvent& mev);

        virtual void touchEvent(const TouchEvent& tev);

        virtual void paint(gfx::Graphics& gf);

        virtual void invalidate()
        {         
        }

    private:
        Pt::Signal<>  _clicked;
        WindowFrame*  _frame;
        gfx::Rect     _geometry;
        bool          _isPressed;
        bool          _visible;
        bool          _isHighlight;
};


class MinimizeButton : public FrameButton
{
public:
    MinimizeButton();

    virtual ~MinimizeButton();

    void paint(gfx::Graphics& gf);
};


class MaximizeButton : public FrameButton
{
public:
    MaximizeButton();

    virtual ~MaximizeButton();

    void paint(gfx::Graphics& gf);
};


class CloseButton : public FrameButton
{
public:
    CloseButton();

    virtual ~CloseButton();

    void paint(gfx::Graphics& gf);
};


class MenuButton : public FrameButton
{
    public:
        MenuButton();

        virtual ~MenuButton();

        void paint(gfx::Graphics& gf);

        void setIcon(uint32_t& i)
        {
            _icon = i;
        }

        virtual void invalidate()
        {
            _iconPixmap.reset();    

            const gfx::Icon* ic = _rm->get<wildcat::gui::gfx::Icon>(_icon);

            if (ic == nullptr)
            {
                Application& app = Application::instance();
                ic = app.resourceMng().get< wildcat::gui::gfx::Icon>(_icon);

                if (ic == nullptr)
                    return;
            }

            _iconPixmap = ic->getImage(Application::inst().screen().scaleFactor());
        }

        const gfx::Pixmap& icon() const
        {
            return _iconPixmap;
        }

        void setRm(wildcat::gui::base::ResourceManager* rm)
        {
            _rm = rm;   
        }

    private:
        uint32_t _icon;        
        gfx::Pixmap  _iconPixmap;
        wildcat::gui::base::ResourceManager* _rm;

};


}}}

#endif
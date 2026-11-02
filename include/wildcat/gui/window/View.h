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
#ifndef WILDCAT_GUI_WINDOW_VIEW_H
#define WILDCAT_GUI_WINDOW_VIEW_H

#include <wildcat/gui/window/Element.h>
#include <wildcat/gui/gfx/Point.h>

class Widget;

namespace wildcat{
namespace gui {
namespace window {

typedef Element ViewBase;

class WILDCAT_GUI_WINDOW_API View : public ViewBase
{
    friend class Control;

    public:
        enum FocusPolicy
        {
            NoFocus,
            AcceptFocus,
            KeepFocus
        };

    protected:
        View();

    public:
        virtual ~View();

        gfx::Point toControl(const Control& widget, const gfx::Point& pos) const;

        gfx::Point fromControl(const Control& widget, const gfx::Point& pos) const;
        
        bool designMode() const
        {
            return onGetDesignMode();
        }   

        void setDesignMode(bool mode)
        {
            onSetDesignMode(mode);
        }

        const Pt::String& title()  const
        {
            return _title;
        }

        const Pt::String& trTitle()  const
        {
            return _trTitle;
        }

        void setTitle(const Pt::String& text)
        {
            _title =  text;
            Pt::Any a(_title);
            _titleChanged.send(*this, a);   
            invalidate();
        }

        base::Signal& titleChanged()
        {
            return _titleChanged;   
        }

        const gfx::Pixmap& iconImage() const;

        uint32_t iconId() const
        {
            return _iconId;
        }

        void setIcon(const std::string& name);

        void setIconId(uint32_t id)
        {
            onSetIcon(id);
        }

        wildcat::gui::base::Signal& iconChanged()
        {
            return _iconChanged;
        }

    protected:
        virtual void onAttach(Control& widget) = 0;

        virtual void onDetach(Control& widget) = 0;

        virtual void onInit(Control& widget) = 0;

        virtual void onRelease(Control& widget) = 0;

        virtual gfx::Point onToControl(const Control& widget, const gfx::Point& pos) const = 0;

        virtual gfx::Point onFromControl(const Control& widget, const gfx::Point& pos) const = 0;
        
        virtual void onSetIcon(uint32_t id);

    protected:
        virtual void onRepaintRequest(Control& widget, const gfx::Rect& rect) = 0;

        virtual void onRelayoutRequest(Control& widget) = 0;

        virtual void onEnableRequest(Control& widget, bool isEnable) = 0;

        virtual void onActivateRequest(Control& w, bool active) = 0;

        virtual void onShowRequest(Control& widget, bool isShown) = 0;

        virtual void onMoveRequest(Control& widget, const gfx::Point& pos) = 0;

        virtual void onResizeRequest(Control& widget, const gfx::Size& size) = 0;
      
        virtual void onRaiseRequest(Control& widget) = 0;

        virtual void onInvalidate();

        virtual bool onGetDesignMode() const;

        virtual void onSetDesignMode(bool mode);

    private:
        bool            _designMode;
        Pt::String      _title;
        Pt::String      _trTitle;
        base::Signal    _titleChanged;
        uint32_t        _iconId;
        gfx::Pixmap     _iconPixmap;
        wildcat::gui::base::Signal _iconChanged;  
};

}}}

#endif

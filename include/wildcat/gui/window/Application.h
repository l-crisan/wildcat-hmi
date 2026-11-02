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
#ifndef WILDCAT_GUI_WINDOW_APPLICATION_H
#define WILDCAT_GUI_WINDOW_APPLICATION_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/Screen.h>
#include <wildcat/gui/window/InputMethod.h>
#include <wildcat/gui/window/SystemColors.h>
#include <wildcat/gui/window/ToolTip.h>
#include <wildcat/gui/gfx/Icon.h>
#include <wildcat/gui/gfx/Font.h>
#include <wildcat/gui/gfx/ColorManager.h>
#include <wildcat/gui/gfx/PngReader.h>
#include <wildcat/gui/base/ResourceManager.h>
#include <wildcat/gui/base/Signal.h>
#include <wildcat/gui/base/TextTable.h>
#include <Pt/System/Application.h>
#include <Pt/System/Path.h>
#include <list>

namespace wildcat {
namespace gui {
namespace window {

class Cursor;
class Popup;
class ApplicationImpl;
class WindowBase;

class WILDCAT_GUI_WINDOW_API Application : public Pt::System::Application
{
    friend class Element;
    friend class Popup;
    friend class Screen;

    public:

        enum Colors
        {
            BrightColors,
            DarkColors,
            SystemColors,
            UserColors
        };

    public:
        Application(int argc = 0, char** argv = 0);

        virtual ~Application();

        ApplicationImpl* impl();

        static Application& instance();

        const Screen& screen() const;

        Screen& screen();

        Pt::Timespan inactivityTime() const;

        void setCursor(const Cursor* cursor = 0);


        void setFontDir(const Pt::System::Path& dir);

        void loadImage(const Pt::System::Path& path, gfx::Pixmap& image);


        InputMethod& inputMethod();

        void setInputMethod(InputMethod& im);

        void removeInputMethod(InputMethod& im);

        Element* findVisual(uint64_t id);

        // TODO: this might be the same as loop().waitNext()
        void nextEvent();

        void commitEvent(const Pt::Event& ev);

        void processEvent(const Pt::Event& ev);

        Pt::Signal<const Pt::Event&>& eventReceived();

        void invalidate();

        /** @brief Emulates a key event.
        */
        void sendKeyEvent(const KeyEvent& ev);

        /** @brief Emulates a mouse event.
        */
        void sendMouseEvent(const MouseEvent& ev);

        Element* capture() const;

        Element* pointed() const
        {
            return _pointerOverWidget;
        }

        void setPointed(Element* v);
        

        WindowBase* currentWindow()
        {
            return _currentWindow;
        }

        const WindowBase* currentWindow() const
        {
            return _currentWindow;
        }

        void setCurrentWindow(WindowBase* w)
        {
            _currentWindow = w;
        }

        ToolTip& toolTip()
        {
            return _toolTip;
        }

        const ToolTip& toolTip() const
        {
            return _toolTip;
        }

        uint32_t iconId() const
        {
            return _iconId;
        }

        void setIconId(uint32_t id)
        {
            _iconId = id;
        }   

        const gfx::Pixmap* icon() const
        {
            return resourceMng().get<gfx::Pixmap>(_iconId);
        }

        Colors colors() const
        {
            return _userColor;
        }

        void setColors(Colors v, gfx::ColorPalette*  c = nullptr);

        base::Signal& systemColorsChanged()
        {
            return _systemColorsChanged;
        }

        const std::string& languageCode() const
        {
            return _languageCode;
        }   

        void setLanguageCode(const std::string& code)
        {
            _languageCode = code;
            invalidate();
        }   


        const Pt::String& tr(const Pt::String& key) const;

    public:
        base::ResourceManager& resourceMng()
        {
            return _resMng;
        }

        const base::ResourceManager& resourceMng() const
        {
            return _resMng;
        }

        gfx::ColorPalette& systemColors();
        

        const gfx::ColorPalette& systemColors() const;

        static Application& inst()
        {
            return *((Application*)&Application::instance());
        }

        void setFont(const gfx::Font& font);

        const gfx::Font& font() const
        {
            return _font;
        }

        Pt::Signal<>& textCursorSignal()
        {
            return _textCursorSignal;
        }

        bool textCursorState() const
        {
            return _textCursorState;
        }

        int doubleClickSpeed() const
        {
            return _doubleClickSpeed;
        }

        void setDoubleClickSpeed(int ms)
        {
            _doubleClickSpeed = ms;
        }

        int doubleClickSpeedSlow() const
        {
            return _doubleClickSpeedSlow;
        }

        void setDoubleClickSpeedSlow(int ms)
        {
            _doubleClickSpeedSlow = ms;
        }

        bool isSystemDarkMode() const;
        

    protected:
        virtual void onTextCursor();


    protected:

        void onRequestCapture(Element& target, bool capture);

    private:
        void registerElement(Element& visual);

        void unregisterElement(Element& visual);

    private:
        void onDispatchMouseEvent(const MouseEvent& ev);

        void onProcessMouseEvent(const MouseEvent& ev);


        void onDispatchTouchEvent(const TouchEvent& ev);

        void onProcessTouchEvent(const TouchEvent& ev);


        void onDetectScroll(Element* visual, const gfx::Point& screenPos,
                            bool isPress, bool isPressed);


        void onDispatchScrollEvent(const ScrollEvent& ev);

        void onProcessScrollEvent(const ScrollEvent& ev);


        void onDispatchEnterEvent(const EnterEvent& ev);

        void onProcessEnterEvent(const EnterEvent& ev);


        void onDispatchLeaveEvent(const LeaveEvent& ev);

        void onProcessLeaveEvent(const LeaveEvent& ev);


        void onDispatchKeyEvent(const KeyEvent& ev);

        void onProcessKeyEvent(const KeyEvent& ev);


        void onDispatchInvalidateEvent(const InvalidateEvent& ev);

        void onProcessInvalidateEvent(const InvalidateEvent& ev);


        void onDispatchLayoutEvent(const LayoutEvent& ev);
        
        void onProcessLayoutEvent(const LayoutEvent& ev);


        void onDispatchPaintEvent(const PaintEvent& ev);

        void onProcessPaintEvent(const PaintEvent& ev);


        void onDispatchMoveEvent(const MoveEvent& ev);

        void onProcessMoveEvent(const MoveEvent& ev);


        void onDispatchResizeEvent(const ResizeEvent& ev);

        void onProcessResizeEvent(const ResizeEvent& ev);


        void onDispatchActivateEvent(const ActivateEvent& ev);

        void onProcessActivateEvent(const ActivateEvent& ev);


        void onDispatchEnableEvent(const EnableEvent& ev);

        void onProcessEnableEvent(const EnableEvent& ev);


        void onDispatchShowEvent(const ShowEvent& ev);

        void onProcessShowEvent(const ShowEvent& ev);


        void onDispatchCloseEvent(const CloseEvent& ev);

        void onProcessCloseEvent(const CloseEvent& ev);


        void onDispatchFocusEvent(const FocusEvent& ev);

        void onProcessFocusEvent(const FocusEvent& ev);


        void onDispatchWindowStateEvent(const WindowStateEvent& ev);

        void onProcessWindowStateEvent(const WindowStateEvent& ev);

    private:
        void initSystemColors();

        void loadCursors();

        void loadIcons();

        void invertIcons();

    private:
        typedef std::map<Pt::uint64_t, Element*> ElementMap;

        ApplicationImpl*             _impl;
        Pt::Signal<const Pt::Event&> _eventReceived;
        Screen*                      _mainScreen;
        ElementMap                    _visuals;
        
        DefaultInputMethod*          _defaultInputMethod;
        InputMethod*                 _inputMethod;
                                            
        gfx::Point                  _scrollFrom;
        bool                         _onScroll;
        gfx::PngReader               _iconReader;
        double                       _scaleFactor;
        gfx::Font _font;
        Pt::System::Timer  _textCursorTimer;
        bool _textCursorState;
        Pt::Signal<> _textCursorSignal;
        int _doubleClickSpeed;
        int _doubleClickSpeedSlow;
        Cursor  _currentCursor;
        Element* _pointerOverWidget;
        WindowBase* _currentWindow;
        ToolTip _toolTip;
        std::list<Element*>           _capture;
        uint32_t _iconId;
        base::Signal   _systemColorsChanged;
        std::string _languageCode;
        gfx::ColorPalette _brightSystemColors;
        gfx::ColorPalette _darkSystemColors;
        Colors  _colors;                 
        Colors  _userColor;
        base::ResourceManager _resMng;
};

}}}

#endif

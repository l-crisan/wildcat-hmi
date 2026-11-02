/*
   Copyright (C) 2026 Marc Boris Duerner
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
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/Window.h>
#include <wildcat/gui/window/Popup.h>
#include <wildcat/gui/window/Element.h>
#include <wildcat/gui/window/Widget.h>
#include <wildcat/gui/window/MouseEvent.h>
#include <wildcat/gui/window/TouchEvent.h>
#include <wildcat/gui/window/ScrollEvent.h>
#include <wildcat/gui/window/EnterEvent.h>
#include <wildcat/gui/window/LeaveEvent.h>
#include <wildcat/gui/window/KeyEvent.h>
#include <wildcat/gui/window/InvalidateEvent.h>
#include <wildcat/gui/window/LayoutEvent.h>
#include <wildcat/gui/window/PaintEvent.h>
#include <wildcat/gui/window/ResizeEvent.h>
#include <wildcat/gui/window/MoveEvent.h>
#include <wildcat/gui/window/ActivateEvent.h>
#include <wildcat/gui/window/EnableEvent.h>
#include <wildcat/gui/window/ShowEvent.h>
#include <wildcat/gui/window/CloseEvent.h>
#include <wildcat/gui/window/FocusEvent.h>
#include <wildcat/gui/window/WindowStateEvent.h>
#include <wildcat/gui/base/String.h>
#include <wildcat/gui/base/Hash.h>
#include <Pt/System/FileInfo.h>
#include "ApplicationImpl.h"
#include "ScreenImpl.h"
#include <cmath>
#include <cassert>
#include <fstream>
#include <algorithm>

//Resources
#include <wildcat/gui/resource/icon/32-size_hor.xpm>
#include <wildcat/gui/resource/icon/32-size_ver.xpm>
#include <wildcat/gui/resource/icon/32-text.xpm>
#include <wildcat/gui/resource/icon/32-wait01.xpm>
#include <wildcat/gui/resource/icon/32-default.xpm>
#include <wildcat/gui/resource/icon/32-cursor_crosshairs.xpm>
#include <wildcat/gui/resource/icon/32-size_NESW.xpm>
#include <wildcat/gui/resource/icon/32-size_NWSE.xpm>
#include <wildcat/gui/resource/icon/Undo.xpm>
#include <wildcat/gui/resource/icon/Redo.xpm>
#include <wildcat/gui/resource/icon/Cut.xpm>
#include <wildcat/gui/resource/icon/Copy.xpm>
#include <wildcat/gui/resource/icon/Paste.xpm>
#include <wildcat/gui/resource/icon/Delete.xpm>

namespace wildcat {
namespace gui {
namespace window {

Application::Application(int argc, char** argv)
: Pt::System::Application(0, argc, argv)
, _impl( new ApplicationImpl() ) 
, _mainScreen(0)
, _defaultInputMethod(0)
, _inputMethod(0)
, _onScroll(false)
, _scaleFactor(1)
, _font("Open Sans", 12, "Regular")
, _textCursorTimer()
, _textCursorState(true)
, _textCursorSignal()
, _doubleClickSpeed(500)
, _doubleClickSpeedSlow(1000)
, _pointerOverWidget(0)
, _currentWindow(nullptr)
, _iconId(0)
, _languageCode("default")
, _colors(Colors::BrightColors)
, _userColor(Colors::BrightColors)
{
    this->init(*_impl);

    initSystemColors();

    // default style with default options    
    _mainScreen = new Screen(*_impl);

    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchMouseEvent);
    _eventReceived += Pt::slot(*this, &Application::onProcessMouseEvent);
    
    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchTouchEvent);
    _eventReceived += Pt::slot(*this, &Application::onProcessTouchEvent);
    
    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchScrollEvent);
    _eventReceived += Pt::slot(*this, &Application::onProcessScrollEvent);
    
    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchEnterEvent);
    _eventReceived += Pt::slot(*this, &Application::onProcessEnterEvent);
    
    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchLeaveEvent);
    _eventReceived += Pt::slot(*this, &Application::onProcessLeaveEvent);
    
    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchKeyEvent);
    _eventReceived += Pt::slot(*this, &Application::onProcessKeyEvent);
    
    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchInvalidateEvent );
    _eventReceived += Pt::slot(*this, &Application::onProcessInvalidateEvent);

    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchLayoutEvent);
    _eventReceived += Pt::slot(*this, &Application::onProcessLayoutEvent);


    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchPaintEvent);
    _eventReceived += Pt::slot(*this, &Application::onProcessPaintEvent);

    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchResizeEvent);
    _eventReceived += Pt::slot(*this, &Application::onProcessResizeEvent);

    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchMoveEvent);
    _eventReceived += Pt::slot(*this, &Application::onProcessMoveEvent);

    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchActivateEvent );
    _eventReceived += Pt::slot(*this, &Application::onProcessActivateEvent);

    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchEnableEvent );
    _eventReceived += Pt::slot(*this, &Application::onProcessEnableEvent );

    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchShowEvent );
    _eventReceived += Pt::slot(*this, &Application::onProcessShowEvent );

    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchCloseEvent );
    _eventReceived += Pt::slot(*this, &Application::onProcessCloseEvent );

    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchFocusEvent );
    _eventReceived += Pt::slot(*this, &Application::onProcessFocusEvent );

    loop().eventReceived() += Pt::slot(*this, &Application::onDispatchWindowStateEvent);
    _eventReceived += Pt::slot(*this, &Application::onProcessWindowStateEvent);

    _defaultInputMethod = new DefaultInputMethod;
    setInputMethod(*_defaultInputMethod);

    _textCursorTimer.timeout() += Pt::slot(*this, &Application::onTextCursor);
    _textCursorTimer.setActive(loop());
    _textCursorTimer.start(500);

    setFont(_font);
    loadCursors();
    loadIcons();

    _toolTip.setEnabled(&loop());
}


Application::~Application()
{
    _toolTip.setEnabled(nullptr);

    if(_inputMethod)
        removeInputMethod(*_inputMethod);

    delete _defaultInputMethod;

    delete _mainScreen;
    delete _impl;
}

void Application::invertIcons()
{
    std::vector<gfx::Icon*> icos = resourceMng().getAll<gfx::Icon>();

    for( size_t i = 0; i < icos.size(); ++i)
        icos[i]->invert();

    for( size_t i = 0;i < screen().windows().size(); ++i)
    {
        Window* w = screen().windows()[i]; 
        w->setIconId(w->iconId());
    }
}

void Application::loadIcons()
{
    gfx::Icon* icon = nullptr;

    //Undo
    icon = new gfx::Icon();
    icon->loadXpm(IconUndo);
    resourceMng().add(*icon, "UndoIcon");

    //Redo
    icon = new gfx::Icon();
    icon->loadXpm(IconRedo);
    resourceMng().add(*icon, "RedoIcon");

    //Cut
    icon = new gfx::Icon();
    icon->loadXpm(IconCut);
    resourceMng().add(*icon, "CutIcon");

    //Copy
    icon = new gfx::Icon();
    icon->loadXpm(IconCopy);
    resourceMng().add(*icon, "CopyIcon");

    //Paste
    icon = new gfx::Icon();
    icon->loadXpm(IconPaste);
    resourceMng().add(*icon, "PasteIcon");

    //Delete
    icon = new gfx::Icon();
    icon->loadXpm(IconDelete);
    resourceMng().add(*icon, "DeleteIcon");
}

void Application::loadCursors()
{
    Cursor* cursor = nullptr;
    
    //Move
    cursor = new Cursor();
    cursor->loadCursor(cursor_crosshairs_xpm);
    cursor->setName("MoveCursor");
    cursor->setHotSpot(gfx::Point(7, 8));
    resourceMng().add(*cursor, cursor->name());

    //Arrow
    cursor = new Cursor();
    cursor->loadCursor(_2_default);
    cursor->setName("ArrowCursor");
    resourceMng().add(*cursor, cursor->name());

    //Wait
    cursor = new Cursor();
    cursor->loadCursor(wait01_xpm);
    cursor->setName("WaitCursor");
    resourceMng().add(*cursor, cursor->name());

    //Size NWSE
    cursor = new Cursor();
    cursor->loadCursor(size_NWSE);
    cursor->setName("SizeNWSECursor");
    cursor->setHotSpot(gfx::Point(7, 7));
    resourceMng().add(*cursor, cursor->name());

    //Size NESW
    cursor = new Cursor();
    cursor->loadCursor(size_NESW);
    cursor->setName("SizeNESWCursor");
    cursor->setHotSpot(gfx::Point(7, 7));
    resourceMng().add(*cursor, cursor->name());

    //Size 	WE
    cursor = new Cursor();
    cursor->loadCursor(_2_size_hor);
    cursor->setName("SizeWECursor");
    cursor->setHotSpot(gfx::Point(12, 6));
    resourceMng().add(*cursor, cursor->name());

    //Size 	NS
    cursor = new Cursor();
    cursor->loadCursor(_2_size_ver);
    cursor->setName("SizeNSCursor");
    cursor->setHotSpot(gfx::Point(5, 10));
    resourceMng().add(*cursor, cursor->name());

    //Size 	Text
    cursor = new Cursor();
    cursor->loadCursor(cursor_text_insert_xpm);
    cursor->setXHotspot(8);
    cursor->setYHotspot(10);
    cursor->setName("TextCursor");
    resourceMng().add(*cursor, cursor->name());
}



void Application::initSystemColors()
{

    _brightSystemColors.clear();
    //Workspace
    _brightSystemColors.add((int)SystemColors::ColorType::Desktop, gfx::ArgbColor(171, 171, 171));
    _brightSystemColors.add((int)SystemColors::ColorType::Workingspace, gfx::ArgbColor(191, 191, 191));

    //Window
    _brightSystemColors.add((int)SystemColors::ColorType::Window, gfx::ArgbColor(255, 255 ,255));
    _brightSystemColors.add((int)SystemColors::ColorType::WindowTitleText, gfx::ArgbColor(0, 0, 0));
    _brightSystemColors.add((int)SystemColors::ColorType::WindowTitleBar, gfx::ArgbColor(232, 232, 232));
    _brightSystemColors.add((int)SystemColors::ColorType::WindowTitleBarActive, gfx::ArgbColor(240, 244, 249));
    _brightSystemColors.add((int)SystemColors::ColorType::WindowTitleBarDisabled, gfx::ArgbColor(192, 205, 219));
    _brightSystemColors.add((int)SystemColors::ColorType::WindowFrame, gfx::ArgbColor(222, 222, 222));
    _brightSystemColors.add((int)SystemColors::ColorType::WindowBorder, gfx::ArgbColor(232, 232, 232));
    _brightSystemColors.add((int)SystemColors::ColorType::WindowBorderActive, gfx::ArgbColor(240, 244, 249));
    _brightSystemColors.add((int)SystemColors::ColorType::WindowBorderDisabled, gfx::ArgbColor(192, 205, 219));
    //Control
    _brightSystemColors.add((int)SystemColors::ColorType::Control, gfx::ArgbColor(240, 240, 240));
    _brightSystemColors.add((int)SystemColors::ColorType::ControlDark, gfx::ArgbColor(160, 160, 160));
    _brightSystemColors.add((int)SystemColors::ColorType::ControlDarkDark, gfx::ArgbColor(100, 100, 100));

    _brightSystemColors.add((int)SystemColors::ColorType::ControlLight, gfx::ArgbColor(245, 245, 245));
    _brightSystemColors.add((int)SystemColors::ColorType::ControlLightLight, gfx::ArgbColor(250, 250, 250));
    _brightSystemColors.add((int)SystemColors::ColorType::ControlDisabled, gfx::ArgbColor(242, 242, 242));
    _brightSystemColors.add((int)SystemColors::ColorType::ControlHighlight, gfx::ArgbColor(229, 243, 255));
    _brightSystemColors.add((int)SystemColors::ColorType::ControlFrame, gfx::ArgbColor(180, 180, 180));
    _brightSystemColors.add((int)SystemColors::ColorType::ControlFrameDisabled, gfx::ArgbColor(180, 180, 180));
    _brightSystemColors.add((int)SystemColors::ColorType::ControlText, gfx::ArgbColor(0, 0, 0));
    _brightSystemColors.add((int)SystemColors::ColorType::ControlTextDisabled, gfx::ArgbColor(109, 109, 109));
    _brightSystemColors.add((int)SystemColors::ColorType::ControlTextHighlight, gfx::ArgbColor(196, 213, 255));

    //Text
    _brightSystemColors.add((int)SystemColors::ColorType::TextBackground, gfx::ArgbColor(255, 255, 255));
    _brightSystemColors.add((int)SystemColors::ColorType::Text, gfx::ArgbColor(0, 0, 0));
    _brightSystemColors.add((int)SystemColors::ColorType::TextHighlight, gfx::ArgbColor(204, 232, 255));
    _brightSystemColors.add((int)SystemColors::ColorType::TextDisabled, gfx::ArgbColor(192, 205, 219));

    //Info
    _brightSystemColors.add((int)SystemColors::ColorType::Info, gfx::ArgbColor(241, 242, 247));
    _brightSystemColors.add((int)SystemColors::ColorType::InfoText, gfx::ArgbColor(0, 0, 0));
    _brightSystemColors.setName("BrightSystemColors");
    gfx::ColorManager::instance().add(0, _brightSystemColors);


    for(int i = 0; i < (int)SystemColors::ColorType::Count; ++i)
    {
        const gfx::ArgbColor* color = _brightSystemColors.get(i);

        if(color == nullptr)
            continue;

        gfx::ArgbColor complementary(color->alpha(),
                                     255 - color->red(),
                                     255 - color->green(),
                                     255 - color->blue());

        _darkSystemColors.add(i, complementary);
    }

    _darkSystemColors.setName("DarkSystemColors");
}

gfx::ColorPalette& Application::systemColors()
{
    return *gfx::ColorManager::instance().get(0);
}

const gfx::ColorPalette& Application::systemColors() const
{
   return *gfx::ColorManager::instance().get(0);
}

ApplicationImpl* Application::impl()
{
    return _impl;
}

Application& Application::instance()
{
    return static_cast<Application&>( Pt::System::Application::instance() );
}

void Application::onTextCursor()
{
    _textCursorState = !_textCursorState;    
    _textCursorSignal.send();
}


bool Application::isSystemDarkMode() const
{
    return _impl->isSystemDarkMode();
}


const Pt::String& Application::tr(const Pt::String& key) const
{  
    const base::TextTable* table = resourceMng().get<base::TextTable>(_languageCode);

    if( table  == nullptr)
        return key;
    
    return table->tr(key);
}

const Screen& Application::screen() const
{
    return *_mainScreen;
}

Screen& Application::screen()
{
    return *_mainScreen;
}

Pt::Timespan Application::inactivityTime() const
{
    return _impl->inactivityTime();
}

void Application::setCursor(const Cursor* cursor)
{
   _impl->setCursor( cursor );

   if(cursor)
     _currentCursor = *cursor;
   else
     _currentCursor.clear();  
}

void Application::setFontDir(const Pt::System::Path& dir)
{
    _impl->setFontDir(dir);
}

void Application::setFont(const gfx::Font& font)
{
    _font = font;
    _impl->setDefaultFont(_font);
}

void Application::loadImage(const Pt::System::Path& path, gfx::Pixmap& image)
{
    if( ! Pt::System::FileInfo::exists(path) )
    {
        image = gfx::Pixmap();
        return;
    }

    std::ifstream fs(path.toLocal().c_str(), std::ios::binary);

    _iconReader.reset();
    _iconReader.attach(fs, image);
    _iconReader.read();
}




InputMethod& Application::inputMethod()
{
    return *_inputMethod;
}

void Application::setInputMethod(InputMethod& im)
{
    if(_inputMethod)
        removeInputMethod(*_inputMethod);

    _inputMethod = &im;
    im.registerApplication(*this);
}

void Application::removeInputMethod(InputMethod& im)
{
    if(_inputMethod == &im)
    {
        _inputMethod = _defaultInputMethod;
        im.unregisterApplication(*this);
    }
}
Element* Application::findVisual(Pt::uint64_t id)
{
    ElementMap::iterator it =_visuals.find(id);
    return it != _visuals.end() ? it->second : 0; 
}

void Application::registerElement(Element& visual)
{
    ElementMap::const_iterator it = _visuals.find( visual.id() );
    assert( it == _visuals.end() );

    ElementMap::value_type elem(visual.id(), &visual);
    _visuals.insert(elem);
}

void Application::unregisterElement(Element& visual)
{
   _visuals.erase( visual.id() );

   if( _pointerOverWidget == &visual)
       _pointerOverWidget = nullptr;
    
}

void Application::nextEvent()
{
    _impl->nextEvent();
}

void Application::commitEvent(const Pt::Event& ev)
{
    loop().commitEvent(ev);    
}

void Application::processEvent(const Pt::Event& ev)
{
    _eventReceived.send(ev);
}

Pt::Signal<const Pt::Event&>&  Application::eventReceived()
{
    return _eventReceived;
}

void Application::invalidate()
{
    ElementMap::iterator it = _visuals.begin();
  
    for( ; it != _visuals.end(); ++it)
    {
        it->second->invalidate();
    }
}

void Application::sendKeyEvent(const KeyEvent& ev)
{
    _impl->sendKeyEvent(ev);
}

void Application::sendMouseEvent(const MouseEvent& ev)
{
    _impl->sendMouseEvent(ev);
}


void Application::setColors(Colors v, gfx::ColorPalette* c)
{
    _userColor = v; 

    switch (v)
    {
        case Colors::BrightColors:
            gfx::ColorManager::instance().set(0, _brightSystemColors);

            if (_colors == Colors::DarkColors)    
                invertIcons();

            _colors = Colors::BrightColors; 
        break;

        case Colors::DarkColors:
            gfx::ColorManager::instance().set(0, _darkSystemColors);

            if (_colors == Colors::BrightColors)
                invertIcons();

            _colors = Colors::DarkColors;
        break;

        case Colors::SystemColors:
            if (isSystemDarkMode()) 
            {
                gfx::ColorManager::instance().set(0, _darkSystemColors);                

                if (_colors == Colors::BrightColors)
                    invertIcons();

                _colors = Colors::DarkColors;
            }
            else
            {
                gfx::ColorManager::instance().set(0, _brightSystemColors);
                

                if (_colors == Colors::DarkColors)
                    invertIcons();  

                _colors = Colors::BrightColors;
            }
        break;        

        case Colors::UserColors:
        {
            if (c)
                gfx::ColorManager::instance().set(0, *c);
        }
        break;
    }

    invalidate();
}

Element* Application::capture() const
{
    if (!_capture.empty())
        return _capture.back();

    return nullptr;
}

void Application::onRequestCapture(Element& target, bool isCapture)
{
    std::list<Element*>::iterator it = std::find(_capture.begin(), _capture.end(), &target);

    if (it != _capture.end())
        _capture.erase(it);

    if (isCapture)
        _capture.push_back(&target);

    Element* capture = this->capture();

    _mainScreen->impl()->setCapture(capture);
}

void Application::onDispatchMouseEvent(const MouseEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessMouseEvent(ev);
}

void Application::onProcessMouseEvent(const MouseEvent& ev)
{
    _toolTip.setMouse(ev.position());   

    Element* visual = ev.element();
    gfx::Point screenPos = ev.position();


    Element* capture = this->capture();

    Element* hit = _mainScreen->hitTest(screenPos, HitTestHint::HitAll);

    // IME window
    Window* ime = inputMethod().activeWindow();

    if(ime)
    {
        if(hit)
        {
            if( ime == hit || ime->isAncestorOf(*hit) )
            {
                // TODO: also if capture is in a popup that is related to the IME
                if( capture && capture->isDescendantOf(*ime) )
                {
                    capture->processEvent(ev);
                }
                else
                {
                    MouseEvent mev = ev;
                    mev.setElement(ime);
                    _mainScreen->processEvent(mev);
                }

                return;
            }
        }
    }


    // hide IME window
    if( ev.isPress(MouseEvent::Left) )
    {
        Element* receiver = inputMethod().receiver();
        if(hit && receiver)
        {
            bool keepOpen = receiver == hit || hit->isDescendantOf(*receiver);

            if( ! keepOpen )
                inputMethod().finish();
        }
    }
        
    
    //Mouse over handling Enter/Leave event     
    if (hit == nullptr)
    {
        if (_pointerOverWidget)
        {
            LeaveEvent leaveEv(*_pointerOverWidget);
            _pointerOverWidget->processEvent(leaveEv);
            _pointerOverWidget = nullptr; 
        }
    }
    else
    {
        if (hit != _pointerOverWidget)
        {
            if(_pointerOverWidget)
            {
                //Leave
                LeaveEvent leaveEv(*_pointerOverWidget);
                _pointerOverWidget->processEvent(leaveEv);
            }
            //New hit test because leave can destroy the widget
            hit = _mainScreen->hitTest(screenPos, HitTestHint::HitAll);

            if (hit && hit->isEnabled() && hit->isVisible())
            {
                _pointerOverWidget = hit;

                //Enter
                EnterEvent enterEv(*_pointerOverWidget);
                _pointerOverWidget->processEvent(enterEv);
            }
            else
            {
                _pointerOverWidget = nullptr;
            }
        }
    }    
    
    capture = this->capture();

    if( capture)
        capture->processEvent(ev); 
    else
        _mainScreen->processEvent(ev);    
}

void Application::setPointed(Element* v)
{
    if (_pointerOverWidget && _pointerOverWidget != v)
    {
        LeaveEvent leaveEv(*_pointerOverWidget);
        _pointerOverWidget->processEvent(leaveEv);
    }   
    
    _pointerOverWidget = v;
}

void Application::onDispatchTouchEvent(const TouchEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessTouchEvent(ev);
}

void Application::onProcessTouchEvent(const TouchEvent& ev)
{
    //std::clog << "APP TOUCH: " << ev.position().x() << ", " << ev.position().y() << std::endl;

    Element* visual = ev.element();
    gfx::Point screenPos = ev.position();

    //
    // Detect scroll gestures
    //
    onDetectScroll( visual, screenPos, ev.isPress(), ev.isPressed() );

    Element* capture = this->capture();

    //
    // IME window
    //
    Window* ime = inputMethod().activeWindow();
    if(ime)
    {
        Element* hit = _mainScreen->hitTest(screenPos, HitTestHint::HitAll);
        if(hit)
        {
            if( ime == hit || ime->isAncestorOf(*hit) )
            {
                // TODO: also if capture is in a popup that is related to the IME
                if( capture && capture->isDescendantOf(*ime) )
                {
                    capture->processEvent(ev);
                }
                else
                {
                    TouchEvent tev = ev;
                    tev.setElement(ime);
                    _mainScreen->processEvent(tev);
                }

                return;
            }
        }
    }

    //
    // hide IME window
    //
    if( ev.isPress() )
    {
        Element* hit = _mainScreen->hitTest(screenPos, HitTestHint::HitAll);
        Element* receiver = inputMethod().receiver();
        if(hit && receiver)
        {
            bool keepOpen = receiver == hit ||
                            hit->isDescendantOf(*receiver);
            if( ! keepOpen )
                inputMethod().finish();
        }
    }

    //
    // dispatch event
    //
    if(capture)
    {
        capture->processEvent(ev);
    }
    else
    {
        _mainScreen->processEvent(ev);
    }
}

void Application::onDetectScroll(Element* visual, const gfx::Point& screenPos, bool isPress, bool isPressed)
{
    const int threshold = 8;
    
    // TODO: start scroll only if within visual

    if(isPress)
    {
        //std::clog << "SCROLL START" << std::endl;
        _scrollFrom = screenPos;
    }
    else if(isPressed)
    {
        int deltaY = screenPos.y() - _scrollFrom.y();
        
        if( ! _onScroll && std::fabs(deltaY) > threshold )
        {
            _onScroll = true;

            if(deltaY > 0)
                _scrollFrom.addY(threshold);
            else
                _scrollFrom.subY(threshold);

            deltaY = screenPos.y() - _scrollFrom.y();
            //std::clog << "SCROLL STARTED: " << deltaY << std::endl;
            
            ScrollEvent sev(*visual);
            sev.set(ScrollEvent::Vertical, deltaY);
            processEvent(sev);

            _scrollFrom = screenPos;
        }
        else if(_onScroll)
        {
            //std::clog << "SCROLLING: " << deltaY << std::endl;
            ScrollEvent sev(*visual);
            sev.set(ScrollEvent::Vertical, deltaY);
            processEvent(sev);

            _scrollFrom = screenPos;
        }
    }
    else
    {
        if(_onScroll)
        {
            //std::clog << "SCROLL STOP" << std::endl;
            _onScroll = false;
        }
    }
}

void Application::onDispatchScrollEvent(const ScrollEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessScrollEvent(ev);
}

void Application::onProcessScrollEvent(const ScrollEvent& ev)
{
    _mainScreen->processEvent(ev);
}

void Application::onDispatchEnterEvent(const EnterEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessEnterEvent(ev);
}

void Application::onProcessEnterEvent(const EnterEvent& ev)
{
    Element* visual = ev.element();
    visual->processEvent(ev);
}

void Application::onDispatchLeaveEvent(const LeaveEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessLeaveEvent(ev);
}

void Application::onProcessLeaveEvent(const LeaveEvent& ev)
{
    Element* visual = ev.element();
    visual->processEvent(ev);
}

void Application::onDispatchKeyEvent(const KeyEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessKeyEvent(ev);
}

void Application::onProcessKeyEvent(const KeyEvent& ev)
{
    Element* visual = ev.element();

    if(visual)
        visual->processEvent(ev);
    else
        _mainScreen->processEvent(ev);
}

void Application::onDispatchInvalidateEvent(const InvalidateEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessInvalidateEvent(ev);
}

void Application::onProcessInvalidateEvent(const InvalidateEvent& ev)
{
    Element* visual = ev.element();
    if(visual)
        visual->processEvent(ev);
}

void Application::onDispatchLayoutEvent(const LayoutEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessLayoutEvent(ev);
}

void Application::onProcessLayoutEvent(const LayoutEvent& ev)
{
    Element* visual = ev.element();
    if(visual)
        visual->processEvent(ev);
}


void Application::onDispatchPaintEvent(const PaintEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    //onProcessPaintEvent(ev);
    processEvent(ev);
}

void Application::onProcessPaintEvent(const PaintEvent& ev)
{
    Element* visual = ev.element();
    if(visual)
        visual->processEvent(ev);
}

void Application::onDispatchMoveEvent(const MoveEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    processEvent(ev);
}

void Application::onProcessMoveEvent(const MoveEvent& ev)
{
    Element* visual = ev.element();
    if(visual)
        visual->processEvent(ev);
}

void Application::onDispatchResizeEvent(const ResizeEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    processEvent(ev);
}

void Application::onProcessResizeEvent(const ResizeEvent& ev)
{
    Element* visual = ev.element();
    if(visual)
        visual->processEvent(ev);
}

void Application::onDispatchActivateEvent(const ActivateEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    processEvent(ev);
}

void Application::onProcessActivateEvent(const ActivateEvent& ev)
{
    Element* visual = ev.element();
    if(visual)
        visual->processEvent(ev);
}

void Application::onDispatchEnableEvent(const EnableEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessEnableEvent(ev);
}

void Application::onProcessEnableEvent(const EnableEvent& ev)
{
    Element* visual = ev.element();
    if(visual)
        visual->processEvent(ev);
}

void Application::onDispatchShowEvent(const ShowEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessShowEvent(ev);
}

void Application::onProcessShowEvent(const ShowEvent& ev)
{
    Element* visual = ev.element();
    if(visual)
        visual->processEvent(ev);
}

void Application::onDispatchCloseEvent(const CloseEvent& ev )
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessCloseEvent(ev);
}

void Application::onProcessCloseEvent(const CloseEvent& ev)
{
    Element* visual = ev.element();
    if(visual)
        visual->processEvent(ev);
}

void Application::onDispatchFocusEvent(const FocusEvent& ev)
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessFocusEvent(ev);
}

void Application::onProcessFocusEvent(const FocusEvent& ev)
{
    Element* visual = ev.element();
    if(visual)
        visual->processEvent(ev);
}

void Application::onDispatchWindowStateEvent(const WindowStateEvent& ev )
{
    // make sure receiver still exists
    ElementMap::iterator it = _visuals.find( ev.id() );
    if( it == _visuals.end() )
        return;

    onProcessWindowStateEvent(ev);
}

void Application::onProcessWindowStateEvent(const WindowStateEvent& ev )
{
    Element* visual = ev.element();
    if(visual)
        visual->processEvent(ev);
}

}}}


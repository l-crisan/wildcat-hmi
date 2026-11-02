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
#include "ApplicationImpl.h"
#include "ScreenImpl.h"
#include "Keycodes.h"
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/gfx/Graphics.h>
#include <Pt/System/Logger.h>
#include <Pt/System/Clock.h>
#include <Pt/DateTime.h>
#include <Pt/Utf8Codec.h>

#include <emscripten.h>
#include <emscripten/threading.h>
#include <pthread.h>
#include <vector>

namespace wildcat {
namespace gui {
namespace window {

ApplicationImpl::ApplicationImpl()
: _exiting(false)
, _wakePending(false)
, _lastActivityTime(Pt::System::Clock::getSystemTime())
{
    pthread_t self = pthread_self();

    emscripten_set_keydown_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, &ApplicationImpl::onKeyDown, self);
    emscripten_set_keyup_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, &ApplicationImpl::onKeyUp, self);

    emscripten_set_mousedown_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, &ApplicationImpl::onMouseEvent, self);
    emscripten_set_mouseup_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, &ApplicationImpl::onMouseEvent, self);
    emscripten_set_mousemove_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, &ApplicationImpl::onMouseEvent, self);
    emscripten_set_wheel_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, &ApplicationImpl::onWheelEvent, self);
   
}

ApplicationImpl::~ApplicationImpl()
{
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, 0);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, 0);

    emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, 0);
    emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, 0);
    emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, 0);
    emscripten_set_wheel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, 0);
}

void ApplicationImpl::setCursor(const Cursor* cursor)
{    
    if(cursor == nullptr || cursor->name() == "ArrowCursor")
    {
        MAIN_THREAD_EM_ASM({
            var canvas = document.querySelector('canvas');
            if(canvas) canvas.style.cursor = 'default';
        });

        return;
    }

    if( _cursor == *cursor)
        return;

    if(cursor->empty())
    {
        MAIN_THREAD_EM_ASM({
            var canvas = document.querySelector('canvas');
            if(canvas) canvas.style.cursor = 'none';
        });

        return;
    }
    // For custom cursors, create a data URL from the cursor image
    const gfx::Pixmap& pixmap = cursor->pixmap();
    if(!pixmap.empty())
    {
        int w = cursor->width();
        int h = cursor->height();
        int xHot = cursor->xHotspot();
        int yHot = cursor->yHotspot();

        // Convert BGRA to RGBA for canvas
        std::vector<uint8_t> rgba(w * h * 4);
        const uint8_t* src = pixmap.data();

        for(int i = 0; i < w * h * 4; i += 4)
        {
            rgba[i]     = src[i + 2];  // R
            rgba[i + 1] = src[i + 1];  // G
            rgba[i + 2] = src[i];      // B
            rgba[i + 3] = src[i + 3];  // A
        }

        MAIN_THREAD_EM_ASM({
            var w = $0;
            var h = $1;
            var xHot = $2;
            var yHot = $3;
            var dataPtr = $4;

            var tmpCanvas = document.createElement('canvas');
            tmpCanvas.width = w;
            tmpCanvas.height = h;
            var ctx = tmpCanvas.getContext('2d');

            var imgData = ctx.createImageData(w, h);
            var view = HEAPU8.subarray(dataPtr, dataPtr + w * h * 4);
            imgData.data.set(view);
            ctx.putImageData(imgData, 0, 0);

            var dataUrl = tmpCanvas.toDataURL();
            var canvas = document.querySelector('canvas');
            if(canvas) {
                canvas.style.cursor = 'url(' + dataUrl + ') ' + xHot + ' ' + yHot + ', auto';
            }
        }, w, h, xHot, yHot, rgba.data());
    }
    else
    {
        // Fallback to default cursor
        MAIN_THREAD_EM_ASM({
            var canvas = document.querySelector('canvas');
            if(canvas) canvas.style.cursor = 'default';
        });
    }
}

Pt::Timespan ApplicationImpl::inactivityTime() const
{
    Pt::DateTime now = Pt::System::Clock::getSystemTime();
    Pt::Timespan inactivity = now - _lastActivityTime;
    return inactivity;
}

void ApplicationImpl::setFontDir(const Pt::System::Path& dir)
{
    gfx::Graphics::setFontDir(dir);
}

void ApplicationImpl::setDefaultFont(const gfx::Font& font)
{
    gfx::Graphics::setDefaultFont(font);
}

void ApplicationImpl::sendKeyEvent(const KeyEvent& ev)
{
    _lastActivityTime = Pt::System::Clock::getSystemTime();
}

void ApplicationImpl::sendMouseEvent(const MouseEvent& ev)
{
}

void ApplicationImpl::nextEvent()
{
    this->waitNext();
}

void ApplicationImpl::onAttachSelectable(Pt::System::Selectable&)
{
}

void ApplicationImpl::onDetachSelectable(Pt::System::Selectable&)
{
}

void ApplicationImpl::onCancel(Pt::System::Selectable& s)
{
}

void ApplicationImpl::onReady(Pt::System::Selectable& s)
{
}


bool ApplicationImpl::isSystemDarkMode() const
{
    int dark = MAIN_THREAD_EM_ASM_INT({
    if (window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches) {
        return 1;
    }
    return 0;
        });

    return dark == 1;
}


bool ApplicationImpl::waitNext()
{
    _timerQueue.processTimers();

    emscripten_current_thread_process_queued_calls();

    {
        Pt::System::MutexLock lock(_wakeMutex);

        if (!_wakePending)
            _wakeCondition.wait(_wakeMutex, static_cast<unsigned int>(10));

        _wakePending = false;
    }
    

    this->processEvents();

    return !_exiting;
}

void ApplicationImpl::onRun()
{
    while (this->waitNext())
        ;
}

void ApplicationImpl::run()
{
    this->onRun();
}

void ApplicationImpl::onExit()
{
    _exiting = true;

    _eventQueue.exit();
    wake();
}

void ApplicationImpl::onCommitEvent(const Pt::Event& ev)
{
    _eventQueue.pushEvent(ev);
    wake();
}

void ApplicationImpl::onQueueEvent(const Pt::Event& ev)
{
    _eventQueue.pushEvent(ev);
}

void ApplicationImpl::onWake()
{
    Pt::System::MutexLock lock(_wakeMutex);
    _wakePending = true;
    _wakeCondition.signal();
}

void ApplicationImpl::onProcessEvents()
{
    _eventQueue.processEvents(this->eventReceived());


    ScreenImpl* screen = Application::instance().screen().impl();

    if (screen && screen->commitPending())
        screen->commitFrame();
}

void ApplicationImpl::onAttachTimer(Pt::System::Timer& timer)
{
    _timerQueue.addTimer(timer);

    wake();
}

void ApplicationImpl::onDetachTimer(Pt::System::Timer& timer)
{
    _timerQueue.removeTimer(timer);
    wake();
}

void ApplicationImpl::dispatchKeyEvent(const EmscriptenKeyboardEvent& e, bool press)
{
    _lastActivityTime = Pt::System::Clock::getSystemTime();

    Screen& screen = gui::window::Application::instance().screen();

    Key::Modifiers modifiers;

    if (e.shiftKey)
        modifiers.add(Key::Shift);

    if (e.ctrlKey)
        modifiers.add(Key::Control);

    if (e.altKey)
        modifiers.add(Key::Alt);

    if (e.metaKey)
        modifiers.add(Key::Meta);

    Key key(modifiers, toKeycode(e.code));

    if (press)
        _keyEvent.setPress(key, Pt::Char());
    else
        _keyEvent.setRelease(key, Pt::Char());

    _keyEvent.setElement(&screen);
    this->commitEvent(_keyEvent);

    //
    // NOTE: produces a fake key event with a character since keydown
    //       does not report composed characters
    //
    if (press)
    {
        Pt::String text = Pt::Utf8Codec::decode(e.key, std::strlen(e.key));
        if (text.size() == 1)
        {
            Key textKey;

            KeyEvent textEvent(screen);
            textEvent.setPress(textKey, text[0]);
            this->commitEvent(textEvent);
        }
    }
}



void ApplicationImpl::dispatchWheelEvent(const EmscriptenWheelEvent& e, int eventType)
{
    _lastActivityTime = Pt::System::Clock::getSystemTime();

    Screen& screen = Application::instance().screen();

    // Bestimme, ob es sich um vertikales oder horizontales Scrollen handelt
    ScrollEvent::Wheel wheel = ScrollEvent::Vertical;
    double delta = -e.deltaY;  // Negativ, damit Scroll nach unten positiv ist

    if (std::abs(e.deltaX) > std::abs(e.deltaY))
    {
        wheel = ScrollEvent::Horizontal;
        delta = -e.deltaX;
    }

    // Normalisiere die Delta-Werte basierend auf dem deltaMode
    if (e.deltaMode == DOM_DELTA_PIXEL)
    {
        // Pixelwerte normalisieren (typischer Wert ist ~100 pro Scroll-Schritt)
        delta = delta / 100.0;
    }
    else if (e.deltaMode == DOM_DELTA_LINE)
    {
        // Zeilen sind bereits normalisiert
        delta = delta;
    }
    else if (e.deltaMode == DOM_DELTA_PAGE)
    {
        // Seiten in Zeilen umrechnen (ca. 30 Zeilen pro Seite)
        delta = delta * 30.0;
    }
    
    _sev.setElement(&screen);
    _sev.set(wheel, delta);

    this->commitEvent(_sev);
}

void ApplicationImpl::dispatchMouseEvent(const EmscriptenMouseEvent& e, int eventType)
{
    _lastActivityTime = Pt::System::Clock::getSystemTime();

    Screen& screen = Application::instance().screen();
    const double scaling = screen.scaleFactor();

    gfx::Point pos(e.targetX, e.targetY);
    pos.scale(1.0/scaling);

    _mev.setElement(&screen);
    _mev.setPosition(pos);

    if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE)
    {
        _mev.setMove();
    }
    else
    {
        MouseEvent::Button button = e.button == 0 ? MouseEvent::Left :
            e.button == 2 ? MouseEvent::Right :
            MouseEvent::Middle;

        if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
            _mev.setPress(button);
        else
            _mev.setRelease(button);
    }

    this->commitEvent(_mev);
}

EM_BOOL ApplicationImpl::onWheelEvent(int eventType, const EmscriptenWheelEvent* e, void* userData)
{
    ApplicationImpl* app = static_cast<ApplicationImpl*>(userData);
    app->dispatchWheelEvent(*e, eventType);

    return true;  // Event wurde behandelt
}

EM_BOOL ApplicationImpl::onKeyDown(int /*eventType*/, const EmscriptenKeyboardEvent* e, void* userData)
{
    ApplicationImpl* app = static_cast<ApplicationImpl*>(userData);
    app->dispatchKeyEvent(*e, true);
    return true;
}

EM_BOOL ApplicationImpl::onKeyUp(int /*eventType*/, const EmscriptenKeyboardEvent* e, void* userData)
{
    ApplicationImpl* app = static_cast<ApplicationImpl*>(userData);
    app->dispatchKeyEvent(*e, false);
    return true;
}

EM_BOOL ApplicationImpl::onMouseEvent(int eventType, const EmscriptenMouseEvent* e, void* userData)
{
    ApplicationImpl* app = static_cast<ApplicationImpl*>(userData);
    app->dispatchMouseEvent(*e, eventType);
    
    return true;
}


}}}

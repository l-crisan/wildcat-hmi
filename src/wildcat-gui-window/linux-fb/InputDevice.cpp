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
  MA 02110-1301 USA*/
#include "InputDevice.h"
#include <Pt/System/EventLoop.h>
#include <Pt/String.h>
#include <Pt/Utf8Codec.h>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <linux/input.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


namespace wildcat { 
namespace gui {
namespace window {

InputDevice::InputDevice(const char* deviceName)
: _ioh(*this)
, _loop(nullptr)
, _shift(false)
, _control(false)
, _alt(false)
, _meta(false)
, _keyEvent()
, _mouseEvent()
, _touchCount(0)
, _touchEvent()
{
    _ioh.fd = ::open(deviceName, O_RDONLY|O_NONBLOCK);

    if(_ioh.fd < 0) 
        throw Pt::System::AccessFailed(deviceName);
}

InputDevice::InputDevice()
: _ioh(*this)
, _loop(nullptr)
, _shift(false)
, _control(false)
, _alt(false)
, _meta(false)
, _keyEvent()
, _mouseEvent()
, _touchCount(0)
, _touchEvent()
{
    _ioh.fd = -1;
}

InputDevice::~InputDevice()
{
    try 
    { 
        this->close(); 
    } 
    catch(...)
    {
    }
}

void InputDevice::open(const char* deviceName)
{
    close();

    _ioh.fd = ::open(deviceName, O_RDONLY|O_NONBLOCK);

    if(_ioh.fd < 0) 
        throw Pt::System::AccessFailed(deviceName);
}

void InputDevice::close()
{
    if(_ioh.fd != -1)
    {
        ::close(_ioh.fd);
        _ioh.fd = -1;
    }
}

void InputDevice::begin()
{
    if(! _loop) 
        throw std::logic_error("input device not active");

    Pt::System::Selector& selector = _loop->selector();

    selector.beginRead(&_ioh);
}

void InputDevice::onAttach(Pt::System::EventLoop& loop)
{
    _loop = &loop;
    _xkb.open();
    _keymap.open(); 
}

void InputDevice::onDetach(Pt::System::EventLoop&)
{    
    _xkb.close();    
    _keymap.close();
    _loop = nullptr;
}

void InputDevice::onCancel()
{
    throw std::logic_error("not implemented");
}

bool InputDevice::onRun()
{
    size_t             count = 0;
    struct input_event ev;
    size_t             evSize = sizeof(ev);

    for(;;)
    {
        ssize_t n = ::read(_ioh.fd, &ev, evSize);

        if(n <= 0) 
            break;
        ++count;

        switch(ev.type)
        {
            case EV_KEY: 
                onKey(ev); 
            break;

            case EV_REL: 
                onRelative(ev); 
            break;

            case EV_ABS: 
                onAbsolute(ev);
            break;

            case EV_SYN:
            {
                if(_touchCount > 0)
                {
                    _eventReady.send(_touchEvent);
                    if(_touchEvent.isRelease()) _touchCount = 0; else _touchEvent.setMove();
                }
            }
           break;
        }
    }
    return count > 0;
}

void InputDevice::onRelative(const input_event& ev)
{
    _mouseEvent.setMove();

    if(ev.code == REL_X) 
        _mouseEvent.setX(_mouseEvent.x() + static_cast<double>(ev.value));
    else if(ev.code == REL_Y) 
        _mouseEvent.setY(_mouseEvent.y() + static_cast<double>(ev.value));

    if(_mouseEvent.x() < 0) 
        _mouseEvent.setX(0);

    if(_mouseEvent.x() >= _screenWidth) 
        _mouseEvent.setX(_screenWidth - 1);

    if(_mouseEvent.y() < 0) 
        _mouseEvent.setY(0);

    if(_mouseEvent.y() >= _screenHeight) 
        _mouseEvent.setY(_screenHeight - 1);

    _eventReady.send(_mouseEvent);
}

void InputDevice::onAbsolute(const input_event& ev)
{
    switch(ev.code)
    {
        case ABS_MT_SLOT:
        case ABS_MT_TRACKING_ID:
        case ABS_MT_POSITION_Y: 
            return;

        case ABS_MT_POSITION_X: 
            return;

        case ABS_MT_PRESSURE:
            return;

        case ABS_X:
            _touchEvent.setX(static_cast<double>(ev.value));
             _touchCount++; 
        break;

        case ABS_Y:
            _touchEvent.setY(static_cast<double>(ev.value));
            _touchCount++; 
        break;

        case ABS_PRESSURE: 
        return;        
    }
}

uint32_t InputDevice::transHardcoded(unsigned evcode, Pt::Char& ch)
{    
    ch = 0;

    switch(evcode)
    {
        case KEY_A: 
            ch='a'; 
            return Key::A;

        case KEY_B: 
            ch='b'; 
            return Key::B;

        case KEY_C:
            ch='c'; 
            return Key::C;

        case KEY_D:
            ch='d'; 
            return Key::D;

        case KEY_E: 
            ch='e'; 
            return Key::E;

        case KEY_F: 
            ch='f'; 
            return Key::F;

        case KEY_G: 
            ch='g'; 
            return Key::G;

        case KEY_H: 
            ch='h'; 
            return Key::H;

        case KEY_I: 
            ch='i'; 
            return Key::I;

        case KEY_J: 
            ch='j'; 
            return Key::J;

        case KEY_K: 
            ch='k'; 
            return Key::K;

        case KEY_L: 
            ch='l'; 
            return Key::L;

        case KEY_M: ch='m'; return Key::M;
        case KEY_N: ch='n'; return Key::N;
        case KEY_O: ch='o'; return Key::O;
        case KEY_P: ch='p'; return Key::P;
        case KEY_Q: ch='q'; return Key::Q;
        case KEY_R: ch='r'; return Key::R;
        case KEY_S: ch='s'; return Key::S;
        case KEY_T: ch='t'; return Key::T;
        case KEY_U: ch='u'; return Key::U;
        case KEY_V: ch='v'; return Key::V;
        case KEY_W: ch='w'; return Key::W;
        case KEY_X: ch='x'; return Key::X;
        case KEY_Y: ch='y'; return Key::Y;
        case KEY_Z: ch='z'; return Key::Z;

        case KEY_1: ch='1'; return Key::D1;
        case KEY_2: ch='2'; return Key::D2;
        case KEY_3: ch='3'; return Key::D3;
        case KEY_4: ch='4'; return Key::D4;
        case KEY_5: ch='5'; return Key::D5;
        case KEY_6: ch='6'; return Key::D6;
        case KEY_7: ch='7'; return Key::D7;
        case KEY_8: ch='8'; return Key::D8;
        case KEY_9: ch='9'; return Key::D9;
        case KEY_0: ch='0'; return Key::D0;

        case KEY_KP0: return Key::NumPad0; 
        case KEY_KP1: return Key::NumPad1; 
        case KEY_KP2: return Key::NumPad2;
        case KEY_KP3: return Key::NumPad3;
        case KEY_KP4: return Key::NumPad4; 
        case KEY_KP5: return Key::NumPad5;
        case KEY_KP6: return Key::NumPad6; 
        case KEY_KP7: return Key::NumPad7; 
        case KEY_KP8: return Key::NumPad8;
        case KEY_KP9: return Key::NumPad9;

        case KEY_SPACE: ch=' '; return Key::Space;
        case KEY_ENTER:
        case KEY_KPENTER: ch='\r'; return Key::Return;
        case KEY_BACKSPACE: ch='\b'; return Key::Backspace;
        case KEY_TAB: ch='\t'; return Key::Tab;
        case KEY_ESC: return Key::Escape;

        case KEY_LEFT: return Key::ArrowLeft; 
        case KEY_RIGHT: return Key::ArrowRight;
        case KEY_UP: return Key::ArrowUp; 
        case KEY_DOWN: return Key::ArrowDown;

        case KEY_HOME: return Key::Home;
        case KEY_END: return Key::End;
        case KEY_PAGEUP: return Key::PageUp; 
        case KEY_PAGEDOWN: return Key::PageDown;
        case KEY_INSERT: return Key::Insert; 
        case KEY_DELETE: return Key::Delete;

        case KEY_F1: return Key::F1; 
        case KEY_F2: return Key::F2; 
        case KEY_F3: return Key::F3;
        case KEY_F4: return Key::F4;
        case KEY_F5: return Key::F5; 
        case KEY_F6: return Key::F6; 
        case KEY_F7: return Key::F7; 
        case KEY_F8: return Key::F8;
        case KEY_F9: return Key::F9; 
        case KEY_F10: return Key::F10; 
        case KEY_F11: return Key::F11;
        case KEY_F12: return Key::F12;

        //case KEY_KP_DIVIDE: return Key::Divide;
        //case KEY_KP_MULTIPLY: return Key::Multiply;
        //case KEY_KP_SUBTRACT: return Key::Subtract; 
        //case KEY_KP_ADD: return Key::Add; 
        //case KEY_KP_DOT: return Key::Decimal;

        case KEY_MINUS: ch='-'; return static_cast<uint32_t>(ch);
        case KEY_EQUAL: ch='='; return static_cast<uint32_t>(ch);
        case KEY_BACKSLASH: ch='\\'; return static_cast<uint32_t>(ch);
        case KEY_SEMICOLON: ch=';'; return static_cast<uint32_t>(ch);
        case KEY_APOSTROPHE: ch='\''; return static_cast<uint32_t>(ch);
        case KEY_COMMA: ch=','; return static_cast<uint32_t>(ch);
        case KEY_DOT: ch='.'; return static_cast<uint32_t>(ch);
        case KEY_SLASH: ch='/'; return static_cast<uint32_t>(ch);
        case KEY_GRAVE: ch='`'; return static_cast<uint32_t>(ch);
        case KEY_LEFTBRACE: ch='['; return static_cast<uint32_t>(ch);
        case KEY_RIGHTBRACE: ch=']'; return static_cast<uint32_t>(ch);

        default: break;
    }
    return Key::NoKey;
}

uint32_t InputDevice::transIoctl(unsigned code, Pt::Char& outChar)
{
    struct kbentry ke; ke.kb_table=0; ke.kb_index=code;

    ::ioctl(STDIN_FILENO, KDGKBENT, &ke);

    const unsigned typ = KTYP(ke.kb_value);
    outChar = 0;

    switch(typ)
    { 
        case KT_LATIN:
        case KT_LETTER:
        {
            const unsigned ch = code & 0xFFu;
            switch(ch)
            {
                case 0x08: 
                    return Key::Backspace; 

                case 0x09: 
                    return Key::Tab; 

                case 0x0D: 
                    return Key::Return; 

                case 0x1B: 
                    return Key::Escape; 

                default: 
                break;
            }

            if(ch>=32 && ch<=126) 
            { 
                outChar = ch; 
                return Pt::toupper(ch).value(); 
            }
        }
        break;

        case KT_FN:
        {
            if(code<=23) 
                return Key::F1 + static_cast<uint32_t>(code);

            switch(code)
            {
                case 24: 
                    return Key::Insert; 

                case 25: 
                    return Key::Delete; 

                case 26: 
                    return Key::Home; 

                case 27: 
                    return Key::End; 

                case 28: 
                    return Key::PageUp; 

                case 29: 
                    return Key::PageDown; 

                default: 
                break;
            }
        }
        break;

        case KT_SPEC:
        {
            switch(code)
            {
                case 0: 
                    return Key::NoKey; 
                case 1: 
                    return Key::Return; 

                case 8: 
                    return Key::CapsLock; 

                default: 
                break;
            }
        }
        break;

        case KT_PAD:
        {
            if(code<10) 
                return Key::NumPad0 + static_cast<uint32_t>(code);

            switch(code)
            {
                case 10: 
                    return Key::Add; 

                case 11: 
                    return Key::Subtract; 

                case 12: 
                    return Key::Multiply; 

                case 13: 
                    return Key::Divide; 

                case 14: 
                    return Key::Return; 

                case 15: 
                    return Key::Decimal; 

                case 16: 
                    return Key::Separator; 
                default: 
                break;
             }
        }
        break;

        case KT_CUR:
        {
            switch(code)
            {
                case 0: 
                    return Key::ArrowDown; 

                case 1: 
                    return Key::ArrowLeft; 

                case 2: 
                    return Key::ArrowRight; 

                case 3: 
                    return Key::ArrowUp; 
                default: 
                break;
             }
        }
        break;
    }

    return Key::NoKey;
}

uint32_t InputDevice::trans(unsigned code, Pt::Char& ch)
{
    //Try XKB    
    uint32_t key = _xkb.translate(code, ch);    

    if( key  != Key::NoKey) 
        return key;
    
    //Try keymap
    key = _keymap.translate(code, ch);  

    if(key != Key::NoKey) 
        return key; 
    //Try IOCTL
    ch = 0;

    key = transIoctl(code, ch);

    if(key != Key::NoKey)
        return key;

    //Hardcoded keys(EN Layout)
    return transHardcoded(code, ch);
}

void InputDevice::onKey(const input_event& ev)
{    
    if(ev.code == 272) //(ev.code == BTN_TOUCH)
    {
        if(ev.value == 0) 
            _mouseEvent.setRelease(MouseEvent::Left); 
        else 
            _mouseEvent.setPress(MouseEvent::Left);

        _eventReady.send(_mouseEvent); return;
    }
    
    if(ev.code == 273) //(ev.code == BTN_RIGHT)
    {
        if(ev.value == 0) 
            _mouseEvent.setRelease(MouseEvent::Right); 
        else 
        _mouseEvent.setPress(MouseEvent::Right);

        _eventReady.send(_mouseEvent); return;
    }

    if(ev.code == 330)//(ev.code == BTN_TOUCH)
    {
        if(ev.value == 0) 
        { 
            _touchCount++; _touchEvent.setRelease(); 
        } 
        else 
        {
            _touchCount++; _touchEvent.setPress(); 
        }

        return;
    }

    uint32_t keyCode = Key::NoKey; 
    Pt::Char ch = 0;
    
    _xkb.updateState(ev.code, ev.value);
    
    switch(ev.code)
    {
        case KEY_LEFTSHIFT: 
        case KEY_RIGHTSHIFT: 
            if(ev.value==1)
            { 
                _shift=true; 
                keyCode=Key::ShiftKey;
            } 
            else 
            {
                _shift=false; 
            } 
        break;

        case KEY_LEFTCTRL:
        case KEY_RIGHTCTRL: 
            if(ev.value==1)
            { 
                _control=true; 
                keyCode=Key::ControlKey;
            } 
            else
            {
                _control=false; 
            }
        break;

        case KEY_LEFTALT:
        case KEY_RIGHTALT: 

            if(ev.value==1)
            { 
                _alt=true; 
                keyCode=Key::AltKey;
             } 
             else 
             {
                _alt=false; 
             }
       break;

        case KEY_LEFTMETA: 
        case KEY_RIGHTMETA: 
            if(ev.value==1)
            { 
                _meta=true; 
                keyCode=Key::MetaKey;
            } 
            else
            {
                _meta=false; 
            }
        break;

        default: 
            keyCode = trans(ev.code, ch); 
        break;

    }

    Key::Modifiers modifiers; 

    if(_shift) 
        modifiers.add(Key::Shift); 

    if(_control) 
        modifiers.add(Key::Control); 

   if(_alt) 
     modifiers.add(Key::Alt); 

   if(_meta) 
    modifiers.add(Key::Meta);

    Key key(modifiers, keyCode);
    
    if(ev.value == 1)
        _keyEvent.setPress(key, ch); 
    else 
        _keyEvent.setRelease(key, ch);

    _eventReady.send(_keyEvent);
}

}}}

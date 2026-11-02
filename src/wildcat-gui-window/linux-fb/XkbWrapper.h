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
#ifndef WILDCAT_GUI_WINDOW_XKBWRAPPER_H
#define WILDCAT_GUI_WINDOW_XKBWRAPPER_H


#include <Pt/Singleton.h>
#include <Pt/String.h>
#include <Pt/System/Library.h>
#include <inttypes.h>

namespace wildcat {
namespace gui {
namespace window {
    
typedef void* (*xkb_context_new_t)(int flags);
typedef void* (*xkb_keymap_new_from_names)(void*, void* names, int flags);
typedef void* (*xkb_state_new_t)(void* keymap);
typedef void (*xkb_unref_t)(void* ctx); 
typedef int (*xkb_state_update_key_t)(void* state, uint32_t key, int direction);
typedef int (*xkb_state_update_key_t)(void* state, uint32_t key,int direction);
typedef uint32_t (*xkb_state_key_get_one_sym_t)(void* state, uint32_t key);
typedef int (*xkb_state_key_get_utf8_t)(void* state, uint32_t  key, char* buffer, size_t size);

// sudo apt install libxkbcommon-dev
// sudo apt install libxkbcommon0
class XkbWrapper
{
    public:

        XkbWrapper();

        virtual ~XkbWrapper();

        void open();

        void updateState(uint32_t code, int value);  

        uint32_t translate(unsigned code, Pt::Char& ch);

        void close();

    private:
        void* _xkbCtx;
        void* _xkbKeymap;
        void* _xkbState;

    private:
        static Pt::System::Library _lib;   
        static xkb_context_new_t            _xkb_context_new;
        static xkb_keymap_new_from_names    _xkb_keymap_new_from_names;
        static xkb_state_new_t              _xkb_state_new;
        static xkb_unref_t                  _xkb_state_unref;
        static xkb_unref_t                  _xkb_context_unref;
        static xkb_unref_t                  _xkb_keymap_unref;
        static xkb_state_update_key_t       _xkb_state_update_key;
        static xkb_state_key_get_one_sym_t  _xkb_state_key_get_one_sym;    
        static xkb_state_key_get_utf8_t     _xkb_state_key_get_utf8;  
};

}}}


#endif
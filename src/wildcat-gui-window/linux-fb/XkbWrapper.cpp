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
#include "XkbWrapper.h"
#include "XkbKeysyms.h"
#include <wildcat/gui/window/Key.h>

namespace wildcat{
namespace gui {
namespace window {

struct xkb_rule_names_t 
{
    const char* rules;
    const char* model;
    const char* layout;
    const char* variant;
    const char* options;
};

Pt::System::Library         XkbWrapper::_lib;
xkb_context_new_t           XkbWrapper::_xkb_context_new = nullptr;
xkb_keymap_new_from_names   XkbWrapper::_xkb_keymap_new_from_names = nullptr;
xkb_state_new_t             XkbWrapper::_xkb_state_new = nullptr;
xkb_unref_t                 XkbWrapper::_xkb_context_unref = nullptr;
xkb_unref_t                 XkbWrapper::_xkb_state_unref = nullptr;
xkb_unref_t                 XkbWrapper::_xkb_keymap_unref = nullptr;
xkb_state_update_key_t      XkbWrapper::_xkb_state_update_key = nullptr;
xkb_state_key_get_one_sym_t XkbWrapper::_xkb_state_key_get_one_sym = nullptr;
xkb_state_key_get_utf8_t    XkbWrapper::_xkb_state_key_get_utf8 = nullptr;

XkbWrapper::XkbWrapper()
: _xkbCtx(nullptr)
, _xkbKeymap(nullptr)   
, _xkbState(nullptr)
{
    try
    {
        if (_xkb_context_new == nullptr)    
        {
            _lib.open("xkbcommon");
            
           _xkb_context_new            = (xkb_context_new_t)_lib.resolve("xkb_context_new");  
           _xkb_keymap_new_from_names  = (xkb_keymap_new_from_names)_lib.resolve("xkb_keymap_new_from_names");
           _xkb_state_new              = (xkb_state_new_t)_lib.resolve("xkb_state_new");
           _xkb_state_unref            = (xkb_unref_t)_lib.resolve("xkb_state_unref");
           _xkb_context_unref          = (xkb_unref_t)_lib.resolve("xkb_context_unref");
           _xkb_keymap_unref           = (xkb_unref_t)_lib.resolve("xkb_keymap_unref");
           _xkb_state_update_key       = (xkb_state_update_key_t)_lib.resolve("xkb_state_update_key");
           _xkb_state_update_key       = (xkb_state_update_key_t)_lib.resolve("xkb_state_update_key");
           _xkb_state_key_get_one_sym  = (xkb_state_key_get_one_sym_t)_lib.resolve("xkb_state_key_get_one_sym"); 
           _xkb_state_key_get_utf8    = (xkb_state_key_get_utf8_t)_lib.resolve("xkb_state_key_get_utf8");
        }
    }
    catch(const std::exception&)
    {
        _xkb_context_new = nullptr;
    }           
}   

XkbWrapper::~XkbWrapper()
{
    
}

void XkbWrapper::open()
{
    if(_xkb_context_new == nullptr)
        return; 

    _xkbCtx = _xkb_context_new(0);

    if (!_xkbCtx)
        return;

    struct xkb_rule_names_t names;

    std::memset(&names, 0, sizeof(names));

    // Try environment variables first
    const char* rulesEnv = std::getenv("XKB_DEFAULT_RULES");

    if (!rulesEnv)
        rulesEnv = std::getenv("XKB_RULES");

    if (rulesEnv && *rulesEnv)
        names.rules = rulesEnv;
    else
        names.rules = "evdev";

    const char* modelEnv = std::getenv("XKB_DEFAULT_MODEL");

    if (!modelEnv)
        modelEnv = std::getenv("XKB_MODEL");

    if (modelEnv && *modelEnv)
        names.model = modelEnv; else names.model = "pc105";

    const char* layoutEnv = std::getenv("XKB_DEFAULT_LAYOUT");

    if (!layoutEnv)
        layoutEnv = std::getenv("XKB_LAYOUT");

    if (layoutEnv && *layoutEnv)
        names.layout = layoutEnv;

    const char* variantEnv = std::getenv("XKB_DEFAULT_VARIANT");

    if (!variantEnv)
        variantEnv = std::getenv("XKB_VARIANT");

    if (variantEnv && *variantEnv)
        names.variant = variantEnv;

    const char* optionsEnv = std::getenv("XKB_DEFAULT_OPTIONS");

    if (!optionsEnv)
        optionsEnv = std::getenv("XKB_OPTIONS");

    if (optionsEnv && *optionsEnv)
        names.options = optionsEnv;


    std::string model, layout, variant, options;

    if (!names.layout)
    {
        FILE* f = std::fopen("/etc/default/keyboard", "r");

        if (f)
        {
            char line[256];

            while (std::fgets(line, sizeof(line), f))
            {
                char* eq = std::strchr(line, '=');
                if (!eq)
                    continue;

                *eq = '\0';
                char* key = line;
                char* val = eq + 1;

                // trim newline and quotes
                size_t l = std::strlen(val);

                while (l && (val[l - 1] == '\n' || val[l - 1] == '\r' || val[l - 1] == '"'))
                {
                    val[--l] = '\0';
                }

                if (*val == '"')
                    ++val;

                if (std::strcmp(key, "XKBLAYOUT") == 0)
                    layout = val;
                else if (std::strcmp(key, "XKBMODEL") == 0)
                    model = val;
                else if (std::strcmp(key, "XKBVARIANT") == 0)
                    variant = val;
                else if (std::strcmp(key, "XKBOPTIONS") == 0)
                    options = val;
            }

            if (!layout.empty())
                names.layout = names.layout ? names.layout : strdup(layout.c_str());

            if (!model.empty() && !names.model)
                names.model = strdup(model.c_str());

            if (!variant.empty() && !names.variant)
                names.variant = strdup(variant.c_str());

            if (!options.empty() && !names.options)
                names.options = strdup(options.c_str());

            std::fclose(f);
        }
    }    

    _xkbKeymap = _xkb_keymap_new_from_names(_xkbCtx, &names, 0);

    if (_xkbKeymap)
        _xkbState = _xkb_state_new(_xkbKeymap);
}

void XkbWrapper::updateState(uint32_t code, int value)
{
    if (!_xkbState)
        return;
    
    int xkb_key = static_cast<int>(code) + 8;
    _xkb_state_update_key(_xkbState, xkb_key, value != 0);    
}

uint32_t XkbWrapper::translate(unsigned code, Pt::Char& ch)
{
    if(_xkbState == nullptr)
        return Key::NoKey;

    int xkb_key = static_cast<int>(code) + 8;

    uint32_t ks = _xkb_state_key_get_one_sym(_xkbState, xkb_key);

    if (ks != WILDCAT_XKB_KEY_NoSymbol)
    {
        switch (ks)
        {
            case WILDCAT_XKB_KEY_BackSpace: return Key::Backspace;
            case WILDCAT_XKB_KEY_Tab:       return Key::Tab;
            case WILDCAT_XKB_KEY_Return:    return Key::Return;
            case WILDCAT_XKB_KEY_Escape:    return Key::Escape;
            case WILDCAT_XKB_KEY_space:     ch = ' '; return Key::Space;

            case WILDCAT_XKB_KEY_Left:  return Key::ArrowLeft;
            case WILDCAT_XKB_KEY_Right: return Key::ArrowRight;
            case WILDCAT_XKB_KEY_Up:    return Key::ArrowUp;
            case WILDCAT_XKB_KEY_Down:  return Key::ArrowDown;

            case WILDCAT_XKB_KEY_Home:     return Key::Home;
            case WILDCAT_XKB_KEY_End:      return Key::End;
            case WILDCAT_XKB_KEY_Page_Up:  return Key::PageUp;
            case WILDCAT_XKB_KEY_Page_Down:return Key::PageDown;
            case WILDCAT_XKB_KEY_Insert:   return Key::Insert;
            case WILDCAT_XKB_KEY_Delete:   return Key::Delete;

            case WILDCAT_XKB_KEY_F1:  return Key::F1;
            case WILDCAT_XKB_KEY_F2:  return Key::F2;
            case WILDCAT_XKB_KEY_F3:  return Key::F3;
            case WILDCAT_XKB_KEY_F4:  return Key::F4;
            case WILDCAT_XKB_KEY_F5:  return Key::F5;
            case WILDCAT_XKB_KEY_F6:  return Key::F6;
            case WILDCAT_XKB_KEY_F7:  return Key::F7;
            case WILDCAT_XKB_KEY_F8:  return Key::F8;
            case WILDCAT_XKB_KEY_F9:  return Key::F9;
            case WILDCAT_XKB_KEY_F10: return Key::F10;
            case WILDCAT_XKB_KEY_F11: return Key::F11;
            case WILDCAT_XKB_KEY_F12: return Key::F12;

            case WILDCAT_XKB_KEY_KP_0: return Key::NumPad0;
            case WILDCAT_XKB_KEY_KP_1: return Key::NumPad1;
            case WILDCAT_XKB_KEY_KP_2: return Key::NumPad2;
            case WILDCAT_XKB_KEY_KP_3: return Key::NumPad3;
            case WILDCAT_XKB_KEY_KP_4: return Key::NumPad4;
            case WILDCAT_XKB_KEY_KP_5: return Key::NumPad5;
            case WILDCAT_XKB_KEY_KP_6: return Key::NumPad6;
            case WILDCAT_XKB_KEY_KP_7: return Key::NumPad7;
            case WILDCAT_XKB_KEY_KP_8: return Key::NumPad8;
            case WILDCAT_XKB_KEY_KP_9: return Key::NumPad9;
            case WILDCAT_XKB_KEY_KP_Add:      return Key::Add;
            case WILDCAT_XKB_KEY_KP_Subtract: return Key::Subtract;
            case WILDCAT_XKB_KEY_KP_Multiply: return Key::Multiply;
            case WILDCAT_XKB_KEY_KP_Divide:   return Key::Divide;
            case WILDCAT_XKB_KEY_KP_Decimal:  return Key::Decimal;

            default: break;
        }
    }

    // Extract layout-aware UTF-8 character (handles ä, ö, ü, ß, etc.)
    char utf8[32] = { 0 };
    int got = _xkb_state_key_get_utf8(_xkbState, xkb_key, utf8, sizeof(utf8));

    if (got > 0 && utf8[0] != '\0')
    {
        Pt::String s = Pt::Utf8Codec::decode(utf8, got);

        if (!s.empty())
        {
            Pt::Char cp = s[0];

            // Accept all printable characters (>= 32), including non-ASCII
            if (cp >= 32)
            {
                ch = cp;

                // For ASCII letters, return uppercase as key code
                if (cp >= 'a' && cp <= 'z')
                    return static_cast<uint32_t>(Pt::toupper(cp).value());

                // For everything else (umlauts, digits, punctuation, Unicode)
                // return the codepoint directly as key code
                return static_cast<uint32_t>(cp);
            }
        }
    }

    return Key::NoKey;
}

void XkbWrapper::close()
{
    if (_xkbState)
        _xkb_state_unref(_xkbState);

    if (_xkbKeymap)
        _xkb_keymap_unref(_xkbKeymap);

    if (_xkbCtx)
        _xkb_context_unref(_xkbCtx);

    _xkbState = nullptr;
    _xkbKeymap = nullptr;
    _xkbCtx = nullptr;
}

}}}
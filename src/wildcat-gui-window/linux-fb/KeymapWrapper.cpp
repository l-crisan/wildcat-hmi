#include "KeymapWrapper.h"  
#include "XkbKeysyms.h"
#include <wildcat/gui/window/Key.h>

namespace wildcat {
namespace gui {
namespace window {

Pt::System::Library KeymapWrapper::_lib;
lk_init_t           KeymapWrapper::_lk_init     = nullptr;
lk_kernel_keymap_t  KeymapWrapper::_lk_kernel_keymap = nullptr;
lk_get_key_t        KeymapWrapper::_lk_get_key  = nullptr;
lk_free_t           KeymapWrapper::_lk_free     = nullptr;

KeymapWrapper::KeymapWrapper()
: _context(0)
{
    try
    {
        if (_lk_init == nullptr)    
        {
           _lib.open("keymap");
            
           _lk_init          = (lk_init_t)_lib.resolve("lk_init");  
           _lk_kernel_keymap = (lk_kernel_keymap_t)_lib.resolve("lk_kernel_keymap");
           _lk_get_key       = (lk_get_key_t)_lib.resolve("lk_get_key");
           _lk_free          = (lk_free_t)_lib.resolve("lk_free");
         
        }
    }
    catch(const std::exception& ex)
    {
        _lk_init = nullptr;
    }           
}   

KeymapWrapper::~KeymapWrapper()
{
    close();
}


void KeymapWrapper::open()
{
    if(_lk_init == nullptr)
        return;

    _context = _lk_init();

    if(_context == nullptr)
       return;

    _lk_kernel_keymap(_context, 0);
}

uint32_t KeymapWrapper::translate(unsigned code, Pt::Char& ch)
{
    if (_lk_get_key == nullptr)
        return Key::NoKey;

    if (_context == nullptr)
        return Key::NoKey;

    int keysym = _lk_get_key(_context, 0, code);

    if (keysym == 0)
        return Key::NoKey;

    ch = 0;

    switch (keysym)
    {
        case WILDCAT_XKB_KEY_BackSpace: return Key::Backspace;
        case WILDCAT_XKB_KEY_Tab:       return Key::Tab;
        case WILDCAT_XKB_KEY_Return:    return Key::Return;
        case WILDCAT_XKB_KEY_Escape:    return Key::Escape;
        case WILDCAT_XKB_KEY_space:     ch = ' '; return Key::Space;

        case WILDCAT_XKB_KEY_Left:      return Key::ArrowLeft;
        case WILDCAT_XKB_KEY_Right:     return Key::ArrowRight;
        case WILDCAT_XKB_KEY_Up:        return Key::ArrowUp;
        case WILDCAT_XKB_KEY_Down:      return Key::ArrowDown;

        case WILDCAT_XKB_KEY_Home:      return Key::Home;
        case WILDCAT_XKB_KEY_End:       return Key::End;
        case WILDCAT_XKB_KEY_Page_Up:   return Key::PageUp;
        case WILDCAT_XKB_KEY_Page_Down: return Key::PageDown;
        case WILDCAT_XKB_KEY_Insert:    return Key::Insert;
        case WILDCAT_XKB_KEY_Delete:    return Key::Delete;

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
        case WILDCAT_XKB_KEY_KP_Separator: return Key::Separator;
        case WILDCAT_XKB_KEY_KP_Enter:    return Key::Return;

        case WILDCAT_XKB_KEY_Num_Lock:    return Key::NumLock;
        case WILDCAT_XKB_KEY_Caps_Lock:   return Key::CapsLock;
        case WILDCAT_XKB_KEY_Scroll_Lock: return Key::ScrollLock;
        case WILDCAT_XKB_KEY_Pause:       return Key::Pause;
        case WILDCAT_XKB_KEY_Print:       return Key::PrintScreen;
        case WILDCAT_XKB_KEY_Menu:        return Key::AppsMenu;

        default: break;
    }

    uint32_t ks = static_cast<uint32_t>(keysym);

    // Latin-1 printable characters (keysym == Unicode for 0x20..0xFF)
    if (ks >= 0x0020 && ks <= 0x00FF)
    {
        ch = static_cast<Pt::Char>(ks);

        if (ks >= 'a' && ks <= 'z')
            return static_cast<uint32_t>(Pt::toupper(ks).value());

        if (ks >= 'A' && ks <= 'Z')
            return ks;

        return ks;
    }

    // Unicode keysyms (0x01000100..0x0110FFFF) -> Unicode codepoint
    if (ks >= 0x01000100 && ks <= 0x0110FFFF)
    {
        uint32_t cp = ks - 0x01000000;
        ch = static_cast<Pt::Char>(cp);

        if (cp >= 'a' && cp <= 'z')
            return static_cast<uint32_t>(Pt::toupper(cp).value());

        return cp;
    }

    return Key::NoKey;
}

void KeymapWrapper::close()
{
    if (_context != nullptr && _lk_free != nullptr)
    {
        _lk_free(_context);
        _context = nullptr;
    }
}

}}}
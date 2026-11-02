#include <wildcat/gui/window/Key.h>
#include <Pt/Types.h>

namespace wildcat {
namespace gui {
namespace window {

Pt::uint32_t toKeycode(const char* domCode)
{
    static const std::map<std::string, Pt::uint32_t> codeMap = {
      { "Backspace",    Key::Backspace },
      { "Tab",          Key::Tab       },
      { "Enter",        Key::Return    },
      { "Escape",       Key::Escape    },
      { "Space",        Key::Space     },

      { "Digit0",  Key::D0 },
      { "Digit1",  Key::D1 },
      { "Digit2",  Key::D2 },
      { "Digit3",  Key::D3 },
      { "Digit4",  Key::D4 },
      { "Digit5",  Key::D5 },
      { "Digit6",  Key::D6 },
      { "Digit7",  Key::D7 },
      { "Digit8",  Key::D8 },
      { "Digit9",  Key::D9 },

      { "KeyA",  Key::A },
      { "KeyB",  Key::B },
      { "KeyC",  Key::C },
      { "KeyD",  Key::D },
      { "KeyE",  Key::E },
      { "KeyF",  Key::F },
      { "KeyG",  Key::G },
      { "KeyH",  Key::H },
      { "KeyI",  Key::I },
      { "KeyJ",  Key::J },
      { "KeyK",  Key::K },
      { "KeyL",  Key::L },
      { "KeyM",  Key::M },
      { "KeyN",  Key::N },
      { "KeyO",  Key::O },
      { "KeyP",  Key::P },
      { "KeyQ",  Key::Q },
      { "KeyR",  Key::R },
      { "KeyS",  Key::S },
      { "KeyT",  Key::T },
      { "KeyU",  Key::U },
      { "KeyV",  Key::V },
      { "KeyW",  Key::W },
      { "KeyX",  Key::X },
      { "KeyY",  Key::Y },
      { "KeyZ",  Key::Z },

      { "ShiftLeft",     Key::Shift },
      { "ShiftRight",    Key::Shift },
      { "ControlLeft",   Key::Control },
      { "ControlRight",  Key::Control },
      { "AltLeft",       Key::Alt },
      { "AltRight",      Key::Alt },
      { "MetaLeft",      Key::Meta },
      { "MetaRight",     Key::Meta },

      { "ArrowLeft",   Key::ArrowLeft  },
      { "ArrowRight",  Key::ArrowRight },
      { "ArrowUp",     Key::ArrowUp    },
      { "ArrowDown",   Key::ArrowDown  },

      { "NumLock",  Key::NumLock },
      { "Numpad0",  Key::NumPad0 },
      { "Numpad1",  Key::NumPad1 },
      { "Numpad2",  Key::NumPad2 },
      { "Numpad3",  Key::NumPad3 },
      { "Numpad4",  Key::NumPad4 },
      { "Numpad5",  Key::NumPad5 },
      { "Numpad6",  Key::NumPad6 },
      { "Numpad7",  Key::NumPad7 },
      { "Numpad8",  Key::NumPad8 },
      { "Numpad9",  Key::NumPad9 },

      { "NumpadMultiply",  Key::Multiply  },
      { "NumpadAdd",       Key::Add       },
      { "NumpadDivide",    Key::Divide    },
      { "NumpadSubtract",  Key::Subtract  },
      { "NumpadDecimal",   Key::Decimal   },
      { "NumpadEnter",     Key::Return    },

      { "F1",   Key::F1  },
      { "F2",   Key::F2  },
      { "F3",   Key::F3  },
      { "F4",   Key::F4  },
      { "F5",   Key::F5  },
      { "F6",   Key::F6  },
      { "F7",   Key::F7  },
      { "F8",   Key::F8  },
      { "F9",   Key::F9  },
      { "F10",  Key::F10 },
      { "F11",  Key::F11 },
      { "F12",  Key::F12 },
      { "F13",  Key::F13 },
      { "F14",  Key::F14 },
      { "F15",  Key::F15 },
      { "F16",  Key::F16 },
      { "F17",  Key::F17 },
      { "F18",  Key::F18 },
      { "F19",  Key::F19 },
      { "F20",  Key::F20 },
      { "F21",  Key::F21 },
      { "F22",  Key::F22 },
      { "F23",  Key::F23 },
      { "F24",  Key::F24 },

      { "Insert",       Key::Insert      },
      { "Delete",       Key::Delete      },
      { "Home",         Key::Home        },
      { "End",          Key::End         },
      { "PageUp",       Key::PageUp      },
      { "PageDown",     Key::PageDown    },
      { "CapsLock",     Key::CapsLock    },
      { "PrintScreen",  Key::PrintScreen },
      { "ScrollLock",   Key::ScrollLock  },
      { "Pause",        Key::Pause       },
      { "Help",         Key::Help        },
      { "ContextMenu",  Key::AppsMenu    },
    };

    std::map<std::string, Pt::uint32_t>::const_iterator it = codeMap.find(domCode);
    if (it == codeMap.end())
        return Key::NoKey;

    return it->second;
}


}}}

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
#include <wildcat/gui/window/Key.h>
#include <Pt/Types.h>

namespace wildcat {

namespace gui {
namespace window {

static const Pt::uint32_t keyMap[] =
{
    wildcat::gui::window::Key::Unknown,           //   0   0x00
    wildcat::gui::window::Key::Unknown,           //   1   0x01   VK_LBUTTON          
    wildcat::gui::window::Key::Unknown,           //   2   0x02   VK_RBUTTON          
    wildcat::gui::window::Key::Break,             //   3   0x03   VK_CANCEL           
    wildcat::gui::window::Key::Unknown,           //   4   0x04   VK_MBUTTON          
    wildcat::gui::window::Key::Unknown,           //   5   0x05   VK_XBUTTON1         
    wildcat::gui::window::Key::Unknown,           //   6   0x06   VK_XBUTTON2         
    wildcat::gui::window::Key::Unknown,           //   7   0x07
    wildcat::gui::window::Key::Backspace,         //   8   0x08   VK_BACK             
    wildcat::gui::window::Key::Tab,               //   9   0x09   VK_TAB              
    wildcat::gui::window::Key::Unknown,           //  10   0x0A
    wildcat::gui::window::Key::Unknown,           //  11   0x0B
    wildcat::gui::window::Key::Clear,             //  12   0x0C   VK_CLEAR            
    wildcat::gui::window::Key::Return,            //  13   0x0D   VK_RETURN           
    wildcat::gui::window::Key::Unknown,           //  14   0x0E
    wildcat::gui::window::Key::Unknown,           //  15   0x0F
    wildcat::gui::window::Key::Shift,             //  16   0x10   VK_SHIFT            
    wildcat::gui::window::Key::Control,           //  17   0x11   VK_CONTROL          
    wildcat::gui::window::Key::Alt,               //  18   0x12   VK_MENU             
    wildcat::gui::window::Key::Pause,             //  19   0x13   VK_PAUSE            
    wildcat::gui::window::Key::CapsLock,          //  20   0x14   VK_CAPITAL          
    wildcat::gui::window::Key::Unknown,           //  21   0x15   VK_KANA VK_HANGUL 
    wildcat::gui::window::Key::Unknown,           //  22   0x16
    wildcat::gui::window::Key::Unknown,           //  23   0x17   VK_JUNJA            
    wildcat::gui::window::Key::Unknown,           //  24   0x18   VK_FINAL            
    wildcat::gui::window::Key::Unknown,           //  25   0x19   VK_HANJA VK_KANJI 
    wildcat::gui::window::Key::Unknown,           //  26   0x1A
    wildcat::gui::window::Key::Escape,            //  27   0x1B   VK_ESCAPE           
    wildcat::gui::window::Key::Unknown,           //  28   0x1C   VK_CONVERT          
    wildcat::gui::window::Key::Unknown,           //  29   0x1D   VK_NONCONVERT       
    wildcat::gui::window::Key::Unknown,           //  30   0x1E   VK_ACCEPT           
    wildcat::gui::window::Key::ModeChange,        //  31   0x1F   VK_MODECHANGE       
    wildcat::gui::window::Key::Space,             //  32   0x20   VK_SPACE            
    wildcat::gui::window::Key::PageUp,            //  33   0x21   VK_PRIOR            
    wildcat::gui::window::Key::PageDown,          //  34   0x22   VK_NEXT             
    wildcat::gui::window::Key::End,               //  35   0x23   VK_END              
    wildcat::gui::window::Key::Home,              //  36   0x24   VK_HOME             
    wildcat::gui::window::Key::ArrowLeft,         //  37   0x25   VK_LEFT             
    wildcat::gui::window::Key::ArrowUp,           //  38   0x26   VK_UP               
    wildcat::gui::window::Key::ArrowRight,        //  39   0x27   VK_RIGHT            
    wildcat::gui::window::Key::ArrowDown,         //  40   0x28   VK_DOWN             
    wildcat::gui::window::Key::Select,            //  41   0x29   VK_SELECT           
    wildcat::gui::window::Key::Print,             //  42   0x2A   VK_PRINT            
    wildcat::gui::window::Key::Execute,           //  43   0x2B   VK_EXECUTE          
    wildcat::gui::window::Key::PrintScreen,       //  44   0x2C   VK_SNAPSHOT         
    wildcat::gui::window::Key::Insert,            //  45   0x2D   VK_INSERT           
    wildcat::gui::window::Key::Delete,            //  46   0x2E   VK_DELETE           
    wildcat::gui::window::Key::Help,              //  47   0x2F   VK_HELP             
    0,                               //  48   0x30   VK_0              
    0,                               //  49   0x31   VK_1              
    0,                               //  50   0x32   VK_2              
    0,                               //  51   0x33   VK_3              
    0,                               //  52   0x34   VK_4              
    0,                               //  53   0x35   VK_5              
    0,                               //  54   0x36   VK_6              
    0,                               //  55   0x37   VK_7              
    0,                               //  56   0x38   VK_8              
    0,                               //  57   0x39   VK_9              
    wildcat::gui::window::Key::Unknown,           //  58   0x3A
    wildcat::gui::window::Key::Unknown,           //  59   0x3B
    wildcat::gui::window::Key::Unknown,           //  60   0x3C
    wildcat::gui::window::Key::Unknown,           //  61   0x3D
    wildcat::gui::window::Key::Unknown,           //  62   0x3E
    wildcat::gui::window::Key::Unknown,           //  63   0x3F
    wildcat::gui::window::Key::Unknown,           //  64   0x40
    0,                               //  65   0x41   VK_A              
    0,                               //  66   0x42   VK_B              
    0,                               //  67   0x43   VK_C              
    0,                               //  68   0x44   VK_D              
    0,                               //  69   0x45   VK_E              
    0,                               //  70   0x46   VK_F              
    0,                               //  71   0x47   VK_G              
    0,                               //  72   0x48   VK_H              
    0,                               //  73   0x49   VK_I              
    0,                               //  74   0x4A   VK_J              
    0,                               //  75   0x4B   VK_K              
    0,                               //  76   0x4C   VK_L              
    0,                               //  77   0x4D   VK_M              
    0,                               //  78   0x4E   VK_N              
    0,                               //  79   0x4F   VK_O              
    0,                               //  80   0x50   VK_P              
    0,                               //  81   0x51   VK_Q              
    0,                               //  82   0x52   VK_R              
    0,                               //  83   0x53   VK_S              
    0,                               //  84   0x54   VK_T              
    0,                               //  85   0x55   VK_U              
    0,                               //  86   0x56   VK_V              
    0,                               //  87   0x57   VK_W              
    0,                               //  88   0x58   VK_X              
    0,                               //  89   0x59   VK_Y              
    0,                               //  90   0x5A   VK_Z              
    wildcat::gui::window::Key::Meta,              //  91   0x5B   VK_LWIN             
    wildcat::gui::window::Key::Meta,              //  92   0x5C   VK_RWIN             
    wildcat::gui::window::Key::AppsMenu,          //  93   0x5D   VK_APPS             
    wildcat::gui::window::Key::Unknown,           //  94   0x5E
    wildcat::gui::window::Key::Sleep,             //  95   0x5F   VK_SLEEP
    wildcat::gui::window::Key::NumPad0,           //  96   0x60   VK_NUMPAD0          
    wildcat::gui::window::Key::NumPad1,           //  97   0x61   VK_NUMPAD1          
    wildcat::gui::window::Key::NumPad2,           //  98   0x62   VK_NUMPAD2          
    wildcat::gui::window::Key::NumPad3,           //  99   0x63   VK_NUMPAD3          
    wildcat::gui::window::Key::NumPad4,           // 100   0x64   VK_NUMPAD4          
    wildcat::gui::window::Key::NumPad5,           // 101   0x65   VK_NUMPAD5          
    wildcat::gui::window::Key::NumPad6,           // 102   0x66   VK_NUMPAD6          
    wildcat::gui::window::Key::NumPad7,           // 103   0x67   VK_NUMPAD7          
    wildcat::gui::window::Key::NumPad8,           // 104   0x68   VK_NUMPAD8          
    wildcat::gui::window::Key::NumPad9,           // 105   0x69   VK_NUMPAD9          
    wildcat::gui::window::Key::Multiply,          // 106   0x6A   VK_MULTIPLY         
    wildcat::gui::window::Key::Add,               // 107   0x6B   VK_ADD              
    wildcat::gui::window::Key::Separator,         // 108   0x6C   VK_SEPARATOR        
    wildcat::gui::window::Key::Subtract,          // 109   0x6D   VK_SUBTRACT         
    wildcat::gui::window::Key::Decimal,           // 110   0x6E   VK_DECIMAL          
    wildcat::gui::window::Key::Divide,            // 111   0x6F   VK_DIVIDE           
    wildcat::gui::window::Key::F1,                // 112   0x70   VK_F1               
    wildcat::gui::window::Key::F2,                // 113   0x71   VK_F2               
    wildcat::gui::window::Key::F3,                // 114   0x72   VK_F3               
    wildcat::gui::window::Key::F4,                // 115   0x73   VK_F4               
    wildcat::gui::window::Key::F5,                // 116   0x74   VK_F5               
    wildcat::gui::window::Key::F6,                // 117   0x75   VK_F6               
    wildcat::gui::window::Key::F7,                // 118   0x76   VK_F7               
    wildcat::gui::window::Key::F8,                // 119   0x77   VK_F8               
    wildcat::gui::window::Key::F9,                // 120   0x78   VK_F9               
    wildcat::gui::window::Key::F10,               // 121   0x79   VK_F10              
    wildcat::gui::window::Key::F11,               // 122   0x7A   VK_F11              
    wildcat::gui::window::Key::F12,               // 123   0x7B   VK_F12              
    wildcat::gui::window::Key::F13,               // 124   0x7C   VK_F13              
    wildcat::gui::window::Key::F14,               // 125   0x7D   VK_F14              
    wildcat::gui::window::Key::F15,               // 126   0x7E   VK_F15              
    wildcat::gui::window::Key::F16,               // 127   0x7F   VK_F16              
    wildcat::gui::window::Key::F17,               // 128   0x80   VK_F17              
    wildcat::gui::window::Key::F18,               // 129   0x81   VK_F18              
    wildcat::gui::window::Key::F19,               // 130   0x82   VK_F19              
    wildcat::gui::window::Key::F20,               // 131   0x83   VK_F20              
    wildcat::gui::window::Key::F21,               // 132   0x84   VK_F21              
    wildcat::gui::window::Key::F22,               // 133   0x85   VK_F22              
    wildcat::gui::window::Key::F23,               // 134   0x86   VK_F23              
    wildcat::gui::window::Key::F24,               // 135   0x87   VK_F24              
    wildcat::gui::window::Key::Unknown,           // 136   0x88
    wildcat::gui::window::Key::Unknown,           // 137   0x89
    wildcat::gui::window::Key::Unknown,           // 138   0x8A
    wildcat::gui::window::Key::Unknown,           // 139   0x8B
    wildcat::gui::window::Key::Unknown,           // 140   0x8C
    wildcat::gui::window::Key::Unknown,           // 141   0x8D
    wildcat::gui::window::Key::Unknown,           // 142   0x8E
    wildcat::gui::window::Key::Unknown,           // 143   0x8F
    wildcat::gui::window::Key::NumLock,           // 144   0x90   VK_NUMLOCK          
    wildcat::gui::window::Key::ScrollLock,        // 145   0x91   VK_SCROLL           
    0,                               // 146   0x92   VK_OEM_FJ_JISHO     
    0,                               // 147   0x93   VK_OEM_FJ_MASSHOU   
    0,                               // 148   0x94   VK_OEM_FJ_TOUROKU   
    0,                               // 149   0x95   VK_OEM_FJ_LOYA  
    0,                               // 150   0x96   VK_OEM_FJ_ROYA  
    wildcat::gui::window::Key::Unknown,           // 151   0x97
    wildcat::gui::window::Key::Unknown,           // 152   0x98
    wildcat::gui::window::Key::Unknown,           // 153   0x99
    wildcat::gui::window::Key::Unknown,           // 154   0x9A
    wildcat::gui::window::Key::Unknown,           // 155   0x9B
    wildcat::gui::window::Key::Unknown,           // 156   0x9C
    wildcat::gui::window::Key::Unknown,           // 157   0x9D
    wildcat::gui::window::Key::Unknown,           // 158   0x9E
    wildcat::gui::window::Key::Unknown,           // 159   0x9F
    wildcat::gui::window::Key::Shift,             // 160   0xA0   VK_LSHIFT           
    wildcat::gui::window::Key::Shift,             // 161   0xA1   VK_RSHIFT           
    wildcat::gui::window::Key::Control,           // 162   0xA2   VK_LCONTROL         
    wildcat::gui::window::Key::Control,           // 163   0xA3   VK_RCONTROL         
    wildcat::gui::window::Key::Alt,               // 164   0xA4   VK_LMENU            
    wildcat::gui::window::Key::Alt,               // 165   0xA5   VK_RMENU            
    wildcat::gui::window::Key::BrowserBack,       // 166   0xA6   VK_BROWSER_BACK     
    wildcat::gui::window::Key::BrowserForward,    // 167   0xA7   VK_BROWSER_FORWARD  
    wildcat::gui::window::Key::BrowserRefresh,    // 168   0xA8   VK_BROWSER_REFRESH  
    wildcat::gui::window::Key::BrowserStop,       // 169   0xA9   VK_BROWSER_STOP     
    wildcat::gui::window::Key::BrowserSearch,     // 170   0xAA   VK_BROWSER_SEARCH   
    wildcat::gui::window::Key::BrowserFavorites,  // 171   0xAB   VK_BROWSER_FAVORITES
    wildcat::gui::window::Key::BrowserHome,       // 172   0xAC   VK_BROWSER_HOME     
    wildcat::gui::window::Key::VolumeMute,        // 173   0xAD   VK_VOLUME_MUTE      
    wildcat::gui::window::Key::VolumeDown,        // 174   0xAE   VK_VOLUME_DOWN      
    wildcat::gui::window::Key::VolumeUp,          // 175   0xAF   VK_VOLUME_UP        
    wildcat::gui::window::Key::MediaNext,         // 176   0xB0   VK_MEDIA_NEXT_TRACK 
    wildcat::gui::window::Key::MediaPrev,         // 177   0xB1   VK_MEDIA_PREV_TRACK 
    wildcat::gui::window::Key::MediaStop,         // 178   0xB2   VK_MEDIA_STOP       
    wildcat::gui::window::Key::MediaPlay,         // 179   0xB3   VK_MEDIA_PLAY_PAUSE 
    wildcat::gui::window::Key::LaunchMail,        // 180   0xB4   VK_LAUNCH_MAIL      
    wildcat::gui::window::Key::LaunchMedia,       // 181   0xB5   VK_LAUNCH_MEDIA_SELECT
    wildcat::gui::window::Key::LaunchApp1,        // 182   0xB6   VK_LAUNCH_APP1      
    wildcat::gui::window::Key::LaunchApp2,        // 183   0xB7   VK_LAUNCH_APP2      
    wildcat::gui::window::Key::Unknown,           // 184   0xB8
    wildcat::gui::window::Key::Unknown,           // 185   0xB9
    0,                               // 186   0xBA   VK_OEM_1            
    0,                               // 187   0xBB   VK_OEM_PLUS         
    0,                               // 188   0xBC   VK_OEM_COMMA        
    0,                               // 189   0xBD   VK_OEM_MINUS        
    0,                               // 190   0xBE   VK_OEM_PERIOD       
    0,                               // 191   0xBF   VK_OEM_2            
    0,                               // 192   0xC0   VK_OEM_3            
    wildcat::gui::window::Key::Unknown,           // 193   0xC1
    wildcat::gui::window::Key::Unknown,           // 194   0xC2
    wildcat::gui::window::Key::Unknown,           // 195   0xC3
    wildcat::gui::window::Key::Unknown,           // 196   0xC4
    wildcat::gui::window::Key::Unknown,           // 197   0xC5
    wildcat::gui::window::Key::Unknown,           // 198   0xC6
    wildcat::gui::window::Key::Unknown,           // 199   0xC7
    wildcat::gui::window::Key::Unknown,           // 200   0xC8
    wildcat::gui::window::Key::Unknown,           // 201   0xC9
    wildcat::gui::window::Key::Unknown,           // 202   0xCA
    wildcat::gui::window::Key::Unknown,           // 203   0xCB
    wildcat::gui::window::Key::Unknown,           // 204   0xCC
    wildcat::gui::window::Key::Unknown,           // 205   0xCD
    wildcat::gui::window::Key::Unknown,           // 206   0xCE
    wildcat::gui::window::Key::Unknown,           // 207   0xCF
    wildcat::gui::window::Key::Unknown,           // 208   0xD0
    wildcat::gui::window::Key::Unknown,           // 209   0xD1
    wildcat::gui::window::Key::Unknown,           // 210   0xD2
    wildcat::gui::window::Key::Unknown,           // 211   0xD3
    wildcat::gui::window::Key::Unknown,           // 212   0xD4
    wildcat::gui::window::Key::Unknown,           // 213   0xD5
    wildcat::gui::window::Key::Unknown,           // 214   0xD6
    wildcat::gui::window::Key::Unknown,           // 215   0xD7
    wildcat::gui::window::Key::Unknown,           // 216   0xD8
    wildcat::gui::window::Key::Unknown,           // 217   0xD9
    wildcat::gui::window::Key::Unknown,           // 218   0xDA
    0,                               // 219   0xDB   VK_OEM_4            
    0,                               // 220   0xDC   VK_OEM_5            
    0,                               // 221   0xDD   VK_OEM_6            
    0,                               // 222   0xDE   VK_OEM_7            
    0,                               // 223   0xDF   VK_OEM_8
    wildcat::gui::window::Key::Unknown,           // 224   0xE0
    wildcat::gui::window::Key::Unknown,           // 225   0xE1   VK_OEM_AX           
    0,                               // 226   0xE2   VK_OEM_102 ("<>" key)       
    wildcat::gui::window::Key::Unknown,           // 227   0xE3   VK_ICO_HELP         
    wildcat::gui::window::Key::Unknown,           // 228   0xE4   VK_ICO_00           
    wildcat::gui::window::Key::Unknown,           // 229   0xE5   VK_PROCESSKEY       
    wildcat::gui::window::Key::Unknown,           // 230   0xE6   VK_ICO_CLEAR        
    wildcat::gui::window::Key::Unknown,           // 231   0xE7   VK_PACKET           
    wildcat::gui::window::Key::Unknown,           // 232   0xE8
    wildcat::gui::window::Key::Unknown,           // 233   0xE9   VK_OEM_RESET
    wildcat::gui::window::Key::Unknown,           // 234   0xEA   VK_OEM_JUMP
    wildcat::gui::window::Key::Unknown,           // 235   0xEB   VK_OEM_PA1
    wildcat::gui::window::Key::Unknown,           // 236   0xEC   VK_OEM_PA2
    wildcat::gui::window::Key::Unknown,           // 237   0xED   VK_OEM_PA3
    wildcat::gui::window::Key::Unknown,           // 238   0xEE   VK_OEM_WSCTRL
    wildcat::gui::window::Key::Unknown,           // 239   0xEF   VK_OEM_CUSEL
    wildcat::gui::window::Key::Unknown,           // 240   0xF0   VK_OEM_ATTN
    wildcat::gui::window::Key::Unknown,           // 241   0xF1   VK_OEM_FINISH
    wildcat::gui::window::Key::Unknown,           // 242   0xF2   VK_OEM_COPY
    wildcat::gui::window::Key::Unknown,           // 243   0xF3   VK_OEM_AUTO
    wildcat::gui::window::Key::Unknown,           // 244   0xF4   VK_OEM_ENLW
    wildcat::gui::window::Key::Unknown,           // 245   0xF5   VK_OEM_BACKTAB
    wildcat::gui::window::Key::Unknown,           // 246   0xF6   VK_ATTN             
    wildcat::gui::window::Key::Unknown,           // 247   0xF7   VK_CRSEL            
    wildcat::gui::window::Key::Unknown,           // 248   0xF8   VK_EXSEL            
    wildcat::gui::window::Key::Unknown,           // 249   0xF9   VK_EREOF            
    wildcat::gui::window::Key::Play,              // 250   0xFA   VK_PLAY             
    wildcat::gui::window::Key::Zoom,              // 251   0xFB   VK_ZOOM             
    wildcat::gui::window::Key::Unknown,           // 252   0xFC   VK_NONAME           
    wildcat::gui::window::Key::Unknown,           // 253   0xFD   VK_PA1              
    wildcat::gui::window::Key::Clear,             // 254   0xFE   VK_OEM_CLEAR        
    wildcat::gui::window::Key::Unknown            // 255   0xFF
};

const std::size_t keyMapSize = sizeof(keyMap) / sizeof(Pt::uint32_t);

}}}

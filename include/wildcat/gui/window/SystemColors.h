/*
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
#ifndef WILDCAT_GUI_WINDOW_SYSTEMCOLORS_H
#define WILDCAT_GUI_WINDOW_SYSTEMCOLORS_H


#include <wildcat/gui/gfx/Color.h>
#include <wildcat/gui/window/Api.h>

namespace wildcat{
namespace gui {
namespace window {

class WILDCAT_GUI_WINDOW_API SystemColors
{
    public:
        enum class ColorType
        {
            Desktop = 0,
            Workingspace,

            Window,
            WindowTitleText,
            WindowTitleBar,
            WindowTitleBarActive,
            WindowTitleBarDisabled,

            WindowBorder,
            WindowFrame,
            WindowBorderActive,
            WindowBorderDisabled,

            //Control
            Control,
            ControlDark,
            ControlDarkDark,
            ControlLight,
            ControlLightLight,
            ControlDisabled,
            ControlHighlight,

            //Frame
            ControlFrame,
            ControlFrameDisabled,

            //text
            ControlText,
            ControlTextDisabled,
            ControlTextHighlight,

            Info,
            InfoText,

            TextBackground,
            Text,
            TextHighlight,
            TextDisabled,
            Count
        };

        static std::string toString(int index);

    public:
        SystemColors();

        virtual ~SystemColors();
        
        const gfx::Color& get(int index) const
        {
            return _colors[index];
        }

        void set(int index, const gfx::Color& color)
        {
            _colors[index] = color;
        }   

        const gfx::ArgbColor& aGet(int index) const
        {
            return _argbColors[index];
        }

        const gfx::ArgbColor& aControl() const
        {
            return _argbColors[(int)ColorType::Control];
        }

        const gfx::Color& control() const
        {
            return _colors[(int)ColorType::Control] ;
        }

        void setControl(const gfx::Color& c)
        {
            _colors[(int)ColorType::Control] = c;
        }


        void setAControl(const gfx::ArgbColor& c)
        {
            _argbColors[(int)ColorType::Control] = c;
        }

        const gfx::ArgbColor& aControlDark() const
        {
            return _argbColors[(int)ColorType::ControlDark];
        }

        const gfx::Color& controlDark() const
        {
            return _colors[(int)ColorType::ControlDark] ;
        }

        void setControlDark(const gfx::Color& c)
        {
            _colors[(int)ColorType::ControlDark] = c;
        }

        const gfx::ArgbColor& aControlDarkDark() const
        {
            return _argbColors[(int)ColorType::ControlDarkDark];
        }

        const gfx::Color& controlDarkDark() const
        {
            return _colors[(int)ColorType::ControlDarkDark] ;
        }

        void setControlDarkDark(const gfx::Color& c)
        {
            _colors[(int)ColorType::ControlDarkDark] = c;
        }

        const gfx::ArgbColor& aControlLight() const
        {
            return _argbColors[(int)ColorType::ControlLight];
        }

        const gfx::Color& controlLight() const
        {
            return _colors[(int)ColorType::ControlLight] ;
        }

        void setControlLight(const gfx::Color& c)
        {
            _colors[(int)ColorType::ControlLight] = c;
        }

        const gfx::ArgbColor& aControlLightLight() const
        {
            return _argbColors[(int)ColorType::ControlLightLight];
        }

        const gfx::Color& controlLightLight() const
        {
            return _colors[(int)ColorType::ControlLightLight] ;
        }

        void setControlLightLight(const gfx::Color& c)
        {
            _colors[(int)ColorType::ControlLightLight] = c;
        }

        const gfx::ArgbColor& aControlDisabled() const
        {
            return _argbColors[(int)ColorType::ControlDisabled];
        }

        const gfx::Color& controlDisabled() const
        {
            return _colors[(int)ColorType::ControlDisabled];
        }

        void setControlDisabled(const gfx::Color& c)
        {
            _colors[(int)ColorType::ControlDisabled] = c;   
        }
       
        const gfx::ArgbColor& aControlHighlight() const
        {
            return _argbColors[(int)ColorType::ControlHighlight];
        }

        const gfx::Color& controlHighlight() const
        {
            return _colors[(int)ColorType::ControlHighlight];
        }

        void setControlHighlight(const gfx::Color& c)
        {
            _colors[(int)ColorType::ControlHighlight] = c;
        }

        const gfx::ArgbColor& aControlFrame() const
        {
            return _argbColors[(int)ColorType::ControlFrame];   
        }

         void setAControlFrame(const gfx::ArgbColor& c)
        {
            _argbColors[(int)ColorType::ControlFrame] = c;
        }


        const gfx::Color& controlFrame() const
        {
            return _colors[(int)ColorType::ControlFrame];
        }

        void setControlFrame(const gfx::Color& c)
        {
            _colors[(int)ColorType::ControlFrame] = c;
        }


        const gfx::ArgbColor& aControlFrameDisabled() const
        {
            return _argbColors[(int)ColorType::ControlFrameDisabled] ;
        }

        const gfx::Color& controlFrameDisabled() const
        {
            return _colors[(int)ColorType::ControlFrameDisabled];
        }

        void setControlFrameDisabled(const gfx::Color& c)
        {
            _colors[(int)ColorType::ControlFrameDisabled] = c;
        }
        
        const gfx::ArgbColor& aControlText() const
        {
            return _argbColors[(int)ColorType::ControlText];
        }

        const gfx::Color& controlText() const
        {
            return _colors[(int)ColorType::ControlText];
        }

        void setControlText(const gfx::Color& c)
        {
            _colors[(int)ColorType::ControlText] = c;
        }        

        const gfx::ArgbColor& aCcontrolTextDisabled() const
        {
            return _argbColors[(int)ColorType::ControlTextDisabled];
        }

        const gfx::Color& controlTextDisabled() const
        {
            return _colors[(int)ColorType::ControlTextDisabled];
        }

        void setControlTextDisabled(const gfx::Color& c)
        {
            _colors[(int)ColorType::ControlTextDisabled] = c;
        }

        const gfx::ArgbColor& aControlTextHighlight() const
        {
            return _argbColors[(int)ColorType::ControlTextHighlight];
        }

        const gfx::Color& controlTextHighlight() const
        {
            return _colors[(int)ColorType::ControlTextHighlight];
        }

        void setControlTextHighlight(const gfx::Color& c)
        {
            _colors[(int)ColorType::ControlTextHighlight] = c;
        }
        

        const gfx::ArgbColor& aInfo() const
        {
            return _argbColors[(int)ColorType::Info];
        }


        const gfx::Color& info() const
        {
            return _colors[(int)ColorType::Info];
        }

        void setInfo(const gfx::Color& c)
        {
            _colors[(int)ColorType::Info] = c;
        }




        const gfx::ArgbColor& aInfoText() const
        {
            return _argbColors[(int)ColorType::InfoText];
        }

        const gfx::Color& infoText() const
        {
            return _colors[(int)ColorType::InfoText];
        }


        void setInfoText(const gfx::Color& c)
        {
            _colors[(int)ColorType::InfoText] = c;
        }




        const gfx::ArgbColor& aTextBackground() const
        {
            return _argbColors[(int)ColorType::TextBackground];
        }


        const gfx::Color& textBackground() const
        {
            return _colors[(int)ColorType::TextBackground];
        }

        void setTextBackground(const gfx::Color& c)
        {
            _colors[(int)ColorType::TextBackground] = c;
        }


        void setATextBackground(const gfx::ArgbColor& c)
        {
            _argbColors[(int)ColorType::TextBackground] = c;
        }
 

        void setWindow(const gfx::Color& c)
        {
            _colors[(int)ColorType::Window] = c;
        }

        const gfx::ArgbColor& aText() const
        {
            return _argbColors[(int)ColorType::Text];
        }

        const gfx::Color& text() const
        {
            return _colors[(int)ColorType::Text];
        }


        void  setText(const gfx::Color& c)
        {
            _colors[(int)ColorType::Text] = c;
        }



        const gfx::ArgbColor& aTextHighlight() const
        {
            return _argbColors[(int)ColorType::TextHighlight];
        }

        const gfx::Color& textHighlight() const
        {
            return _colors[(int)ColorType::TextHighlight];
        }

        void  setTextHighlight(const gfx::Color& c)
        {
            _colors[(int)ColorType::TextHighlight] = c;
        }


        const gfx::ArgbColor& aTextDisabled() const
        {
            return _argbColors[(int)ColorType::TextDisabled];
        }


        const gfx::Color& textDisabled() const
        {
            return _colors[(int)ColorType::TextDisabled];
        }

        void  setTextDisabled(const gfx::Color& c)
        {
            _colors[(int)ColorType::TextDisabled] = c;
        }


        const gfx::ArgbColor& aWindowTitleText() const
        {
            return _argbColors[(int)ColorType::WindowTitleText];
        }

        const gfx::Color& windowTitleText() const
        {
            return _colors[(int)ColorType::WindowTitleText];    
        }

        void setWindowTitleText(const gfx::Color& c)
        {
            _colors[(int)ColorType::WindowTitleText] = c;
        }


        const gfx::ArgbColor& aWindowTitleBar() const
        {
            return _argbColors[(int)ColorType::WindowTitleBar];
        }


        const gfx::Color& windowTitleBar() const
        {
            return _colors[(int)ColorType::WindowTitleBar];
        }   

        void setWindowTitleBar(const gfx::Color& c)
        {
            _colors[(int)ColorType::WindowTitleBar] = c;
        }   


        const gfx::ArgbColor& aWindowTitleBarActive() const
        {
            return _argbColors[(int)ColorType::WindowTitleBarActive];
        }

        const gfx::Color& windowTitleBarActive() const
        {
            return _colors[(int)ColorType::WindowTitleBarActive];
        }   

        void setWindowTitleBarActive(const gfx::Color& c)
        {
            _colors[(int)ColorType::WindowTitleBarActive] = c;
        }   


        const gfx::ArgbColor& aWindowTitleBarDisabled() const
        {
            return _argbColors[(int)ColorType::WindowTitleBarDisabled]  ;
        }

        const gfx::Color&  windowTitleBarDisabled() const   
        {
            return _colors[(int)ColorType::WindowTitleBarDisabled];
        }   

        void setWindowTitleBarDisabled(const gfx::Color& c)
        {
            _colors[(int)ColorType::WindowTitleBarDisabled] = c;
        }   


        const gfx::ArgbColor& aWindow() const
        {
            return _argbColors[(int)ColorType::Window];
        }

        const gfx::Color& window() const
        {
            return _colors[(int)ColorType::Window];
        }

        const gfx::ArgbColor& aWindowFrame() const
        {
            return _argbColors[(int)ColorType::WindowFrame];
        }

        const gfx::Color&  windowFrame() const
        {
            return _colors[(int)ColorType::WindowFrame];
        }   

        void setWindowFrame(const gfx::Color& c)
        {
            _colors[(int)ColorType::WindowFrame] = c;
        }   


        const gfx::ArgbColor& aWindowBorder() const
        {
            return _argbColors[(int)ColorType::WindowBorder];
        }   

        const gfx::Color& windowBorder() const
        {
            return _colors[(int)ColorType::WindowBorder];
        }

        void setWindowBorder(const gfx::Color& c)
        {
            _colors[(int)ColorType::WindowBorder] = c;
        }   


        const gfx::ArgbColor& aWindowBorderActive() const
        {
            return _argbColors[(int)ColorType::WindowBorderActive];
        }   

        const gfx::Color&  windowBorderActive() const
        {
            return _colors[(int)ColorType::WindowBorderActive];
        }

        void setWindowBorderActive(const gfx::Color& c)
        {
            _colors[(int)ColorType::WindowBorderActive] = c;
        }


        const gfx::ArgbColor&  aWindowBorderDisabled() const
        {
            return _argbColors[(int)ColorType::WindowBorderDisabled];
        }

        const gfx::Color&  windowBorderDisabled() const
        {
            return _colors[(int)ColorType::WindowBorderDisabled] ;
        }

        void setWindowBorderDisabled(const gfx::Color& c)
        {
            _colors[(int)ColorType::WindowBorderDisabled] = c;
        }


        const gfx::ArgbColor& aDesktop() const
        {
            return _argbColors[(int)ColorType::Desktop];   
        }

        const gfx::Color& desktop() const
        {
            return _colors[(int)ColorType::Desktop];
        }

        void setDesktop(const gfx::Color& c)
        {
            _colors[(int)ColorType::Desktop] = c;
        }   


        const gfx::ArgbColor& aWorkingspace() const
        {
            return _argbColors[(int)ColorType::Workingspace];
        }

        const gfx::Color& workingspace() const
        {
            return _colors[(int)ColorType::Workingspace];
        }

        void setWorkingspace(const gfx::Color& c)
        {
            _colors[(int)ColorType::Workingspace] = c;
        }        

        void setPalleteId(uint32_t id);

        void load();

    private:                
        gfx::Color _colors[(int)ColorType::Count];    
        gfx::ArgbColor _argbColors[(int)ColorType::Count];

};

}}}

#endif
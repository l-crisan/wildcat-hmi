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
#ifndef WILDCAT_GUI_GFX_COLOR_H
#define WILDCAT_GUI_GFX_COLOR_H

#include <wildcat/gui/gfx/Api.h>
#include <Pt/SerializationInfo.h>
#include <inttypes.h>
#include <string>

namespace wildcat {
namespace gui {
namespace gfx {

class WILDCAT_GUI_GFX_API ArgbColor
{
    public:
        ArgbColor();

        ArgbColor(uint32_t v);

        ArgbColor(const std::string& name);

        ArgbColor(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

        ArgbColor(uint8_t r, uint8_t g, uint8_t b);

        uint8_t alpha() const
        {
            return _color >> 24;
        }

        uint8_t red() const
        {
            return (_color & 0x00FF0000) >> 16;
        }

        uint8_t green() const
        {
            return (_color & 0x0000FF00) >> 8;
        }

        uint8_t blue() const
        {
            return _color & 0x000000FF;
        }

        void setAlpha(uint8_t a)
        {   
            _color = (_color & 0x00FFFFFF) | (uint32_t(a) << 24);
        }

        void setRed(uint8_t r)
        {
            _color = (_color & 0xFF00FFFF) | (uint32_t(r) << 16);
        }

        void setGreen(uint8_t g)
        { 
            _color = (_color & 0xFFFF00FF) | (uint32_t(g) << 8);
        }

        void setBlue(uint8_t b)
        { 
            _color = (_color & 0xFFFFFF00) | uint32_t(b);
        }

        void set(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
        {
            setAlpha(a);
            setRed(r);
            setGreen(g);
            setBlue(b);
        }

        bool operator==(const ArgbColor& c) const
        {
            return _color == c._color;
        }

        bool operator!=(const ArgbColor& c) const
        {
            return _color != c._color;
        }

        uint32_t color() const
        {
            return _color;
        }

        void setColor(uint32_t v)
        {
            _color = v;
        }
        
        void invert();

    public:
        static ArgbColor fromRgb8(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

        static ArgbColor fromName(const std::string& name);

    private:
        uint32_t _color;
};

inline uint16_t toRgb565(const ArgbColor& c)
{
    return ((c.red() & 0xf8) << 8) | ((c.green() & 0xfc) << 3) | (c.blue() >> 3);
}

inline uint16_t toRgb565(uint32_t argb)
{
    return ((argb >> 16 & 0xf8) << 8) | ((argb >> 8 & 0xfc) << 3) | ((argb & 0xff) >> 3);
}

inline uint32_t fromRgb565(uint16_t c)
{
    return 0xFF000000 | ((c & 0xf800) << 8) | ((c & 0x07e0) << 5) | ((c & 0x001f) << 3);
}   


WILDCAT_GUI_GFX_API std::ostream& operator<<(std::ostream& os, const ArgbColor& obj);

WILDCAT_GUI_GFX_API std::istream& operator>>(std::istream& is, ArgbColor& obj);

class WILDCAT_GUI_GFX_API Color
{
    public:
        Color();

        Color(const ArgbColor& c);

        Color(int palette, int index);

        Color(const std::string& name);

        Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

        Color(uint8_t r, uint8_t g, uint8_t b);

        uint8_t alpha() const;

        uint8_t red() const;

        uint8_t green() const;

        uint8_t blue() const;

        uint32_t color() const;

        ArgbColor getColor() const;

        void invert();

        void load();
        
        void setColor( const ArgbColor& c)
        {
            _color= c;
            clearPallete();
        }

        void setColor(uint32_t c)
        {
            _color.setColor(c);
            clearPallete();
        }

        void setAlpha(uint8_t a)
        {
            if(_index != -1)
                _color = color();

            _color.setAlpha(a);
            clearPallete();
        }

        void setRed(uint8_t r)
        {
            if (_index != -1)
                _color = color();

            _color.setRed(r);
            clearPallete();
        }

        void setGreen(uint8_t g)
        {
            if (_index != -1)
                _color = color();

            _color.setGreen(g);
            clearPallete();
        }

        void setBlue(uint8_t b)
        {
            if (_index != -1)
                _color = color();

            _color.setBlue(b);
            clearPallete();
        }

        int palette() const
        {
            return _palette;
        }

        void setPalette(int v)
        {
            _palette = v;

        }

        int index() const
        {
            return _index;
        }

        void setIndex(int v)
        {
            _index = v;
        }

        bool operator==(const Color& c) const
        {
            return getColor() == c.getColor();
        }

        bool operator!=(const Color& c) const
        {
            return getColor() != c.getColor();
        }

        void operator=(const ArgbColor& c)
        {
            _color  = c;
        }

        std::string toString() const;

        static Color fromRgb8(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        {
            return Color(ArgbColor::fromRgb8(r,g,b,a));
        }

        static Color fromName(const std::string& n)
        {
            return Color(ArgbColor::fromName(n));
        }

    private:
        void clearPallete()
        {
            _palette = -1;
            _index = -1;
        }
       
    private:
        ArgbColor   _color;
        int     _palette;
        int     _index;
};

WILDCAT_GUI_GFX_API std::ostream& operator<<(std::ostream& os, const Color& obj);


WILDCAT_GUI_GFX_API std::istream& operator>>(std::istream& is, Color& obj);

}}}

#endif

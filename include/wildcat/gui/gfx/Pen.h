/*
   Copyright (C) 2017 Marc Boris Duerner
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
#ifndef WILDCAT_GUI_GFX_PEN_H
#define WILDCAT_GUI_GFX_PEN_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Color.h>
#include <Pt/SmartPtr.h>
#include <cstddef>

namespace wildcat {
namespace gui {
namespace gfx {

class PenData;

class WILDCAT_GUI_GFX_API Pen 
{
    public:
        enum Style 
        { 
            Solid = 0,
            Dot   = 1,
            Dash  = 2,
            DashPattern = 3
        };
        enum CapStyle 
        { 
            FlatCap   = 0,
            SquareCap = 1,
            RoundCap  = 2
        };

        enum JoinStyle 
        { 
            NoJoin    = 0,
            BevelJoin = 1,
            MiterJoin = 2,
            RoundJoin = 3
        };

        /** @brief Constructs a null pen.

            The default pen is null.
        */
        Pen();

        /** @brief Constructs a Pen with the specified color.

            The pen size is 1, the style is solid and the cap and join
            styles are round.
        */
        Pen(const ArgbColor& color);

        /** @brief Constructs a Pen with the specified size, color and styles.
        */
        Pen(const ArgbColor& color, std::size_t width, Style style = Solid,
            CapStyle cap = FlatCap, JoinStyle join = BevelJoin);

        /** @brief Constructs a Pen with the specified size, color and custom styles.
        */
        Pen(const ArgbColor& color, std::size_t width,
            const std::vector<Pt::uint8_t>& dashPattern,
            CapStyle cap = FlatCap, JoinStyle join = BevelJoin);


        bool isNull() const
        {
            return size() == 0;
        }


        void setColor(const ArgbColor& color)
        {
            _color = color;
        }


        const ArgbColor& color() const
        {
            return _color;
        }


        void setSize(std::size_t size)
        {
            _size = size;
        }


        std::size_t size() const
        {
            return _size;
        }


        void setStyle(Style style)
        {
            _style = style;
        }


        Style style() const
        {
            return _style;
        }


        void setDashPattern(const std::vector<Pt::uint8_t>& dashPattern)
        {

            _dashPattern = dashPattern;
        }


        const std::vector<Pt::uint8_t>& dashPattern() const
        {
            return _dashPattern;
        }


        void setCapStyle(CapStyle cap)
        {
            _capStyle = cap;
        }


        CapStyle capStyle() const
        {
            return _capStyle;
        }


        void setJoinStyle(JoinStyle join)
        {

            _joinStyle = join;
        }


        JoinStyle joinStyle() const
        {
            return _joinStyle;
        }

    private:
        ArgbColor                _color;
        std::size_t              _size;
        Pen::Style               _style;
        std::vector<Pt::uint8_t> _dashPattern;
        Pen::CapStyle            _capStyle;
        Pen::JoinStyle           _joinStyle;
};



WILDCAT_GUI_GFX_API  std::ostream& operator<<(std::ostream& os, const Pen& obj);


WILDCAT_GUI_GFX_API std::istream& operator>>(std::istream& is, Pen& obj);

}}}

#endif

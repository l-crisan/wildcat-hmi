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
#ifndef WILDCAT_GUI_GFX_SHAPE_H
#define WILDCAT_GUI_GFX_SHAPE_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Graphics.h>
#include <wildcat/gui/gfx/Point.h>

namespace wildcat {
namespace gui{  
namespace gfx {

class WILDCAT_GUI_GFX_API Shape
{
    public:
        Shape();

        virtual ~Shape();

        virtual void draw(Graphics& gf) const = 0;        

        virtual void translate(const gfx::Point& v) = 0;

        virtual void scale(double factor) = 0;


        double width() const
        {
            return _size.width();
        }

        double height() const
        {
            return _size.height();
        }

        const gfx::Size& size()  const
        {
            return _size;
        }

        void setSize(const gfx::Size& v)
        {
            _size = v;
        }

        void reset()
        {
            onReset();
        }
    protected:
        virtual void onReset()
        {

        }
    protected:
        gfx::Size _size;

};

}}}

#endif

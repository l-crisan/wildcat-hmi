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
#ifndef WILDCAT_GUI_GFX_FONTMETRICS_H
#define WILDCAT_GUI_GFX_FONTMETRICS_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Size.h>
#include <Pt/Types.h>
#include <cstddef>

namespace wildcat {
namespace gui {
namespace gfx {
    

class WILDCAT_GUI_GFX_API FontMetrics
{
    public:
        FontMetrics();

        int ascent() const
        {
            return _ascent;
        }

        void setAscent(int n)
        {
            _ascent = n;
        }

        int descent() const
        {
            return _descent;
        }

        void setDescent(int n)
        {
            _descent = n;
        }

        int capHeight() const
        {
            return _capHeight;
        }

        void setCapHeight(int n)
        {
            _capHeight = n;
        }

        int leading() const
        {
            return _leading;
        }

        void setLeading(int n)
        {
            _leading = n;
        }

        int height() const
        {
            return _ascent + _descent;
        }

        int width() const
        {
            return _width;
        }

        void setWidth(int v)
        {
            _width = v;
        }

        int lineHeight() const
        {
            return height();
        }
        
        gfx::Point baseline() const
        {
            return gfx::Point(0, _ascent);
        }

    private:
        int _ascent;
        int _descent;
        int _capHeight;
        int _leading;
        int _width;
};

}}}

#endif

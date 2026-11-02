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
#ifndef WILDCAT_GUI_GFX_CLIPPOLYGON_H
#define WILDCAT_GUI_GFX_CLIPPOLYGON_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Rect.h>
#include <vector>

namespace wildcat{
namespace gui { 
namespace gfx{

class WILDCAT_GUI_GFX_API ClipPolygon
{
    public:
        /** @brief Default constructor
            The default constructor does nothing.
        */
        ClipPolygon();

        /** @brief Perform clipping

            @see ClipPolygon::clip
        */
        void operator() (std::vector<Point>& in,
                         const Rect& clippingArea )
        { this-> clip(in, clippingArea); }

        /** @brief Perform clipping

            The polygon described by a vector of points is clipped
            against a clipping rectangle. The vector of points will
            be modified, thus the clipping results in a new polygon.

            @param in Polygon points
            @param clippingArea Rectangle to clip against

        */
        void clip( std::vector<Point>& in,
                   const Rect& clippingArea );

    private:
        enum Orientation{Left, Right, Top, Bottom} ;

        void clipEdge( const std::vector<Point>& in,
                       std::vector<Point>& out,
                       Point edgePoint0, Point edgePoint1);

        Point intersect( const Point& from,
                                   const Point& to,
                                   const Point& edge0,
                                   const Point& edge1 );

        bool inside( const Point& p, const Point& edge0,
                     const Point& edge1 );
};

}}}

#endif

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
#ifndef WILDCAT_GUI_GFX_REGIONGRAPHICS_H
#define WILDCAT_GUI_GFX_REGIONGRAPHICS_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Rect.h>
#include <wildcat/gui/gfx/Pen.h>
#include <wildcat/gui/gfx/Brush.h>
#include <wildcat/gui/gfx/Font.h>
#include <wildcat/gui/gfx/FontMetrics.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/gfx/Graphics.h>
#include <wildcat/gui/gfx/Transform.h>

namespace wildcat {
namespace gui {
namespace gfx {

class WILDCAT_GUI_GFX_API RegionGraphics : public Graphics
{
    public:
        RegionGraphics();

        RegionGraphics(Graphics& base, const Rect& rect);

        virtual ~RegionGraphics();
        
        Graphics* base() const;

        void detach();

        void attach(Graphics& base, const Rect& rect);

        const Rect& area() const;

        void move(const Point& size);

        void resize(const Size& size);

        virtual const Pixmap& image() const;


        virtual const Size& onSize() const;

        
        virtual void setCompositionMode(const CompositionMode& mode);

        virtual CompositionMode compositionMode() const;


        virtual void setPen(const Pen& pen);

        virtual const Pen& pen() const;


        virtual void setBrush(const Brush& brush);

        virtual const Brush& brush() const;


        virtual void setFont(const Font& font);
        
        virtual const Font& font() const;


        virtual void setClip(const Rect& clip);

        virtual const Rect& clip() const;

        virtual void resetClip();

        virtual FontMetrics fontMetrics(const Pt::String& text) const;

        virtual void drawText(const Point& to, const Pt::String& text);

        virtual void drawText(const Point& to, const Pt::String& text, const Transform& trans);

        virtual void drawText(const Point& to, const Pt::String& text, const ArgbColor& highlight);

        virtual void drawText(const Point& to, const Pt::String& text, const Transform& trans, const ArgbColor& highlight);

        virtual void drawLine(const Point& from, const Point& to);

        virtual void drawPolyline(const Point* points, size_t count);

        virtual void fillPolygon(const Point* points, size_t count);

        virtual void drawRect(const Rect& rectangle);

        virtual void fillRect(const Rect& rectangle);

        virtual void drawEllipse(const Point& topLeft, const Size& size);

        virtual void fillEllipse(const Point& topLeft, const Size& size);

        virtual void drawChord(const Point& topLeft, const Size& size, float degBegin, float degEnd);

        virtual void fillChord(const Point& topLeft, const Size& size, float degBegin, float degEnd);

        virtual void drawPie(const Point& topLeft, const Size& size, float degBegin, float degEnd);

        virtual void fillPie(const Point& topLeft, const Size& size, float degBegin, float degEnd);

        virtual void drawArc(const Point& topLeft, const Size& size, float degBegin, float degEnd);

        virtual void drawImage(const Point& to, const Pixmap& image);

        virtual void drawImage(const Point& to, const Pixmap& image, const Rect& imgRect);
    private:
        Graphics* _base;
        Rect    _area;
        mutable Rect  _clip;
        Pixmap _myImage;
};

}}}

#endif

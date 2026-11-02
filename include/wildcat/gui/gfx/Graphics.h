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
#ifndef WILDCAT_GUI_GFX_GRAPHICS_H
#define WILDCAT_GUI_GFX_GRAPHICS_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Rect.h>
#include <wildcat/gui/gfx/Pen.h>
#include <wildcat/gui/gfx/Brush.h>
#include <wildcat/gui/gfx/Font.h>
#include <wildcat/gui/gfx/FontInfo.h>
#include <wildcat/gui/gfx/FontMetrics.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/gfx/Transform.h>
#include <wildcat/gui/gfx/Icon.h>
#include <wildcat/gui/gfx/Polygon.h>
#include <Pt/System/Path.h>
#include <vector>

namespace wildcat {
namespace gui{
namespace gfx {

class Shape;

class WILDCAT_GUI_GFX_API Graphics
{
    public:
        Graphics();

        virtual ~Graphics();

        const Size& size() const;        
        
        double scaleFactor() const
        {
            return _scaleFactor;
        }

        void setScaleFactor(double v)
        {
            _scaleFactor = v;
        }

        virtual const Pixmap& image() const = 0;

        virtual const Size& onSize() const = 0;

        virtual void setCompositionMode(const CompositionMode& mode) = 0;

        virtual CompositionMode compositionMode() const = 0;
        
        virtual void setPen(const Pen& pen) = 0;

        virtual const Pen& pen() const = 0;

        virtual void setBrush(const Brush& brush) = 0;

        virtual const Brush& brush() const = 0;

        virtual void setFont(const Font& font) = 0;

        virtual const Font& font() const = 0;

        virtual void setClip(const Rect& clip) = 0;

        virtual const Rect& clip() const = 0;

        virtual void resetClip() = 0;

        virtual FontMetrics fontMetrics(const Pt::String& text) const = 0;        

        virtual void drawText(const Point& to, const Pt::String& text) = 0;

        virtual void drawText(const Point& to, const Pt::String& text, const ArgbColor& highlight) = 0;

        virtual void drawText(const Point& to, const Pt::String& text, const Transform& trans) = 0;

        virtual void drawText(const Point& to, const Pt::String& text, const Transform& trans, const ArgbColor& highlight) = 0;

        virtual void drawLine(const Point& from, const Point& to) = 0;
    
        virtual void drawPolyline(const Point* points, size_t count) = 0;

        virtual void fillPolygon(const Point* points, size_t count) = 0;

        virtual void drawRect(const Rect& rectangle) = 0;

        virtual void fillRect(const Rect& rectangle) = 0;

        virtual void drawEllipse(const Point& topLeft, const Size& size) = 0;

        virtual void fillEllipse(const Point& topLeft, const Size& size) = 0;

        virtual void fillCircle(const Point& topLeft, int  diameter)
        {
            fillEllipse(topLeft, Size(diameter, diameter));
        }

        virtual void drawChord(const Point& topLeft, const Size& size, float degBegin, float degEnd) = 0;

        virtual void fillChord(const Point& topLeft, const Size& size, float degBegin, float degEnd) = 0;

        virtual void drawPie(const Point& topLeft, const Size& size, float degBegin, float degEnd) = 0;

        virtual void fillPie(const Point& topLeft, const Size& size, float degBegin, float degEnd) = 0;

        virtual void drawArc(const Point& topLeft, const Size& size, float degBegin, float degEnd) = 0;

        virtual void drawImage(const Point& to, const Pixmap& image) = 0;

        virtual void drawImage(const Point& to, const Pixmap& image, const Rect& imgRect) = 0;

        virtual void drawShape(const Shape& shape);

        virtual void drawFrame(const Rect& r, int corner);

        virtual void fillFrame(const Rect& r, int corner);

        const std::string& name() const
        {
            return _name;
        }

        void setName(const std::string& n)
        {
            _name = n;
        }

    public:
        Point toPhysical(const Point& p) const
        {
            PointF pp(p.x()* scaleFactor(), p.y()* scaleFactor()); 
            return Point(std::round(pp.x()), std::round(pp.y()));
        }

        Size toPhysical(const Size& s) const
        {
            SizeF ss(s.width()* scaleFactor(), s.height()* scaleFactor());
            return Size(std::round(ss.width()), std::round(ss.height()));
        }

        Rect toPhysical(const Rect& r) const
        {
            if (r.isNull())
                return Rect(toPhysical(r.topLeft()), Size(0, 0));

            Rect rr = r;

            rr.scale(scaleFactor());    
            return rr;  
        }

        Point toLogical(const Point& p) const
        {
            PointF pp(p.x()/ scaleFactor(), p.y()/ scaleFactor());

            return Point(std::round(pp.x()), std::round(pp.y()));
        }

        Size toLogical(const Size& s) const
        {
            gfx::SizeF ss(s.width()/ scaleFactor(), s.height()/ scaleFactor());

            return Size(std::round(ss.width()), std::round(ss.height()));
        }

        Rect toLogical(const Rect& r) const
        {
            return Rect(toLogical(r.topLeft()), toLogical(r.size()));
        }

        int toLogical(int n) const
        {
            return std::round(n / scaleFactor());
        }

        int toPhysical(int n) const
        {
            return std::round(n * scaleFactor());
        }


    public:
        static void setFontDir(const Pt::System::Path& path);

        static const gfx::Font& defaultFont();

        static void setDefaultFont(const gfx::Font& name);

        static const std::vector<gfx::FontInfo>& fonts();

        static FontMetrics fontMetrics(const Font& font, const Pt::String& text);

        static gfx::Polygon toPolygon(const gfx::Rect& rect, int corner);

        static Size fontMetricsBox(const Pt::String& text, const Font& font);

    private:
        double _scaleFactor;
        std::string _name;
};

}}}

#endif

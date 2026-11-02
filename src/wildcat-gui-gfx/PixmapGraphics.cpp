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
#include <wildcat/gui/gfx/PixmapGraphics.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/gfx/Pen.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Brush.h>
#include <wildcat/gui/gfx/Font.h>
#include <wildcat/gui/gfx/FontMetrics.h>
#include <wildcat/gui/gfx/Algorithm.h>
#include <Pt/System/Clock.h>
#include <Pt/String.h>
#include <Pt/Math.h>
#include "Rasterizer.h"
#include <algorithm>

namespace wildcat {
namespace gui{
namespace gfx {


PixmapGraphics::PixmapGraphics()
: _image()
, _rasterizer(new Rasterizer(_image))
{
}


PixmapGraphics::PixmapGraphics(const Size& size, std::size_t stride)
: _rasterizer(new Rasterizer(_image))
{
  _image.resize( size, stride);
}


PixmapGraphics::~PixmapGraphics()
{
  delete _rasterizer;
}


const Pixmap& PixmapGraphics::image() const
{
    return _rasterizer->image();
}

Pixmap& PixmapGraphics::image()
{
    return _rasterizer->image();
}

const Size& PixmapGraphics::onSize() const
{
    return _size;
}


void PixmapGraphics::resize(const Size& size)
{ 
    reset(size);
}

void PixmapGraphics::reset(const Pixmap& image)
{
    _size =  Size( image.width(), image.height()  );

    _image = image;
    _rasterizer->reset(_image);

    _clip = Rect(Point(0,0), _size);
    _rasterizer->resetClip();
}


void PixmapGraphics::reset(const Size& size, std::size_t stride)
{
    _size = size;
    gfx::Size se = toPhysical(size);

    _image.resize(se, stride);
    setClip(Rect(Point(0,0), _size));
}


void PixmapGraphics::setCompositionMode(const CompositionMode& mode)
{
    _rasterizer->setCompositionMode(mode);
    _compMode =mode;
}


void PixmapGraphics::setClip(const Rect& rect)
{
    _clip = rect;
    _rasterizer->setClip(toPhysical(rect));
}

void PixmapGraphics::resetClip()
{
    _clip = Rect(Point(0,0), _size);
    _rasterizer->resetClip();
}

void PixmapGraphics::clear(const ArgbColor& color)
{
    _rasterizer->setBrush(color);
    _rasterizer->fillRect(Rect(Point(0,0), _image.size()));
}

void PixmapGraphics::setPen(const Pen& pen)
{

  size_t penSize = pen.size() <= 1 ? 1 : static_cast<size_t>(std::max(1, toPhysical(static_cast<int>(pen.size()))));

   Pen scaledPen = pen;
   
   scaledPen.setSize(penSize);

    if (pen.color().alpha() != 255)
        setCompositionMode(CompositionMode::SourceOver);
    else
        setCompositionMode(CompositionMode::SourceCopy);

  _pen = pen;

  _rasterizer->setPen(scaledPen) ;
}


void PixmapGraphics::setBrush(const Brush& brush)
{
    if( brush.color().alpha() != 255)
        setCompositionMode(CompositionMode::SourceOver);
    else
        setCompositionMode(CompositionMode::SourceCopy);

    _rasterizer->setBrush(brush);
}


void PixmapGraphics::setFont(const Font& font)
{
    _font = font;
    _rasterizer->setFont(_font);
}

FontMetrics PixmapGraphics::fontMetrics(const Pt::String& text) const
{
	return _rasterizer->fontMetrics(text);
}

void PixmapGraphics::drawLine(const Point& from, const  Point& to)
{
    _rasterizer->drawLine( toPhysical(from),  toPhysical(to) );
}

void PixmapGraphics::drawText(const Point& to, const Pt::String& text)
{
    const double sf = scaleFactor();

    Transform trans;
    trans.scale(sf, sf);

    _rasterizer->drawText(toPhysical(to), text, trans);
}

void PixmapGraphics::drawText(const Point& to, const Pt::String& text, const ArgbColor& highlight)
{    
    drawText(to, text, Transform(), highlight);
}

void PixmapGraphics::drawText(const Point& to, const Pt::String& text, const Transform& t, const ArgbColor& highlight)
{
    double sf = scaleFactor();

    Transform trans = t;
    trans.scale(sf, sf);

    Point o = toPhysical(to);
    Pen oldPen = pen();

    setPen(highlight);

    {// left
        Point p1 = o;

        p1.subX(1);

        _rasterizer->drawText(p1, text, trans);
    }

    {// top left
        Point p1 = o;

        p1.subX(1);
        p1.subY(1);

        _rasterizer->drawText(p1, text, trans);
    }

    {// top
        Point p1 = o;

        p1.subY(1);

        _rasterizer->drawText(p1, text, trans);
    }

    {// top right
        Point p1 = o;

        p1.addX(1);
        p1.subY(1);

        _rasterizer->drawText(p1, text, trans);
    }

    {// right
        Point p1 = o;

        p1.addX(1);

        _rasterizer->drawText(p1, text, trans);
    }

    {// bottom right
        Point p1 = o;

        p1.addX(1);
        p1.addY(1);

        _rasterizer->drawText(p1, text, trans);
    }

    {// bottom
        Point p1 = o;

        p1.addY(1);

        _rasterizer->drawText(p1, text, trans);
    }

    {// bottom left
        Point p1 = o;

        p1.subX(1);
        p1.addY(1);

        _rasterizer->drawText(p1, text, trans);
    }

    //Center
    setPen(oldPen);
    _rasterizer->drawText(o, text, trans);
}



void PixmapGraphics::drawText(const Point& to, const Pt::String& text, 
                            const Transform& t)
{
    double sf = scaleFactor();

    Transform trans = t;
    trans.scale(sf, sf);

    _rasterizer->drawText(toPhysical(to), text, trans);
}


void PixmapGraphics::drawRect(const Rect& r)
{
    if (r.isNull())
        return;

    _rasterizer->drawRect( toPhysical(r) );
}


void PixmapGraphics::fillRect(const Rect& r)
{
    if(r.isNull())
      return;

    _rasterizer->fillRect( toPhysical(r) );
}


void PixmapGraphics::drawEllipse(const Point& topLeft, const Size& size)
{
    Rect pr = toPhysical(Rect(topLeft, size));
    _rasterizer->drawEllipse(pr.topLeft(), pr.size());
}


void PixmapGraphics::fillEllipse(const Point& topLeft, const Size& size)
{
    Rect pr = toPhysical(Rect(topLeft, size));
    _rasterizer->fillEllipse(pr.topLeft(), pr.size());
}


void PixmapGraphics::drawPolyline(const Point* points, const size_t n)
{
    std::vector<Point> ps;

    for (size_t i = 0; i < n; ++i)
        ps.push_back( toPhysical(points[i]) );

    _rasterizer->drawPolyline(&ps[0], n);
}


void PixmapGraphics::fillPolygon(const Point* points, const size_t n)
{
    std::vector<Point> ps;

    for (size_t i = 0; i < n; ++i)
        ps.push_back( toPhysical(points[i]) );

    _rasterizer->fillPolygon(&ps[0], n);
}


void PixmapGraphics::drawImage(const Point& to, const Pixmap& image)
{
    gfx::Point p = toPhysical(to);  
    _rasterizer->drawImage( p, image);
}


void PixmapGraphics::drawImage(const Point& to, const Pixmap& image, const Rect& imageRect)
{
    const gfx::Rect dst = toPhysical(Rect(to, imageRect.size()));
    const gfx::Point origin = toPhysical(to - imageRect.topLeft());
    const gfx::Rect src(dst.topLeft() - origin, dst.size());

    _rasterizer->drawImage(dst.topLeft(), image, src);
}


void PixmapGraphics::drawChord(const Point& topLeft, const Size& size, float degBegin, float degEnd)
{
    //_rasterizer->drawChord(topLeft, size, degBegin, degEnd);
}


void PixmapGraphics::fillChord(const Point& topLeft, const Size& size, float degBegin, float degEnd)
{
}


void PixmapGraphics::drawPie(const Point& topLeft, const Size& size, float degBegin, float degEnd)
{
}


void PixmapGraphics::fillPie(const Point& topLeft, const Size& size, float degBegin, float degEnd)
{
}


void PixmapGraphics::drawArc(const Point& topLeft, const Size& size, float degBegin, float degEnd)
{
}

}}}

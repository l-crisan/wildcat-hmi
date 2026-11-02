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
#include <wildcat/gui/gfx/RegionGraphics.h>

namespace wildcat {
namespace gui {
namespace gfx {

RegionGraphics::RegionGraphics()
: _base(0)
{
}

RegionGraphics::RegionGraphics(Graphics& base, const Rect& rect)
: _base(&base)
, _area(rect)
{           
    
}

void RegionGraphics::detach()
{
    _base = 0;
    _area = Rect();
}

void RegionGraphics::attach(Graphics& base, const Rect& rect)
{
    _base = &base;
    _area = rect;
}

RegionGraphics::~RegionGraphics()
{
}

Graphics* RegionGraphics::base() const
{
    return _base;
}

const Rect& RegionGraphics::area() const
{
    return _area;
}

void RegionGraphics::move(const Point& pos)
{
    _area.setOrigin(pos);
}

void RegionGraphics::resize(const Size& size)
{   
    _area.setSize(size);
}

const Size& RegionGraphics::onSize() const
{
    return _area.size();
}


const Pixmap& RegionGraphics::image() const
{
    if(!_base)
        return _myImage;

    //Todo: cut sub Image
    return _base->image();
}

void RegionGraphics::setCompositionMode(const CompositionMode& mode)
{
    _base->setCompositionMode(mode);
}

CompositionMode RegionGraphics::compositionMode() const
{
    return _base->compositionMode();
}

void RegionGraphics::setPen(const Pen& pen)
{
    _base->setPen(pen);
}

const Pen& RegionGraphics::pen() const
{
    return _base->pen();
}

void RegionGraphics::setBrush(const Brush& brush)
{
    _base->setBrush(brush);
}

const Brush& RegionGraphics::brush() const
{
    return _base->brush();
}

void RegionGraphics::setFont(const Font& font)
{
    _base->setFont(font);
}

const Font& RegionGraphics::font() const
{
    return _base->font();
}

void RegionGraphics::setClip(const Rect& clip)
{
   _clip = clip;
   Point o(_area.topLeft() + clip.topLeft());

   Rect r(o, clip.size());
      
   _base->setClip(r.intersect(_area));
}

const Rect& RegionGraphics::clip() const
{
    gfx::Point pos = _base->clip().topLeft() - _area.topLeft();
    _clip = _base->clip();
    _clip.setOrigin(pos);
    return _clip ;
}

void RegionGraphics::resetClip()
{
    _base->resetClip();
}

FontMetrics RegionGraphics::fontMetrics(const Pt::String& text) const
{
    return _base->fontMetrics(text);
}

void RegionGraphics::drawText(const Point& to, const Pt::String& text)
{
    _base->drawText(to + _area.topLeft(), text);
}

void RegionGraphics::drawText(const Point& to, const Pt::String& text, const Transform& trans)
{
    _base->drawText(to + _area.topLeft(), text, trans);
}

void RegionGraphics::drawText(const Point& to, const Pt::String& text, const ArgbColor& highlight)
{
    _base->drawText(to + _area.topLeft(), text, highlight);
}

void RegionGraphics::drawText(const Point& to, const Pt::String& text, const Transform& trans, const ArgbColor& highlight)
{
    _base->drawText(to + _area.topLeft(), text, trans, highlight);
}


void RegionGraphics::drawLine(const Point& from, const Point& to)
{
    _base->drawLine(_area.topLeft() + from, _area.topLeft() + to);
}

void RegionGraphics::drawPolyline(const Point* points, size_t count)
{
    std::vector<Point> pp(count);

    for( size_t i = 0; i < count; ++i)
        pp[i] = _area.topLeft() + points[i];

    _base->drawPolyline(&pp[0], count);
}

void RegionGraphics::fillPolygon(const Point* points, size_t count)
{
    std::vector<Point> pp(count);

    for (size_t i = 0; i < count; ++i)
        pp[i] = _area.topLeft() + points[i];

    _base->fillPolygon(&pp[0], count);
}

void RegionGraphics::drawRect(const Rect& rectangle)
{
    Point o(_area.topLeft() + rectangle.topLeft());

    Rect r(o, rectangle.size());

    _base->drawRect(r);
}

void RegionGraphics::fillRect(const Rect& rectangle)
{
    Point o(_area.topLeft() + rectangle.topLeft());

    Rect r(o, rectangle.size());

    _base->fillRect(r);
}

void RegionGraphics::drawEllipse(const Point& topLeft, const Size& size)
{
    Point o(_area.topLeft() + topLeft);
   
    _base->drawEllipse(o, size);
}

void RegionGraphics::fillEllipse(const Point& topLeft, const Size& size)
{
    Point o(_area.topLeft() + topLeft);

    _base->fillEllipse(o, size);
}

void RegionGraphics::drawChord(const Point& topLeft, const Size& size, float degBegin, float degEnd)
{
    Point o(_area.topLeft() + topLeft);

    _base->drawChord(o, size, degBegin, degEnd);
}

void RegionGraphics::fillChord(const Point& topLeft, const Size& size, float degBegin, float degEnd)
{
    Point o(_area.topLeft() + topLeft);

    _base->fillChord(o, size, degBegin, degEnd);
}

void RegionGraphics::drawPie(const Point& topLeft, const Size& size, float degBegin, float degEnd)
{
    Point o(_area.topLeft() + topLeft);

    _base->drawPie(o, size, degBegin, degEnd);
}

void RegionGraphics::fillPie(const Point& topLeft, const Size& size, float degBegin, float degEnd)
{
    Point o(_area.topLeft() + topLeft);

    _base->fillPie(o, size, degBegin, degEnd);
}

void RegionGraphics::drawArc(const Point& topLeft, const Size& size, float degBegin, float degEnd)
{
    Point o(_area.topLeft() + topLeft);

    _base->drawArc(o, size, degBegin, degEnd);
}

void RegionGraphics::drawImage(const Point& to, const Pixmap& image)
{
    Point o = _area.topLeft() + to;

    _base->drawImage(o, image);
}

void RegionGraphics::drawImage(const Point& to, const Pixmap& image, const Rect& imgRect)
{
    Point o = _area.topLeft() + to;

    _base->drawImage(o, image, imgRect);
}


}}}

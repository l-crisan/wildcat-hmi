/*
   Copyright (C) 2026 Marc Boris Duerner
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
#include "FrameButton.h"
#include "WindowFrame.h"
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/window/SystemColors.h>
#include <Pt/Math.h>

namespace wildcat {
namespace gui {
namespace window {

FrameButton::FrameButton()
: _frame(0)
, _isPressed(false)
, _visible(true)
, _isHighlight(false)
{
}

FrameButton::~FrameButton()
{
}

void FrameButton::update()
{
    if(_frame)
        _frame->repaint(_geometry);
}

void FrameButton::moveEvent(const MoveEvent& mev)
{
    _geometry.setOrigin( mev.position() );
}

void FrameButton::resizeEvent(const ResizeEvent& rev)
{
    _geometry.setSize( rev.size() );
}

void FrameButton::enterEvent(const EnterEvent& eev)
{
    _isHighlight = true;
}

void FrameButton::leaveEvent(const LeaveEvent& lev)
{
    _isHighlight = false;

    if(_isPressed)
    {
        _isPressed = false;
        update();
    }
}

void FrameButton::mouseEvent(const MouseEvent& mev)
{
    bool isPressed = mev.isPress() || (_isPressed && mev.isPressed());

    if(_isPressed != isPressed)
        update();

    bool wasPressed = _isPressed;
    _isPressed = isPressed;

    if( wasPressed && mev.isRelease() )
        _clicked.send();
}

void FrameButton::touchEvent(const TouchEvent& tev)
{
    bool isPressed = tev.isPress() || tev.isMove();

    if(_isPressed != isPressed)
        update();

    bool wasPressed = _isPressed;
    _isPressed = isPressed;

    if( wasPressed && tev.isRelease() )
        _clicked.send();
}

void FrameButton::paint(gfx::Graphics& gf)
{
    if (_isHighlight)
    {
        gf.setPen(gfx::Color(0, (int)SystemColors::ColorType::ControlFrame).getColor());
        gf.drawRect(geometry());
    }
}

MinimizeButton::MinimizeButton()
{
}

MinimizeButton::~MinimizeButton()
{
}

void MinimizeButton::paint(gfx::Graphics& gf)
{
    FrameButton::paint(gf);

    int inset = 3;
    int height = 2;

    gfx::Rect frameSymbol(geometry().left() + inset,
        geometry().right() - inset,
        geometry().bottom() - inset - height,
        geometry().bottom() - inset);

    
    
    gf.setBrush(parent()->colors().aControlDark());
    gf.fillRect(frameSymbol);
}

// MaximizeButton
MaximizeButton::MaximizeButton()
{    
}

MaximizeButton::~MaximizeButton()
{
}

void MaximizeButton::paint(gfx::Graphics& gf)
{
    FrameButton::paint(gf);


    int inset = 3;

    gfx::Rect frameSymbol(geometry().left() + inset,
        geometry().right() - inset,
        geometry().top() + inset,
        geometry().bottom() - inset);

    gfx::Pen pen(parent()->colors().aControlDark(), 1, gfx::Pen::Solid, gfx::Pen::SquareCap, gfx::Pen::MiterJoin);
    gf.setPen(pen);
    gf.drawRect(frameSymbol);

    frameSymbol.setHeight(2);

    gf.setBrush(gfx::Color(0, (int)SystemColors::ColorType::ControlDark).getColor());
    gf.fillRect(frameSymbol);
}

// CloseButton
CloseButton::CloseButton()
{

}

CloseButton::~CloseButton()
{
}

void CloseButton::paint(gfx::Graphics& gf)
{
    FrameButton::paint(gf);

    int margin = 3;
    int offset = 1;
    int inset = 1;

    const gfx::Rect& buttonRect = geometry();

    gfx::Point line1[] = { gfx::Point(buttonRect.topLeft().x() + margin + inset,
                                         buttonRect.topLeft().y() + offset + margin + inset),

                            gfx::Point(buttonRect.topLeft().x() + offset + margin + inset,
                                         buttonRect.topLeft().y() + margin + inset),

                            gfx::Point(buttonRect.bottomRight().x() - margin - inset,
                                         buttonRect.bottomRight().y() - offset - margin - inset),

                            gfx::Point(buttonRect.bottomRight().x() - offset - margin - inset,
                                         buttonRect.bottomRight().y() - margin - inset) };

    gfx::Point line2[] = { gfx::Point(buttonRect.topRight().x() - offset - margin - inset,
                                         buttonRect.topRight().y() + margin + inset),

                            gfx::Point(buttonRect.topRight().x() - margin - inset,
                                         buttonRect.topRight().y() + offset + margin + inset),

                            gfx::Point(buttonRect.bottomLeft().x() + offset + margin + inset,
                                         buttonRect.bottomLeft().y() - margin - inset),

                            gfx::Point(buttonRect.bottomLeft().x() + margin + inset,
                                         buttonRect.bottomLeft().y() - offset - margin - inset) };

    gf.setBrush(parent()->colors().aControlDark());
    gf.fillPolygon(line1, 4);
    gf.fillPolygon(line2, 4);
}

// MenuButton
MenuButton::MenuButton()
: _rm(nullptr)
{
    //setColor(gfx::Color(0, (int)SystemColors::ColorType::ControlDark).getColor());
}

MenuButton::~MenuButton()
{
}

void MenuButton::paint(gfx::Graphics& gf)
{
    const gfx::Pixmap& ico = this->icon(); 

    if(ico.empty())
    {
        FrameButton::paint(gf);

        int pixelWidth = 1;

        double triangleWidth = geometry().height() / 2.0;

        // even number of pixels
        int pixelsPerWidth = Pt::lround(triangleWidth / pixelWidth);

        if (pixelsPerWidth % 2 != 0)
            triangleWidth += pixelWidth;

        double triangleHeight = triangleWidth / 2.0;

        double x = (geometry().width() - triangleWidth) / 2.0;
        x = geometry().x() + x;

        double y = (geometry().height() - triangleHeight) / 2.0;
        y = geometry().y() + (y - 1);

        gfx::Point triangle[4];
        triangle[0] = gfx::Point(x, y);
        triangle[1] = gfx::Point(x + triangleWidth, y);
        triangle[2] = gfx::Point(x + triangleHeight, y + triangleHeight);
        triangle[3] = gfx::Point(x, y);

        gfx::Brush brush(parent()->colors().aControlDark());
        gf.setBrush(brush);
        gf.fillPolygon(triangle, 3);
    }
    else
    {
        FrameButton::paint(gf);

        gf.setCompositionMode(gfx::CompositionMode::Mode::SourceOver);
        gf.drawImage( geometry().topLeft(), ico);
        gf.setCompositionMode(gfx::CompositionMode::Mode::SourceCopy);
    }
    //painter.setPen(gfx::Color::fromRgb8(255, 0, 0));
    //painter.drawPolyline(triangle, 4);
}

}}}

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
#ifndef WILDCAT_GUI_GFX_IMAGEGRAPHICS_H
#define WILDCAT_GUI_GFX_IMAGEGRAPHICS_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Graphics.h>
#include <wildcat/gui/gfx/Rect.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <Pt/System/Path.h>

namespace wildcat {
namespace gui { 
namespace gfx {

class Rasterizer;

class WILDCAT_GUI_GFX_API PixmapGraphics : public Graphics
{
    public:
        PixmapGraphics();

        PixmapGraphics(const Size& size, std::size_t stride = 0);

        virtual ~PixmapGraphics();

        void reset(const Pixmap& image);
        
        void set(const Pixmap& image)
        {
            reset(image);
        }
        
        void reset(const Size& size, std::size_t stride = 0);

        virtual const Pixmap& image() const;

        Pixmap& image();


        void resize(const Size& size);

        bool empty() const
        {
            return _image.empty();
        }

        void clear(const gfx::ArgbColor& color = gfx::ArgbColor(1, 1, 1));

        virtual const Size& onSize() const;    
    
        virtual void setClip(const Rect& clip);

        virtual const Rect& clip() const
        {
            return _clip;
        }

        virtual void resetClip();

        virtual void setCompositionMode(const CompositionMode& mode);

        virtual CompositionMode compositionMode() const
        {
            return _compMode;
        }

        

        virtual void setPen(const Pen& pen);
        
        virtual const Pen& pen() const
        {
            return _pen;
        }


        virtual void setBrush(const Brush& brush);

        virtual const Brush& brush() const
        {
            return _brush;
        }



        virtual void setFont(const Font& font);


        virtual const Font& font() const
        {
            return _font;
        }

        virtual FontMetrics fontMetrics(const Pt::String& text) const;

        virtual void drawLine(const Point& from, const Point& to);

        virtual void drawText(const Point& to, const Pt::String& Text);

        
        virtual void drawText(const Point& to, const Pt::String& text, const Transform& trans);


        virtual void drawText(const Point& to, const Pt::String& text, const ArgbColor& highlight);

        virtual void drawText(const Point& to, const Pt::String& text, const Transform& trans, const ArgbColor& highlight);
        


        virtual void drawRect(const Rect& rectangle);

        virtual void fillRect(const Rect& rectangle);

        virtual void drawEllipse(const Point& topLeft, const Size& size);

        virtual void fillEllipse(const Point& topLeft, const Size& size);

        virtual void drawPolyline(const Point* points, size_t pointCount);

        virtual void fillPolygon(const Point* points, size_t pointCount);

        virtual void drawImage(const Point& to, const Pixmap& image);

        virtual void drawImage(const Point& to, const Pixmap& image, const Rect& imgRect);

        virtual void drawChord(const Point& topLeft, const Size& size, float degBegin, float degEnd);

        virtual void fillChord(const Point& topLeft, const Size& size, float degBegin, float degEnd);

        virtual void drawPie(const Point& topLeft, const Size& size, float degBegin, float degEnd);

        virtual void fillPie(const Point& topLeft, const Size& size, float degBegin, float degEnd);

        virtual void drawArc(const Point& topLeft, const Size& size, float degBegin, float degEnd);        


    private:
        Pixmap _image;
        Rasterizer*   _rasterizer;
        Size         _size;           
        Rect _clip;
        Pen _pen;
        Brush _brush;
        CompositionMode _compMode;
        Font _font;

};

}}}

#endif


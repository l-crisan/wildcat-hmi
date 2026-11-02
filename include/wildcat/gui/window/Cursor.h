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
#ifndef WILDCAT_GUI_WINDOW_CURSOR_H
#define WILDCAT_GUI_WINDOW_CURSOR_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/base/Resource.h>
#include <Pt/Types.h>
#include <vector>

namespace wildcat {
namespace gui {
namespace window {

class WILDCAT_GUI_WINDOW_API Cursor : public wildcat::gui::base::Resource
{
    friend WILDCAT_GUI_WINDOW_API std::ostream& operator<<(std::ostream& os, const Cursor& obj);

    friend WILDCAT_GUI_WINDOW_API std::istream& operator>>(std::istream& is, Cursor& obj);

    public:
        Cursor();                

        Cursor(const Cursor& copy );

        virtual ~Cursor();
            
        const std::vector<uint8_t>& andRgb888() const 
        {
            return _andMask;
        }
        
        const std::vector<uint8_t>& xorRgb888() const 
        {
            return _xorMask;
        }

        int width() const
        {
            return _width;
        }

        int height() const
        {
            return _height- _spanHeight;
        }

        int spanHeight() const
        {
            return _height;
        }

        int xHotspot() const
        {
            return _xHotspot;
        }

        int yHotspot() const
        {
            return _yHotspot;
        }
    
        void setXHotspot(int v)
        {
            _xHotspot = v;
        }

        gfx::Point hotSpot() const
        {
            return gfx::Point(_xHotspot, _yHotspot);
        }

        void setHotSpot(const gfx::Point& point)
        {
            _xHotspot = point.x();
            _yHotspot = point.y();  
        }

        void setYHotspot(int v)
        {
            _yHotspot = v;
        }

        bool empty() const
        {
          return _andMask.empty() || _width == 0 || _height == 0;
        }

        void clear();

        void fromImage(const gfx::Pixmap& image);

        void loadCursor(const char* xpmFile);

        void loadCursor(std::istream& xpmStream, bool raw = false);

        void loadCursor(const char* xpmBuffer[]);

        void loadCursor(char* xpmBuffer[]);

        bool operator == (const Cursor& c) const;
        
        bool operator != (const Cursor& c) const;

        void operator=(const Cursor& copy);
        
        const gfx::Pixmap& pixmap() const;   

    public:
        static Cursor* createItemCursor(const Pt::String& text);

    private:
        void update();

    protected:
        virtual void onSave(std::ostream& os) const
        {
            os << *this;
        }

        virtual void onLoad(std::istream& is)
        {
            is >> *this;
            update();
        }


    private:
        std::vector<uint8_t> _andMask;
        std::vector<uint8_t> _xorMask;        
        int _width;
        int _height;
        int _xHotspot;
        int _yHotspot;
        uint64_t _dataHash; 
        int _spanHeight;    
        gfx::Pixmap _pixmap;

    private:
        static wildcat::gui::base::TypeInit<Cursor> _typeInit;
};

WILDCAT_GUI_WINDOW_API std::ostream& operator<<(std::ostream& os, const Cursor& obj);

WILDCAT_GUI_WINDOW_API std::istream& operator>>(std::istream& is, Cursor& obj);


}}}

#endif


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
#ifndef WILDCAT_GUI_GFX_PIXMAP_H
#define WILDCAT_GUI_GFX_PIXMAP_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Rect.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Color.h>
#include <wildcat/gui/gfx/CompositionMode.h>
#include <wildcat/gui/base/Resource.h>
#include <wildcat/gui/base/TypeFactory.h>
#include <ostream>

namespace wildcat {
namespace gui {
namespace gfx {

enum class ImageFileType
{
    Unknown,
    Jpeg,
    Png,
    Bmp,
    Xpm,
    PngBase64,
    Ico
};

class Pixmap;

WILDCAT_GUI_GFX_API std::ostream& operator<<(std::ostream& os, const Pixmap& obj);

WILDCAT_GUI_GFX_API std::istream& operator>>(std::istream& is, Pixmap& obj);

WILDCAT_GUI_GFX_API ImageFileType fromFileExt(const std::string& dileName);

class WILDCAT_GUI_GFX_API Pixmap : public base::Resource
{
    public:

        Pixmap();

        Pixmap(const Pixmap& copy);        

        Pixmap(const uint8_t* buffer, const Size& s, int padding = 0);

        Pixmap(uint8_t* buffer, const Size& s, int padding = 0);

        Pixmap(const Size& s, int padding = 0);        

        virtual ~Pixmap();


        void reset();

        void reset(uint8_t* buffer, const Size& s, size_t padding = 0);

        void reset(const uint8_t* buffer, const Size& s, size_t padding = 0);


        void resize(const Size& s, size_t padding = 0);
      
        void  invert();

        ArgbColor color(int offset) const;

        ArgbColor color(const Point& p) const ;

        void setColor(const ArgbColor& c, const Point& pixel, CompositionMode m = CompositionMode::SourceCopy);

        void setColorSpan(const Pixmap& pixmap, const Point& pixel, size_t lenght, CompositionMode m = CompositionMode::SourceCopy);


        uint32_t* pixel(int offset);

        uint32_t* pixel(const Point& p);

        const uint32_t* pixel(int offset) const;

        const uint32_t* pixel(const Point& p) const;


        const Size& size() const
        {
            return _data->_size;
        }

        size_t width() const
        {
            return _data->_size.width();
        }

        size_t height() const
        {
            return _data->_size.height();
        }

        size_t padding() const
        {
            return _data->_padding;
        }

        void setPadding(int v)
        {
            detach();
            _data->_padding = v;
        }

        void loadXpm(const char** xpmData);

        void loadXpm(char** xpmData);
        
        void load(const char* file, ImageFileType ftype);

        void load(std::istream& stream, ImageFileType ftype);

        void save(const char* file, ImageFileType ftype) const;

        void save(std::ostream& stream, ImageFileType ftype) const;

        std::string toBase64() const;

        void fromBase64(const std::string& s);

        bool empty() const
        {
            return _data->_size.width() == 0 || _data->_size.height() == 0;
        }

        size_t byteSize() const;

        size_t offset(int x, int y) const;

        size_t offset(const Point& p) const;

        size_t lineSize() const
        {
            return (_data->_size.width() * 4 + _data->_padding);
        }

        const uint8_t* data() const
        {
            return _data->_buffer;
        }

        uint8_t* data()
        {
            detach();
            return _data->_buffer;
        }


        uint32_t* begin()
        {
            detach();
            return (uint32_t*)_data->_buffer;
        }

        const uint32_t* begin() const
        {
            return (const uint32_t*)_data->_buffer;
        }

        uint32_t* end()
        {
            detach();
            uint32_t* p = (uint32_t*)_data->_buffer;

            return p + ((_data->_size.width() + _data->_padding *4 )  * _data->_size.height());
        }

        const uint32_t* end() const
        {
            const uint32_t* p = (const uint32_t*)_data->_buffer;
            return p + ((_data->_size.width() + _data->_padding * 4) * _data->_size.height());
        }

        static constexpr size_t pixelStride()
        {
            return 4;
        }

        Pixmap toImage(const Rect& rect) const;    

        void operator=(const Pixmap& copy);

        void copy(const Point& to, const Pixmap& fromBuffer,  const Point& from, size_t length, CompositionMode mode);

        void copy(const Rect& to,  const Pixmap& fromBuffer, const Rect& from , CompositionMode mode);

        void detach();
    public:
        static void sourceOver(uint8_t* to, const uint8_t* from);

        static ImageFileType fromFileExt(const std::string& fileName);        

    protected:

        virtual void onSave(std::ostream& os) const
        {
            os << *this;
        }

        virtual void onLoad(std::istream& is)
        {
            is >> *this;
        }

    private:
        class Data
        {
            public:
                Data();

                virtual ~Data();

                int refCount;   
                uint8_t* _buffer;
                Size _size;
                size_t _padding;
                bool _owner;
        };

    private:
        void release();

    private:
        Data* _data;

    private:
        static wildcat::gui::base::TypeInit<Pixmap> _typeInit;
};

}}}

#endif

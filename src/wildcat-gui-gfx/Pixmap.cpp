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
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/gfx/PngReader.h>
#include <wildcat/gui/gfx/PngWriter.h>
#include <wildcat/gui/gfx/JpegReader.h>
#include <wildcat/gui/gfx/JpegWriter.h>
#include <wildcat/gui/gfx/BmpReader.h>
#include <wildcat/gui/gfx/BmpWriter.h>
#include <wildcat/gui/gfx/XpmReader.h>
#include <wildcat/gui/gfx/IcoReader.h>
#include <wildcat/gui/base/File.h>
#include <wildcat/gui/base/String.h>
#include <Pt/System/Path.h>
#include <fstream>
#include <iostream>
#include <sstream>

namespace wildcat {
namespace gui {
namespace gfx {

wildcat::gui::base::TypeInit<Pixmap> _typeInit("wildcat.gfx.Pixmap");

void Pixmap::sourceOver(uint8_t* to, const uint8_t* from)
{
    uint32_t* pixel = (uint32_t*)to;

    const uint32_t src = *(const uint32_t*)from;
    const uint32_t dst = *pixel;

    const uint32_t sa = src >> 24;

    if (sa == 0)
        return;

    if (sa == 255)
    {
        *pixel = src;
        return;
    }

    const uint32_t da = dst >> 24;
    const uint32_t one_minus_sa = 255 - sa;

    // Schneller Pfad: opakes Ziel (haeufigster Fall)
    if (da == 255)
    {
        const uint32_t rb = ((src & 0x00FF00FF) * sa + (dst & 0x00FF00FF) * one_minus_sa) >> 8;
        const uint32_t g  = (((src >> 8) & 0xFF) * sa + ((dst >> 8) & 0xFF) * one_minus_sa) >> 8;

        *pixel = 0xFF000000 | (rb & 0x00FF00FF) | ((g & 0xFF) << 8);
        return;
    }

    // Allgemeiner Fall: Ziel ist (teil-)transparent, straight alpha
    // Anteil des Hintergrunds, der nach dem Ueberblenden sichtbar bleibt
    const uint32_t db = (da * one_minus_sa + 127) / 255;
    const uint32_t oa = sa + db;

    if (oa == 0)
    {
        *pixel = 0;
        return;
    }

    const uint32_t sr = (src >> 16) & 0xFF;
    const uint32_t sg = (src >> 8) & 0xFF;
    const uint32_t sb = src & 0xFF;

    const uint32_t dr = (dst >> 16) & 0xFF;
    const uint32_t dg = (dst >> 8) & 0xFF;
    const uint32_t dbl = dst & 0xFF;

    const uint32_t r = (sr * sa + dr * db) / oa;
    const uint32_t g = (sg * sa + dg * db) / oa;
    const uint32_t b = (sb * sa + dbl * db) / oa;

    *pixel = (oa << 24) | (r << 16) | (g << 8) | b;
}

Pixmap::Data::Data()
: refCount(0)
, _buffer(nullptr)
, _size()
, _padding(0)
, _owner(true)
{
}


Pixmap::Data::~Data()
{
    if (_owner)
        delete[] _buffer;
}

Pixmap::Pixmap()
: _data(new Data())
{
}


Pixmap::Pixmap(const Pixmap& copy)
:_data(copy._data)
{
    _data->refCount++;
    setName(copy.name());
}

Pixmap::Pixmap(const uint8_t* buffer, const Size& s, int padding)
:_data(new Data())
{    
    _data->_size = s;
    _data->_padding = padding;

    const size_t bs = byteSize();

    _data->_buffer = new uint8_t[bs];

    memcpy(_data->_buffer, buffer, bs);
}

Pixmap::Pixmap(uint8_t* buffer, const Size& s, int padding)
: _data(new Data())
{
    _data->_buffer = buffer;
    _data->_size = s;
    _data->_padding = padding;
    _data->_owner = false;
}

Pixmap::Pixmap(const Size& s, int padding)
: _data(new Data())
{
    _data->_size = s;
    _data->_padding = padding;

    const size_t bs = byteSize();

    _data->_buffer = new uint8_t[bs];
    
    memset(_data->_buffer, 0, bs);
}

Pixmap::~Pixmap()
{
    release();
}

void Pixmap::release()
{
    if (_data->refCount > 0)
        _data->refCount--;
    else
        delete _data;

    _data = nullptr;
}

size_t Pixmap::byteSize() const
{
    return (_data->_size.width() *  4 + _data->_padding) * _data->_size.height();
}

void Pixmap::detach()
{    
    if( _data->refCount == 0)
        return;
    
    Data* newData = new Data();

    const size_t bs = byteSize();

    if(_data->_owner)
    {
        newData->_buffer = new uint8_t[bs];

        memcpy(newData->_buffer, _data->_buffer, bs);        
    }
    else
    {
        newData->_buffer = _data->_buffer;       
    }

    newData->_size    = _data->_size;
    newData->_padding = _data->_padding;
    newData->_owner   = _data->_owner;

    _data->refCount--;
    _data = newData;
}


void Pixmap::reset()
{
    detach();

    if (_data->_owner)
        delete[] _data->_buffer;

    _data->_owner = true;
    _data->_buffer = nullptr;
    _data->_size.set(0, 0);
    _data->_padding = 0;
}

void Pixmap::reset(uint8_t* buffer, const Size& s, size_t padding)
{
    detach();

    if (_data->_owner)
        delete[] _data->_buffer;

    _data->_owner = false;
    _data->_size = s;

    _data->_padding = padding;

    _data->_buffer = buffer;
}

void Pixmap::reset(const uint8_t* buffer, const Size& s, size_t padding)
{
    detach();

    if( _data->_owner)
        delete[] _data->_buffer;
        
    _data->_size = s;

    _data->_padding = padding;

    const size_t bs = byteSize();

    _data->_owner = true;

    _data->_buffer = new uint8_t[bs];
    memcpy(_data->_buffer, buffer, bs);
}

void Pixmap::invert()
{
    for (size_t h = 0; h < height(); ++h)
    {
        for( size_t w = 0; w < width();  ++w)
        {   
            const gfx::Point pos(w, h);

            ArgbColor c = color(pos);

            c.invert();
            setColor(c, pos);
        }
    }
}

void Pixmap::resize(const Size& s, size_t padding)
{
    detach();
    if (_data->_owner)
        delete[] _data->_buffer;

    _data->_size = s;

    _data->_padding = padding;
    const size_t bs = byteSize();

    _data->_owner = true;
    _data->_buffer = new uint8_t[bs];

    memset(_data->_buffer, 0, bs);
}

size_t Pixmap::offset(const Point& p) const
{
    return offset(p.x(), p.y());
}

size_t Pixmap::offset(int x, int y ) const
{    
    return ((_data->_size.width() * sizeof(uint32_t) + _data->_padding) * y) + (x * sizeof(uint32_t));
}

ArgbColor Pixmap::color(int offset) const
{
    return ArgbColor(*(uint32_t*)&_data->_buffer[offset]);
}

ArgbColor Pixmap::color(const Point& p)const
{
    return ArgbColor(*(uint32_t*)&_data->_buffer[offset(p)]);
}

void Pixmap::setColor(const ArgbColor& c, const Point& pixel, CompositionMode m)
{    
    detach();

    if( m == CompositionMode::SourceOver)
    {
        const uint32_t alphaSrc = c.alpha();

        if (alphaSrc != 255)
        {           
            const uint32_t from = c.color();

            sourceOver(&_data->_buffer[offset(pixel)], (const uint8_t*)&from);
            return;
        }
    }
                
    uint32_t* p = (uint32_t*)&_data->_buffer[offset(pixel)];
    *p = c.color();
}

void Pixmap::setColorSpan(const Pixmap& pixmap, const Point& pixel, size_t lenght, CompositionMode m)
{        
    detach();

    if (m == CompositionMode::SourceOver)
    {
        const ArgbColor c(*pixmap.begin());

        const uint32_t alphaSrc = c.alpha();
       
        if (alphaSrc != 255)
        {
            Point pos = pixel;

            const uint32_t from = c.color();

            for (size_t i = 0; i < lenght; ++i)
            {            
                sourceOver(&_data->_buffer[offset(pos)], (const uint8_t*)&from);
                pos.addX(1);
            }

            return;
        }
    }

    uint32_t* p = (uint32_t*)&_data->_buffer[offset(pixel)];    
    memcpy(p, pixmap.begin(), lenght * pixelStride());
}

uint32_t* Pixmap::pixel(int offset)
{
    detach();
    return (uint32_t*)&_data->_buffer[offset];
}

uint32_t* Pixmap::pixel(const Point& p)
{
    detach();
    return (uint32_t*)&_data->_buffer[offset(p)];
}

const uint32_t* Pixmap::pixel(int offset) const
{
    return (uint32_t*)&_data->_buffer[offset];
}

const uint32_t* Pixmap::pixel(const Point& p) const
{
    return (uint32_t*)&_data->_buffer[offset(p)];
}

void Pixmap::load(const char* file, ImageFileType ftype)
{    
    std::ifstream is(file, std::ios::binary);
    load(is, ftype);
    is.close();
}

void Pixmap::loadXpm(const char** xpmData)
{
    detach();
    XpmReader reader(*this);
    reader.read(xpmData);
}

void Pixmap::loadXpm(char** xpmData)
{
    detach();
    XpmReader reader(*this);
    reader.read((const char**)xpmData);
}

void Pixmap::load(std::istream& stream, ImageFileType ftype)
{
    detach();
    switch (ftype)
    {
        case ImageFileType::Ico:
        {
            Icon  icon;
            IcoReader reader(stream, icon);    
            reader.read();
            *this = icon.getImage();    
        }
        break;

        case ImageFileType::Jpeg:
        {
            JpegReader reader(stream, *this);
            reader.read();
        }
        break;

        case ImageFileType::Png:
        {
            PngReader reader(stream, *this);
            reader.read();
        }
        break;

        case ImageFileType::PngBase64:
        {
            std::stringstream pngStream;

            base::String::fromBase64(stream, pngStream);

            PngReader reader(pngStream, *this);

            reader.read();
        }
        break;


        case ImageFileType::Bmp:
        {
            BmpReader reader(stream, *this);
            reader.read();
        }
        break;

        case ImageFileType::Xpm:
        {
            XpmReader reader( *this);
            reader.read(stream);
        }
    }
}

ImageFileType Pixmap::fromFileExt(const std::string& fileName)
{
    Pt::System::Path path(fileName.c_str());
    std::string str = path.extension().narrow();

    str = base::String::toLower(str); ;

    if (str == "jpg" || str == "jpeg")
        return ImageFileType::Jpeg;
    else if (str == "png")
        return ImageFileType::Png;
    else if (str == "bmp")
        return ImageFileType::Bmp;
    else if (str == "xpm")
        return ImageFileType::Xpm;
    else if (str == "ico")
        return ImageFileType::Ico;

    return ImageFileType::Unknown;
}


void Pixmap::save(const char* file, ImageFileType ftype) const
{
    std::ofstream os(file, std::ios::binary | std::ios::trunc);

    save(os, ftype);

    os.flush();
    os.close();
}

void Pixmap::save(std::ostream& stream, ImageFileType ftype) const
{
    switch (ftype)
    {
        case ImageFileType::Jpeg:
        {
            JpegWriter writter(stream);

            writter.write(*this);
        }
        break;

        case ImageFileType::Png:
        {
            PngWriter writer(stream);
            writer.write(*this);
        }
        break;

        case ImageFileType::PngBase64:
        {
            std::stringstream binStream;

            PngWriter writer(binStream);

            writer.write(*this);

            binStream.seekg(0, std::ios::beg);

            base::String::toBase64(binStream, stream);

        }
        break;

        case ImageFileType::Bmp:
        {
            BmpWriter writer(stream);
            writer.write(*this);
        }
        break;
    }
}

std::string Pixmap::toBase64() const
{
    std::stringstream ss;

    save(ss, ImageFileType::PngBase64);

    ss.flush();

    return ss.str();
}

void Pixmap::fromBase64(const std::string& s)
{
    std::stringstream ss(s);

    load(ss, ImageFileType::PngBase64);
}

Pixmap Pixmap::toImage(const Rect& r) const
{   
    const gfx::Rect rectToCopy = r.intersect(gfx::Rect(gfx::Point(0, 0), size()));

    Pixmap sub(r.size());
    
    const int lineSize = rectToCopy.width() * pixelStride();    
    const int toX = rectToCopy.x() - r.x();

    int toY = rectToCopy.y() -r.y();

    for(int y = rectToCopy.y(); y <= rectToCopy.bottom(); ++y, ++toY)
    {
        const uint32_t* src = pixel(Point(rectToCopy.x(), y));
        uint32_t*       dst = sub.pixel(Point(toX, toY));

        memcpy(dst, src, lineSize);
    }

    return sub;
}

void Pixmap::copy(const Point& to, const Pixmap& fromBuffer, const Point& from, size_t length, CompositionMode mode)
{
    detach();
    switch (mode)
    {
        default:
        case CompositionMode::SourceCopy:
            memcpy(pixel(to), fromBuffer.data(), length * 4);
        break;

        case CompositionMode::SourceOver:
        {
            const uint8_t* src = fromBuffer.data();
            uint8_t* dst = (uint8_t*)pixel(to);

            for (size_t i = 0; i < length; ++i)
            {
                sourceOver(dst, src);
                src += 4;
                dst += 4;
            }
        }
        break;
    }
}

void Pixmap::copy(const Rect& to, const Pixmap& fromBuffer, const Rect& fromRect, CompositionMode mode)
{
    detach();
    const int pixelSize = pixelStride();

    Point toPoint = to.topLeft();

    // TODO: equals to toInfo.pitch()
    int toStride = (width() * pixelSize) + padding();
    int fromStride = (fromBuffer.width() * pixelSize) + padding();

    int toBegin = (toPoint.y() * toStride) + (toPoint.x() * pixelSize);
    int fromBegin = (fromRect.y() * fromStride) + (fromRect.x() * pixelSize);

    uint8_t* toLine = data() + toBegin;
    const uint8_t* fromLine = fromBuffer.data() + fromBegin;

    switch (mode)
    {
        default:
        case CompositionMode::SourceCopy:
        {
            int n = fromRect.width() * pixelSize;

            for (int y = 0; y < fromRect.height(); ++y)
            {
                memcpy(toLine, fromLine, n);

                toLine += toStride;
                fromLine += fromStride;
            }

            break;
        }

        case CompositionMode::SourceOver:
        {
            for (int y = 0; y < fromRect.height(); ++y)
            {
                uint8_t* to = toLine;
                const uint8_t* from = fromLine;

                for (int x = 0; x < fromRect.width(); ++x)
                {
                    sourceOver(to, from);                    
                    to += pixelSize;
                    from += pixelSize;
                }

                toLine += toStride;
                fromLine += fromStride;
            }

            break;
        }
    }    
}

void Pixmap::operator=(const Pixmap& copy)
{
    if (this == &copy || _data == copy._data)
        return;

    release();

    _data = copy._data; 
    _data->refCount++;
    setName(copy.name());
}

WILDCAT_GUI_GFX_API std::ostream& operator<<(std::ostream& os, const Pixmap& obj)
{
    os << '(';
    os << obj.name() << ',';

    if (!obj.empty())
    {
        base::String::write(os, obj.toBase64(), ')');
    }
    else
    {
        os << ')';
    }

    os.flush();
    return os;
}

WILDCAT_GUI_GFX_API  std::istream& operator>>(std::istream& is, Pixmap& obj)
{
    char ch;

    is >> ch;
    std::string name;
    std::getline(is, name, ',');
    obj.setName(name);

    const std::string data = base::String::read(is, ')');

    obj.reset();

    if (!data.empty())
        obj.fromBase64(data);

    return is;
}

}}}


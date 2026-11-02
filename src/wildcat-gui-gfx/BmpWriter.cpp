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
#include <wildcat/gui/gfx/BmpWriter.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/gfx/Size.h>
#include "BmpHeader.h"
#include <Pt/IOError.h>
#include <iostream>
#include <ios>
#include <cstdlib>

namespace wildcat {
namespace gui {
namespace gfx {

class BmpWriterImpl
{
    public:
        BmpWriterImpl();

        BmpWriterImpl(std::ostream& stream);
       
        ~BmpWriterImpl();

        void attach(std::ostream& stream);

        void detach();

        void reset();

        void write(const Pixmap& image);

        void beginWrite(const Pixmap& image);

        bool advance();

    private:
        void writeHeader(const Pixmap& img);

    private:
        std::ostream* _target;
        const Pixmap* _image;
        PixmapHeader _header;
        int _row;
};

BmpWriterImpl::BmpWriterImpl()
: _target(0)
, _image(0)
{
    memset(&_header,0, sizeof(_header));
}

BmpWriterImpl::BmpWriterImpl(std::ostream& stream)
: _target(&stream)
, _image(0)
{
    memset(&_header, 0, sizeof(_header));
}

BmpWriterImpl::~BmpWriterImpl()
{

}

void BmpWriterImpl::attach(std::ostream& stream)
{
    _target = &stream;    
}

void BmpWriterImpl::detach()
{
    
}

void BmpWriterImpl::reset()
{
    if( _image)
        _row  = _image->height() - 1;
}


bool BmpWriterImpl::advance()
{
    if( _row < 0)
        return false;

    std::size_t i = 0;
    std::vector<char> line(_image->width() * 3);

    for (std::int32_t x = 0; x < _image->width(); ++x)
    {
        ArgbColor color = _image->color(gfx::Point(x, _row));


        line[i++] = color.blue();
        line[i++] = color.green();
        line[i++] = color.red();
    }

    _target->write(reinterpret_cast<const char*>(line.data()), line.size());

    _row--;

    return true;
}


void BmpWriterImpl::writeHeader(const Pixmap& img)
{
    _image = &img;
    const std::int32_t row_size = _image->width() * 3 + _image->width() % 4;
    const std::uint32_t Pixmap_size = row_size * _image->height();

    // Construct Pixmap header
    memset(&_header, 0, sizeof(_header));

    /* Pixmap file header structure */
    _header.magic = Pixmap_BUFFER_MAGIC;
    _header.file_size = Pixmap_size + sizeof(PixmapHeader);
    _header.reserved1 = 0;
    _header.reserved2 = 0;
    _header.offset_bits = sizeof(PixmapHeader);
    /* Pixmap file info structure */
    _header.size = 40;
    _header.width = _image->width();
    _header.height = _image->height();
    _header.planes = 1;
    _header.bits_per_pixel = 3 * 8; // 24bpp
    _header.compression = 0;
    _header.size_image = Pixmap_size;
    _header.x_pixels_per_meter = 0;
    _header.y_pixels_per_meter = 0;
    _header.clr_used = 0;
    _header.clr_important = 0;

    _target->write(reinterpret_cast<const char*>(&_header), sizeof(_header));
    _row = _image->height() - 1;
}

void BmpWriterImpl::write(const Pixmap& image)
{
    writeHeader(image);
    std::vector<char> line(_image->width() * 3 + _image->width() % 4);

    for (std::int32_t y = _image->height() - 1; y >= 0; --y) 
    {
        std::size_t i = 0;
        for (std::int32_t x = 0; x < _image->width(); ++x) 
        {
            ArgbColor color = _image->color(gfx::Point(x, y));


            line[i++] = color.blue();
            line[i++] = color.green();
            line[i++] = color.red();
        }
        _target->write(reinterpret_cast<const char*>(line.data()), line.size());
    }
}


void BmpWriterImpl::beginWrite(const Pixmap& image)
{
    writeHeader(image);
}



BmpWriter::BmpWriter()
: ImageWriter()
, _impl( new BmpWriterImpl() )
{
}


BmpWriter::BmpWriter(std::ostream& stream)
: ImageWriter(stream)
, _impl( new BmpWriterImpl(stream) )
{
}

BmpWriter::~BmpWriter()
{
  delete _impl;
}


void BmpWriter::onAttach()
{
    _impl->attach(*_stream);
}


void BmpWriter::onDetach()
{
    _impl->detach();
}


void BmpWriter::onReset()
{
    _impl->reset();
}


bool BmpWriter::onAdvance()
{
    return _impl->advance();
}


void BmpWriter::onWrite(const Pixmap& image)
{
    _impl->write(image);
}


void BmpWriter::onBeginWrite(const Pixmap& image)
{
    _impl->beginWrite(image);
}

}}}

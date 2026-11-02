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
#include <wildcat/gui/gfx/BmpReader.h>
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


class BmpReaderImpl
{
    public:
        BmpReaderImpl();

        BmpReaderImpl(std::istream& is, Pixmap& image);
       
        ~BmpReaderImpl();

        void attach(std::istream& is, Pixmap& image);

        void detach();

        void reset();

        Pixmap& get();

        Pixmap* advance();

    private:
        ArgbColor readNextPixel();

        void readHeader();

        void readLine(int y);

    private:
        std::istream* _target;
        Pixmap* _image;
        PixmapHeader _header;
        int _bits;
        bool _init;
        size_t _row;
        std::vector<char> _strike;
};


BmpReaderImpl::BmpReaderImpl()
: _target(0)
, _image(0)
, _init(false)
, _row(0)
{
    memset(&_header,0, sizeof(_header));
}

BmpReaderImpl::BmpReaderImpl(std::istream& is, Pixmap& image)
: _target(&is)
, _image(&image)
, _init(false)
, _row(0)
{
    memset(&_header, 0, sizeof(_header));
}

void BmpReaderImpl::readHeader()
{
    _row = 0;

    // Read Header
    memset(&_header, 0, sizeof(_header));

    _target->read(reinterpret_cast<char*>(&_header), sizeof(PixmapHeader));

    // Check if Pixmap file is valid
    if (_header.magic != Pixmap_BUFFER_MAGIC)
        throw std::logic_error("Load Pixmap failed: Unrecognized file format.");

    _bits = _header.bits_per_pixel;
    _image->resize(Size(_header.width, _header.height));
    _target->seekg(_header.offset_bits, std::ios_base::beg);
    _strike.resize(_header.width % 4);
}

BmpReaderImpl::~BmpReaderImpl()
{

}

void BmpReaderImpl::attach(std::istream& is, Pixmap& image)
{
    _target = &is;
    _image = &image;
}

void BmpReaderImpl::detach()
{

}

void BmpReaderImpl::reset()
{

}

ArgbColor BmpReaderImpl::readNextPixel()
{    
    switch (_bits)
    {
        case 16:
        {
            uint16_t data;
            _target->read((char*)&data, sizeof(data));

            uint8_t r = data >> 6+5;
            uint8_t g = data >> 5;
            uint8_t b = (data & 0x1F);
            return ArgbColor::fromRgb8(r,g,b);

        }
        break;

        case 24:
        {
            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t b = 0;

            _target->read((char*)&b, 1);            
            _target->read((char*)&g, 1);
            _target->read((char*)&r, 1);
            

            return ArgbColor::fromRgb8(r, g, b);
        }

        break;

        case 32:
        {
            uint8_t a = 0;
            uint8_t r = 0;
            uint8_t g = 0;
            uint8_t b = 0;
                       
            _target->read((char*)&b, 1);
            _target->read((char*)&g, 1);
            _target->read((char*)&r, 1);
            _target->read((char*)&a, 1);
            
            return ArgbColor::fromRgb8(r, g, b, a);
        }        
        break;

    }

    return ArgbColor();
}

Pixmap& BmpReaderImpl::get()
{
    readHeader();
    
    for (int y = _header.height -1; y > -1; --y)
        readLine(y);


    return *_image;
}

void BmpReaderImpl::readLine(int y)
{
    for (int x = 0; x < _header.width ; ++x)
    {
        ArgbColor c = readNextPixel();
        _image->setColor(c, gfx::Point(x, y));
    }

    if (!_strike.empty())
        _target->read(&_strike[0], _strike.size());
}

Pixmap* BmpReaderImpl::advance()
{
    if (!_init)
    {
        readHeader();
        _init = true;
        return 0;
    }
    
    if (_row < _header.height)
    {
        readLine(_row);
        _row++;

        return 0;
    }

    return _image;
    
}




BmpReader::BmpReader()
: ImageReader()
, _impl( new BmpReaderImpl() )
{
}

BmpReader::BmpReader(std::istream& is, Pixmap& image)
: ImageReader(is, image)
, _impl( new BmpReaderImpl(is, image) )
{
}

BmpReader::~BmpReader()
{
  delete _impl;
}

void BmpReader::onAttach()
{
    _impl->attach(*_stream, *_image);
}

void BmpReader::onDetach()
{
    _impl->detach();
}

void BmpReader::onReset()
{
    _impl->reset();
}

bool BmpReader::onAdvance()
{
    return _impl->advance() != 0;
}

void BmpReader::onRead()
{
    _impl->get();
}

}}}

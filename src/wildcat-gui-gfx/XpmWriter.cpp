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
#include <wildcat/gui/gfx/XpmWriter.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/gfx/Size.h>
#include <Pt/IOError.h>
#include <vector>
#include <iostream>
#include <ios>
#include <cstdlib>
#include <cstdio>

namespace wildcat {
namespace gui {
namespace gfx {

namespace {

const char g_symbols[] =
    " .XoO+@#$%^&*=-;:>,<1234567890qwertyuipasdfghjklzxcvbnmMNBVCZ"
    "ASDFGHJKLPIUYTREWQ!~^/()_`'][{}|";

const std::size_t g_symbolCount = sizeof(g_symbols) - 1;

std::string symbolFor(std::size_t index, int cpp)
{
    std::string code;

    for (int i = 0; i < cpp; ++i)
    {
        code += g_symbols[index % g_symbolCount];
        index /= g_symbolCount;
    }

    return code;
}

}

XpmWriter::XpmWriter()
: _image(0)
, _row(0)
, _cpp(0)
{
}


XpmWriter::XpmWriter(std::ostream& os)
: ImageWriter(os)
, _image(0)
, _row(0)
, _cpp(0)
{
}


XpmWriter::~XpmWriter()
{

}


void XpmWriter::onAttach()
{

}


void XpmWriter::onDetach()
{

}


void XpmWriter::onReset()
{
    _image = 0;
    _row = 0;
    _cpp = 0;
    _colorCodes.clear();
}


void XpmWriter::buildColorTable(const Pixmap& image)
{
    _colorCodes.clear();

    for (std::size_t y = 0; y < image.height(); ++y)
    {
        for (std::size_t x = 0; x < image.width(); ++x)
        {
            uint32_t c = *image.pixel(Point(int(x), int(y)));

            if (_colorCodes.find(c) == _colorCodes.end())
                _colorCodes[c] = "";
        }
    }

    std::size_t colorCount = _colorCodes.size();

    _cpp = 1;

    while (std::size_t(1) << (8 * _cpp) < colorCount && _cpp < 4)
        ++_cpp;

    std::size_t cap = 1;

    for (int i = 0; i < _cpp; ++i)
        cap *= g_symbolCount;

    while (cap < colorCount)
    {
        ++_cpp;
        cap *= g_symbolCount;
    }

    std::size_t index = 0;

    for (std::map<uint32_t, std::string>::iterator it = _colorCodes.begin();
         it != _colorCodes.end(); ++it, ++index)
    {
        it->second = symbolFor(index, _cpp);
    }
}


std::string XpmWriter::codeFor(uint32_t color) const
{
    std::map<uint32_t, std::string>::const_iterator it = _colorCodes.find(color);

    if (it != _colorCodes.end())
        return it->second;

    return std::string(_cpp, ' ');
}


void XpmWriter::onBeginWrite(const Pixmap& image)
{
    if (! _stream || ! _stream->rdbuf())
        return;

    buildColorTable(image);

    _image = &image;
    _row = 0;

    char header[64];

    std::sprintf(header, "%u %u %u %u",
                  (unsigned)image.width(), (unsigned)image.height(),
                  (unsigned)_colorCodes.size(), (unsigned)_cpp);

    *_stream << "/* XPM */" << std::endl;
    *_stream << "static char *image[] = {" << std::endl;
    *_stream << "\"" << header << "\"," << std::endl;

    for (std::map<uint32_t, std::string>::const_iterator it = _colorCodes.begin();
         it != _colorCodes.end(); ++it)
    {
        char hex[16];

        uint32_t argb = it->first;
        uint8_t alpha = uint8_t(argb >> 24);

        if (alpha == 0)
        {
            *_stream << "\"" << it->second << " c None\"," << std::endl;
        }
        else
        {
            std::sprintf(hex, "#%02X%02X%02X",
                          (unsigned)((argb >> 16) & 0xFF),
                          (unsigned)((argb >> 8) & 0xFF),
                          (unsigned)(argb & 0xFF));

            *_stream << "\"" << it->second << " c " << hex << "\"," << std::endl;
        }
    }
}


bool XpmWriter::onAdvance()
{
    if (! _stream || ! _stream->rdbuf() || ! _image || _row >= _image->height())
        return true;

    std::string line;
    line.reserve(_image->width() * _cpp);

    for (std::size_t x = 0; x < _image->width(); ++x)
    {
        uint32_t c = *_image->pixel(Point(int(x), int(_row)));
        line += codeFor(c);
    }

    *_stream << "\"" << line << "\"";

    ++_row;

    if (_row < _image->height())
    {
        *_stream << "," << std::endl;
        return false;
    }

    *_stream << std::endl << "};" << std::endl;

    _image = 0;
    _row = 0;

    return true;
}


void XpmWriter::onWrite(const Pixmap& image)
{
    onBeginWrite(image);

    while (! onAdvance())
        ;
}

}}}

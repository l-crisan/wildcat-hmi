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
#include <wildcat/gui/gfx/IcoReader.h>
#include <wildcat/gui/gfx/Icon.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Color.h>
#include "IcoHeader.h"
#include <istream>
#include <ios>
#include <vector>
#include <cstdint>
#include <cstring>

namespace wildcat {
namespace gui {
namespace gfx {

class IcoReaderImpl
{
    public:
        IcoReaderImpl();

        IcoReaderImpl(std::istream& is, Icon& icon);

        ~IcoReaderImpl();

        void attach(std::istream& is, Icon& icon);

        void detach();

        void reset();

        void read();

    private:
        void readDirectory();

        void readImage(const IcoDirEntry& entry);

        void readDibImage(const IcoDirEntry& entry);

        template <typename T>
        void readPod(T& value)
        {
            _stream->read(reinterpret_cast<char*>(&value), sizeof(T));
        }

    private:
        std::istream* _stream;
        Icon* _icon;
        IcoDir _dir;
        std::vector<IcoDirEntry> _entries;
};


IcoReaderImpl::IcoReaderImpl()
: _stream(0)
, _icon(0)
{
    memset(&_dir, 0, sizeof(_dir));
}

IcoReaderImpl::IcoReaderImpl(std::istream& is, Icon& icon)
: _stream(&is)
, _icon(&icon)
{
    memset(&_dir, 0, sizeof(_dir));
}

IcoReaderImpl::~IcoReaderImpl()
{
}

void IcoReaderImpl::attach(std::istream& is, Icon& icon)
{
    _stream = &is;
    _icon = &icon;
}

void IcoReaderImpl::detach()
{
    _stream = 0;
    _icon = 0;
}

void IcoReaderImpl::reset()
{
    _entries.clear();
    memset(&_dir, 0, sizeof(_dir));
}

void IcoReaderImpl::readDirectory()
{
    memset(&_dir, 0, sizeof(_dir));
    readPod(_dir);

    if (_dir.reserved != 0 || _dir.type != ICO_TYPE_ICON)
        throw std::logic_error("Load Icon failed: Unrecognized file format.");

    _entries.resize(_dir.count);

    for (std::uint16_t i = 0; i < _dir.count; ++i)
        readPod(_entries[i]);
}

void IcoReaderImpl::readDibImage(const IcoDirEntry& entry)
{
    IcoBitmapInfoHeader header;
    memset(&header, 0, sizeof(header));
    readPod(header);

    const int width = header.width;
    // The stored height contains both the XOR (color) and AND (mask) bitmaps.
    const int height = header.height / 2;
    const int bits = header.bitCount;

    // Palette based formats (1, 4 and 8 bits) store a color table right after
    // the header. Each entry is a RGBQUAD stored as blue, green, red, reserved.
    std::vector<ArgbColor> palette;
    if (bits <= 8)
    {
        int paletteSize = static_cast<int>(header.clrUsed);
        if (paletteSize == 0)
            paletteSize = 1 << bits;

        palette.resize(paletteSize);
        for (int i = 0; i < paletteSize; ++i)
        {
            unsigned char entry[4] = { 0, 0, 0, 0 };
            _stream->read(reinterpret_cast<char*>(entry), sizeof(entry));
            palette[i] = ArgbColor::fromRgb8(entry[2], entry[1], entry[0]);
        }
    }

    Pixmap image;
    image.resize(Size(width, height));

    // Rows are stored bottom-up and padded to a 4 byte boundary.
    const int rowBytes = ((width * bits + 31) / 32) * 4;
    std::vector<char> row(rowBytes);

    // Track whether the 32 bit XOR bitmap provides a usable alpha channel.
    // Many icons store a 32 bit image with an all-zero alpha channel and
    // rely on the AND mask instead. In that case the alpha must not be used.
    bool hasSourceAlpha = false;

    for (int y = height - 1; y >= 0; --y)
    {
        _stream->read(&row[0], rowBytes);

        for (int x = 0; x < width; ++x)
        {
            ArgbColor c;

            switch (bits)
            {
                case 1:
                {
                    const unsigned char byte = static_cast<unsigned char>(row[x / 8]);
                    const int index = (byte >> (7 - (x % 8))) & 0x01;
                    if (index < static_cast<int>(palette.size()))
                        c = palette[index];
                }
                break;

                case 4:
                {
                    const unsigned char byte = static_cast<unsigned char>(row[x / 2]);
                    const int index = (x & 1) ? (byte & 0x0F) : (byte >> 4);
                    if (index < static_cast<int>(palette.size()))
                        c = palette[index];
                }
                break;

                case 8:
                {
                    const int index = static_cast<unsigned char>(row[x]);
                    if (index < static_cast<int>(palette.size()))
                        c = palette[index];
                }
                break;

                case 24:
                {
                    const unsigned char* p = reinterpret_cast<unsigned char*>(&row[x * 3]);
                    c = ArgbColor::fromRgb8(p[2], p[1], p[0]);
                }
                break;

                case 32:
                {
                    const unsigned char* p = reinterpret_cast<unsigned char*>(&row[x * 4]);
                    if (p[3] != 0)
                        hasSourceAlpha = true;

                    c = ArgbColor::fromRgb8(p[2], p[1], p[0], p[3]);
                }
                break;

                default:
                    // Unsupported bit depth for now.
                    c = ArgbColor();
                break;
            }

            image.setColor(c, gfx::Point(x, y));
        }
    }

    // If a 32 bit image has no usable alpha channel, treat all pixels as
    // opaque so that the AND mask below can define the transparency.
    if (bits == 32 && !hasSourceAlpha)
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                ArgbColor c = image.color(gfx::Point(x, y));
                c.setAlpha(255);
                image.setColor(c, gfx::Point(x, y));
            }
        }
    }

    // The AND (transparency) mask is always present in an icon image and
    // applies to every bit depth. A set bit marks a fully transparent pixel.
    const int maskRowBytes = ((width + 31) / 32) * 4;
    std::vector<char> mask(maskRowBytes);

    for (int y = height - 1; y >= 0; --y)
    {
        _stream->read(&mask[0], maskRowBytes);

        for (int x = 0; x < width; ++x)
        {
            const unsigned char byte = static_cast<unsigned char>(mask[x / 8]);
            const bool transparent = (byte >> (7 - (x % 8))) & 0x01;

            if (transparent)
            {
                ArgbColor c = image.color(gfx::Point(x, y));
                c.setAlpha(0);
                image.setColor(c, gfx::Point(x, y));
            }
        }
    }

    _icon->addImage(Size(width, height), image);
}

void IcoReaderImpl::readImage(const IcoDirEntry& entry)
{
    _stream->seekg(entry.imageOffset, std::ios_base::beg);

    // Peek the first bytes to detect a PNG encoded image.
    std::uint32_t magic = 0;
    readPod(magic);
    _stream->seekg(entry.imageOffset, std::ios_base::beg);

    if (magic == ICO_PNG_MAGIC)
    {
        Pixmap image;
        image.load(*_stream, ImageFileType::Png);
        _icon->addImage(image.size(), image);
        return;
    }

    readDibImage(entry);
}

void IcoReaderImpl::read()
{
    readDirectory();

    for (std::size_t i = 0; i < _entries.size(); ++i)
        readImage(_entries[i]);
}


IcoReader::IcoReader()
: _icon(0)
, _stream(0)
{
}

IcoReader::IcoReader(std::istream& is, Icon& icon)
: _icon(&icon)
, _stream(&is)
{
}

IcoReader::~IcoReader()
{
}

void IcoReader::onAttach()
{
}

void IcoReader::onDetach()
{
}

void IcoReader::onReset()
{
}

bool IcoReader::onAdvance()
{
    return true;
}

void IcoReader::onRead()
{
    if (_stream == 0 || _icon == 0)
        throw std::logic_error("Load Icon failed: reader not attached.");

    IcoReaderImpl impl(*_stream, *_icon);
    impl.read();
}


}}}

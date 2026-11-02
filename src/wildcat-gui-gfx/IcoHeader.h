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
#ifndef WILDCAT_GUI_GFX_ICOHEADER_H
#define WILDCAT_GUI_GFX_ICOHEADER_H

#include <ios>
#include <cstdlib>
#include <cinttypes>


namespace wildcat {
namespace gui {
namespace gfx {

static constexpr const std::uint16_t ICO_TYPE_ICON = 1;
static constexpr const std::uint32_t ICO_PNG_MAGIC = 0x474E5089; // "\x89PNG" little endian

#include <Pt/Pack.h>
// ICONDIR - file header of an *.ico file
struct PT_PACKED IcoDir
{
    std::uint16_t reserved;   // Reserved, must be 0
    std::uint16_t type;       // Resource type, 1 for icons
    std::uint16_t count;      // Number of images in the file
};

// ICONDIRENTRY - describes a single image in the file
struct PT_PACKED IcoDirEntry
{
    std::uint8_t  width;         // Width in pixels (0 means 256)
    std::uint8_t  height;        // Height in pixels (0 means 256)
    std::uint8_t  colorCount;    // Number of colors in palette (0 if >=8bpp)
    std::uint8_t  reserved;      // Reserved, must be 0
    std::uint16_t planes;        // Color planes
    std::uint16_t bitCount;      // Bits per pixel
    std::uint32_t bytesInRes;    // Size of the image data in bytes
    std::uint32_t imageOffset;   // Offset of the image data from the start of the file
};

// BITMAPINFOHEADER - describes the DIB stored inside an icon image
struct PT_PACKED IcoBitmapInfoHeader
{
    std::uint32_t size;             // Size of this header (40)
    std::int32_t  width;            // Width of the image
    std::int32_t  height;           // Height of the image (XOR + AND mask, so twice the icon height)
    std::uint16_t planes;           // Color planes
    std::uint16_t bitCount;         // Bits per pixel
    std::uint32_t compression;      // Compression type
    std::uint32_t sizeImage;        // Size of the image data
    std::int32_t  xPelsPerMeter;    // Horizontal resolution
    std::int32_t  yPelsPerMeter;    // Vertical resolution
    std::uint32_t clrUsed;          // Number of colors used
    std::uint32_t clrImportant;     // Number of important colors
};
#include <Pt/PackPop.h>

}}}

#endif

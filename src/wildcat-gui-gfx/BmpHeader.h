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
#ifndef WILDCAT_GUI_GFX_BMPHEADER_H
#define WILDCAT_GUI_GFX_BMPHEADER_H

#include <ios>
#include <cstdlib>
#include <cinttypes>


namespace wildcat {
namespace gui {
namespace gfx {

static constexpr const std::uint16_t Pixmap_BUFFER_MAGIC = 0x4D42;

#include <Pt/Pack.h>
struct PT_PACKED PixmapHeader
{
    // Pixmap file header structure
    std::uint16_t magic;              // Magic number for file always BM which is 0x4D42
    std::uint32_t file_size;          // Size of file
    std::uint16_t reserved1;          // Reserved
    std::uint16_t reserved2;          // Reserved
    std::uint32_t offset_bits;        // Offset to Pixmap data
    // Pixmap file info structure 
    std::uint32_t size;               // Size of info header
    std::int32_t width;               // Width of image
    std::int32_t height;              // Height of image
    std::uint16_t planes;             // Number of color planes
    std::uint16_t bits_per_pixel;     // Number of bits per pixel
    std::uint32_t compression;        // Type of compression to use
    std::uint32_t size_image;         // Size of image data
    std::int32_t x_pixels_per_meter;  // X pixels per meter
    std::int32_t y_pixels_per_meter;  // Y pixels per meter
    std::uint32_t clr_used;           // Number of colors used 
    std::uint32_t clr_important;      // Number of important colors
};
#include <Pt/PackPop.h>

}}}

#endif

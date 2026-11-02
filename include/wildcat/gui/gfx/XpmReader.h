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
#ifndef WILDCAT_GUI_GFX_XPMREADER_H
#define WILDCAT_GUI_GFX_XPMREADER_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/ImageReader.h>
#include <wildcat/gui/gfx/Point.h>
#include <wildcat/gui/gfx/Color.h>
#include <iostream>
#include <map>

namespace wildcat {
namespace gui{
namespace gfx {

class Pixmap;
class XpmReaderImpl;

class WILDCAT_GUI_GFX_API XpmReader
{
    public:
        XpmReader(Pixmap& image);

        virtual ~XpmReader();

        const Point& hotspot() const;

        void read(const char* file);

        void read(std::istream& stream,  bool raw = true);

        void read(const char* stream[]);

    private:
        XpmReaderImpl*  _impl;

};

}}}

#endif

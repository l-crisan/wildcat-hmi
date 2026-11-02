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
#ifndef WILDCAT_GUI_GFX_COLORPALETTE_H
#define WILDCAT_GUI_GFX_COLORPALETTE_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Color.h>
#include <wildcat/gui/base/Resource.h>
#include <map>

namespace wildcat {
namespace gui {
namespace gfx {


class WILDCAT_GUI_GFX_API ColorPalette : public base::Resource
{
    public:
        friend WILDCAT_GUI_GFX_API std::ostream& operator<<(std::ostream& os, const ColorPalette& obj);

        friend  WILDCAT_GUI_GFX_API std::istream& operator>>(std::istream& is, ColorPalette& obj);

    public:
        ColorPalette(const std::string& name = "");

        virtual ~ColorPalette();

        void add( int id, const ArgbColor& color);

        const ArgbColor* get(int id) const;

        ArgbColor* get(int id);

        void remove(int d);

        void clear();

        bool empty() const
        {
            return _palette.empty();
        }

        size_t size() const
        {
            return _palette.size();
        }

    protected:
        virtual void onSave(std::ostream& os) const
        {
            os << *this;
        }

        virtual void onLoad(std::istream& is)
        {
            is >>*this;
        }

     private:
        std::map<int, ArgbColor> _palette;  
};

WILDCAT_GUI_GFX_API std::ostream& operator<<(std::ostream& os, const ColorPalette& obj);

WILDCAT_GUI_GFX_API std::istream& operator>>(std::istream& is, ColorPalette& obj);

}}}

#endif
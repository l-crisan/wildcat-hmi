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
#include <wildcat/gui/gfx/ColorPalette.h>
#include <wildcat/gui/base/String.h>

namespace wildcat {
namespace gui { 
namespace gfx {


ColorPalette::ColorPalette(const std::string& name)
: base::Resource(name)
, _palette()
{
}

ColorPalette::~ColorPalette()
{

}

void ColorPalette::add(int id, const ArgbColor& color)
{
    _palette[id] = color;
}

const ArgbColor* ColorPalette::get(int id) const
{
    auto it = _palette.find(id);

    return it == _palette.end() ? nullptr : &it->second;
}

ArgbColor* ColorPalette::get(int id)
{
    auto it = _palette.find(id);

    return it == _palette.end() ? nullptr : &it->second;
}

void ColorPalette::remove(int id)
{
    auto it = _palette.find(id);

    if( it == _palette.end())
        return;

    _palette.erase(it);
}

void ColorPalette::clear()
{
    _palette.clear();
}

WILDCAT_GUI_GFX_API std::ostream& operator<<(std::ostream& os, const ColorPalette& obj)
{
    os << '(';
    os << obj.name() << ',';

    os << obj.size() << ',';

    if (!obj.empty())
    {
        auto it = obj._palette.begin();

        for (; it != obj._palette.end(); ++it)
        {
            os <<(uint32_t)it->first<<"="<<it->second;

            auto next = it;
            ++next;
            if (next != obj._palette.end())
                os << ';';
        }

        os << ')';
    }
    else
    {
        os << ')';
    }

    os.flush();
    return os;
}

WILDCAT_GUI_GFX_API  std::istream& operator>>(std::istream& is, ColorPalette& obj)
{
    std::string buffer;
    size_t count = 0;
    char ch;

    is >> ch;

    std::getline(is, buffer, ',');
    obj.setName(buffer);

    std::getline(is, buffer, ',');

    count = base::String::to<size_t>(buffer);

    for (size_t i = 0; i < count; ++i)
    {
        
        std::getline(is, buffer, '=');

        int key  = (int)base::String::to<uint32_t>(buffer);

        gfx::ArgbColor c;
        is>>c;
        obj.add(key, c);
    }

    std::getline(is, buffer, ')');
    return is;
}

}}  }

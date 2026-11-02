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
#include <wildcat/gui/gfx/ColorManager.h>

namespace wildcat {
namespace gui { 
namespace gfx {

ColorManager::ColorManager()
: Pt::Singleton<ColorManager>()
, _palette()
, _id(-1)
, _name()
{
}

void ColorManager::add(int id, ColorPalette& palette)
{
    _palette[id] = &palette;
}

void ColorManager::swap(int id1, int id2)
{
    auto it1 = _palette.find(id1);
    auto it2 = _palette.find(id2);

    if(it1 == _palette.end() || it2 == _palette.end())
        return;

    std::swap(it1->second, it2->second);
}   

ColorPalette* ColorManager::get(int id)
{
    auto it = _palette.find(id);

    return it == _palette.end() ? nullptr : it->second;
}

const ColorPalette* ColorManager::get(int id) const
{
    auto it = _palette.find(id);

    return it == _palette.end() ? nullptr : it->second;
}

const ArgbColor* ColorManager::get(int palette, int index) const
{
    const ColorPalette* cp = get(palette);

    return ( cp == nullptr) ? nullptr : cp->get(index);
}

ArgbColor* ColorManager::get(int palette, int index)
{
    ColorPalette* cp = get(palette);

    return (cp == nullptr) ? nullptr : cp->get(index);
}

void ColorManager::remove(int id)
{
    auto it = _palette.find(id);

    if (it == _palette.end())
        return;

    _palette.erase(it);
}

void ColorManager::clear()
{
    _palette.clear();
}

}}}

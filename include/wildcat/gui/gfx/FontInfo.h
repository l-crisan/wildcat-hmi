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
#ifndef WILDCAT_GUI_GFX_FONTINFO_H
#define WILDCAT_GUI_GFX_FONTINFO_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Font.h>
#include <wildcat/gui/base/String.h>
#include <Pt/Types.h>
#include <Pt/String.h>
#include <Pt/System/Path.h>
#include <set>
#include <map>

namespace wildcat {
namespace gui {
namespace gfx {

class WILDCAT_GUI_GFX_API FontInfo
{

public:
    FontInfo();
    
    FontInfo(const std::string& name);
    
    const std::string& name() const
    {
        return _name;
    }

    void setName(const std::string& v)
    {
        _name = v;
    }
   
    
    const std::set<std::string>& styles() const
    {
        return _styles;
    }

    void setStyle(const std::set<std::string>& v)
    {
        _styles = v;
    }

    bool hasStyle(const std::string& s) const
    {
        return _styles.find(s) != _styles.end();
    }

    void addStyle(const std::string& s)
    {        
        _styles.insert(s);
    }

    void addPath(const std::string& stype, const Pt::System::Path& v)
    {
        _path[stype] = v;
    }

    const Pt::System::Path& getPath(const std::string& stype) const
    {
        static Pt::System::Path spath;

        auto it = _path.find(stype);

        return it == _path.end() ? spath : it->second;
    }

    private:
        std::map<std::string, Pt::System::Path> _path;
        std::string _name;        
        std::set<std::string>    _styles;
};


inline bool operator<(const FontInfo & a, const FontInfo & b)
{
    return a.name() < b.name() ;
}

inline bool operator==(const FontInfo& a, const FontInfo& b)
{
    return a.name() == b.name();        
}

inline bool operator!=(const FontInfo a, const FontInfo& b)
{
    return a.name() != b.name();
}

}}}

#endif

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
#ifndef WILDCAT_GUI_BASE_PROPERTYINFO_H
#define WILDCAT_GUI_BASE_PROPERTYINFO_H  


#include <Pt/String.h>
#include <wildcat/gui/base/String.h>
#include <string>
#include <iostream>
#include <map>

namespace wildcat {
namespace gui{  
namespace base {

inline std::istream& operator>>(std::istream& is, Pt::Char& ch)
{
    uint32_t v;
    is >> v;
    ch = v;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Pt::Char& ch)
{
    os << ch.value();
    return os;
}

typedef std::map<std::string, std::string> AttributeMap;

class PropertyInfo
{
    public:
        PropertyInfo(const std::string& name, const std::string& attr)
        : _name(name)
        {
            base::StringVector attrs = base::String::split<std::string>(attr, ";");

            for (size_t i = 0; i < attrs.size(); ++i)
            {
                const std::string& pair = attrs[i];

                const base::StringVector pp = base::String::split<std::string>(pair, "=");

                if (pp.size() == 1)
                    _attributes.insert(std::make_pair(pp[0], ""));
                else
                    _attributes.insert(std::make_pair(pp[0], pp[1]));
            }
        }


        virtual ~PropertyInfo()
        {

        }

        const std::string& name() const
        {
            return _name;
        }
        
        const AttributeMap& attributes() const
        {
            return _attributes; 
        }

        bool hasAttribute(const std::string& attr) const
        {
            return _attributes.find(attr) != _attributes.end();
        }

        const std::string& attributeValue(const std::string& attr) const
        {
            AttributeMap::const_iterator it = _attributes.find(attr);

            if (it == _attributes.end())
            {
                static std::string empty;
                return empty;
            }

            return it->second;
        }

    private:
        std::string _name;
        AttributeMap _attributes;
};

}}}
#endif

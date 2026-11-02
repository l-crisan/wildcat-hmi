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

#include <wildcat/gui/base/TextTable.h>
#include <wildcat/gui/base/String.h>
#include <Pt/StringStream.h>

namespace wildcat {
namespace gui {
namespace base {


TextTable::TextTable()
{

}

TextTable::TextTable(const std::string& name)
: Resource(name)
{
}

TextTable::TextTable(const std::string& name, const TextMap& t)
: Resource(name)
, _textMap(t)   
{
}


TextTable::~TextTable()
{
}


void TextTable::onSave(std::ostream& os) const
{
    os<<*this;
}   


void TextTable::onLoad(std::istream& is)
{
    is>>*this;
}

void TextTable::fromString(const Pt::String& text)
{
    _textMap.clear();

    std::vector<Pt::String> lines = base::String::split<Pt::String>(text, "\n");

    for (size_t i = 0; i < lines.size(); ++i)
    {
        const Pt::String& line = lines[i];

        size_t pos = line.find('=');

        if (pos == Pt::String::npos)
            continue;

        const Pt::String key = line.substr(0, pos);
        const Pt::String value = line.substr(pos + 1);

        _textMap[key] = value;
    }
}

Pt::String TextTable::toString() const
{
    Pt::StringStream ss;

    auto it = _textMap.begin(); 

    for( ; it != _textMap.end(); ++it)
    {
        ss<<it->first<<"="<<it->second;

        auto next = it;

        ++next;

        if( next != _textMap.end())
            ss<<"\n";
    }   

    return Pt::String(ss.str());
}

void TextTable::onAppend(Resource* resource)
{
    const TextTable* tt = dynamic_cast<const TextTable*>(resource);

    if( tt != nullptr)
    {
    
        for (auto it = tt->_textMap.begin(); it != tt->_textMap.end(); ++it)
    
            _textMap[it->first] = it->second;    
    }

    Resource::onAppend(resource);       
}   

WILDCAT_GUI_BASE_API std::ostream& operator<<(std::ostream& os, const TextTable& obj)
{
    os << '(';
    os << obj.name() << ',';

    os << obj.size() << ',';

    if (!obj.empty())
    {
        auto it = obj._textMap.begin();

        for (; it != obj._textMap.end(); ++it)
        {
             os << String::toUtf8(it->first) << '=' << String::String::toUtf8(it->second);

            auto next = it;
            ++next;
            if( next != obj._textMap.end())
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

WILDCAT_GUI_BASE_API  std::istream& operator>>(std::istream& is, TextTable& obj)
{
    std::string buffer;
    size_t count = 0;
    char ch;

    is >> ch;
    
    std::getline(is, buffer, ',');
    obj.setName(buffer);

    std::getline(is, buffer, ',');

    count = String::to<size_t>(buffer);

    for (size_t i = 0; i < count; ++i)
    {
        
        if( i != (count - 1))
            std::getline(is, buffer, ';');
        else
            std::getline(is, buffer, ')');            

        size_t pos = buffer.find('=');

        if (pos == Pt::String::npos)
            continue;
        
        const std::string key   = buffer.substr(0, pos);
        const std::string value = buffer.substr(pos + 1);

        const Pt::String ucKey = String::fromUtf8(key);
        const Pt::String ucValue = String::fromUtf8(value);
        obj.add(ucKey, ucValue);
    }

    return is;
}

}}}

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
#ifndef WILDCAT_GUI_BASE_TEXTTABLE_H
#define WILDCAT_GUI_BASE_TEXTTABLE_H

#include <wildcat/gui/base/Api.h>
#include <wildcat/gui/base/Resource.h>
#include <Pt/String.h>
#include <map>
#include <string>


namespace wildcat {
namespace gui {
namespace base {


class WILDCAT_GUI_BASE_API TextTable : public Resource
{
    public:
        typedef std::map<Pt::String, Pt::String> TextMap;

        friend WILDCAT_GUI_BASE_API std::ostream& operator<<(std::ostream& os, const TextTable& obj);

        friend WILDCAT_GUI_BASE_API  std::istream& operator>>(std::istream& is, TextTable& obj);

    public:
        TextTable();

        TextTable(const std::string& name);

        TextTable(const std::string& name, const TextMap& t);

        
        virtual ~TextTable();   

   
        void clear()
        {
            _textMap.clear();
        }   

        bool empty() const
        {
            return _textMap.empty();
        }   

        void add(const Pt::String& key, const Pt::String& value)
        {
            _textMap[key] = value;
        }   

        void remove(const Pt::String& key)
        {
            _textMap.erase(key);
        }   
        
        const Pt::String& tr(const Pt::String& key) const
        {
            auto it = _textMap.find(key);

            if( it != _textMap.end())
                return it->second;

            return key;
        }   

        bool contains(const Pt::String& key) const
        {
            return _textMap.find(key) != _textMap.end();
        }

        void set(const std::string& text)
        {
            _textMap[text.c_str()] = "set";
        }
    
        size_t size() const
        {
            return _textMap.size();
        }   

        Pt::String toString() const;

        void fromString(const Pt::String& text);

    protected:
        virtual void onSave(std::ostream& os) const override;

        virtual void onLoad(std::istream& is) override; 

        virtual void onAppend(Resource* resource) override;

    private:
        TextMap _textMap;
};

WILDCAT_GUI_BASE_API std::ostream& operator<<(std::ostream& os, const TextTable& obj);

WILDCAT_GUI_BASE_API  std::istream& operator>>(std::istream& is, TextTable& obj);

}}}

#endif

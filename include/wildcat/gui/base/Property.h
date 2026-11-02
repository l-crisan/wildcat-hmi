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
#ifndef WILDCAT_GUI_BASE_PROPERTY_H
#define WILDCAT_GUI_BASE_PROPERTY_H

#include <wildcat/gui/base/Api.h>
#include <wildcat/gui/base/PropertyInfo.h>
#include <wildcat/gui/base/String.h>
#include <Pt/SerializationInfo.h>
#include <Pt/Connectable.h>
#include <Pt/Any.h>
#include <Pt/Types.h>
#include <string>
#include <map>

namespace wildcat {
namespace gui{
namespace base {

class WILDCAT_GUI_BASE_API Property : public Pt::Connectable
{        
    public:
        enum Kind
        {
            Simple,
            Array,
            Struct
        };

    public:
        Property();

        virtual ~Property();
                       
        virtual Kind kind() const = 0;

        virtual const std::type_info& typeInfo() const = 0;

        const std::string& name() const
        {
            return _info->name();
        }

        const AttributeMap& attributes() const
        {
            return _info->attributes();
        }

        bool hasAttribute(const std::string& attr) const
        {
            return _info->hasAttribute(attr);
        }

        const std::string& attributeValue(const std::string& attr) const 
        {
            return _info->attributeValue(attr);
        }

    protected:
        static PropertyInfo* propertyInfo(const std::type_info* id, const std::string& name)
        {
            auto it = _propertyInfos.find(id);

            if (it == _propertyInfos.end())
                return nullptr;

            for (size_t i = 0; i < it->second.size(); ++i)
            {
                if (it->second[i]->name() == name)
                    return it->second[i];
            }

            return nullptr;
        }

        static void registerProperty(const std::type_info* id, PropertyInfo* info)
        {
            auto it = _propertyInfos.find(id);

            if (it != _propertyInfos.end())
            {
                if (!hasProperty(it->second, info->name()))
                {
                    it->second.push_back(info);
                }
            }
            else
            {
                std::vector<PropertyInfo*> props;
                props.push_back(info);
                _propertyInfos.insert(std::make_pair(id, props));
            }
        }

    private:
        static bool hasProperty(const std::vector<PropertyInfo*>& props, const std::string& name)
        {
            for (size_t i = 0; i < props.size(); ++i)
            {
                if (props[i]->name() == name)
                    return true;
            }
            return false;
        }


    protected:
        PropertyInfo* _info;

    private:
        static std::map<const std::type_info*, std::vector<PropertyInfo*>> _propertyInfos;

};

typedef std::vector<Property*> PropertyVector;


}}}

#endif

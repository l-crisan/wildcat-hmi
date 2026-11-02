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
#ifndef WILDCAT_GUI_BASE_OBJECTPROPERTY_H
#define WILDCAT_GUI_BASE_OBJECTPROPERTY_H

#include <wildcat/gui/base/Property.h>
#include <wildcat/gui/base/ObjectPropertyInfo.h>
#include <wildcat/gui/base/String.h>
#include <Pt/Any.h>
#include <Pt/Types.h>
#include <string>
#include <sstream>
#include <map>

namespace wildcat {
namespace gui {
namespace base {

class Object;

class WILDCAT_GUI_BASE_API ObjectProperty : public Property
{
    public:
        ObjectProperty();
        
        virtual ~ObjectProperty();
        
        virtual Object* get() const = 0;

        virtual void set(Object* s) = 0;      

        virtual Kind kind() const
        {
            return Kind::Struct;
        }  
};


template <typename C>
class ObjectPropertyImpl : public ObjectProperty
{
    public:
        ObjectPropertyImpl(const std::string& name, C& parent, Object* (C::* getter)() const, void (C::* setter)(Object* val), const std::string& attributes)
        : ObjectProperty()
        , _obj(&parent)
        {
            _info = propertyInfo(&typeid(C), name);

            if (_info == nullptr)
            {
                _info = new ObjectPropertyInfo<C>(name, getter, setter, attributes);
                registerProperty(&typeid(C), _info);
            }
        }
    

        virtual Object* get() const
        {
            return dynamic_cast<ObjectPropertyInfo<C>*>(_info)->get(*_obj); 
        }


        virtual void set(Object* v)
        {        
            dynamic_cast<ObjectPropertyInfo<C>*>(_info)->set(*_obj, v);
        }


        virtual const std::type_info& typeInfo() const
        {
            return typeid(C);
        }

    private:    
        C* _obj;
};

}}}

#endif



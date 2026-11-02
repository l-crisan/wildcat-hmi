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
#ifndef WILDCAT_GUI_BASE_SIMPLEPROPERTY_H
#define WILDCAT_GUI_BASE_SIMPLEPROPERTY_H

#include <wildcat/gui/base/Property.h>
#include <wildcat/gui/base/SimplePropertyInfo.h>

#include <Pt/Any.h>
#include <Pt/Time.h>
#include <Pt/String.h>
#include <Pt/Date.h>
#include <Pt/DateTime.h>
#include <Pt/Types.h>

#include <string>

namespace wildcat {
namespace gui {
namespace base {

class WILDCAT_GUI_BASE_API SimpleProperty : public Property
{
    public:
        SimpleProperty();
        
        virtual ~SimpleProperty();
              
        virtual Kind kind() const
        {
            return Kind::Simple;
        }

        virtual std::string get() const = 0;

        virtual void set(const std::string& v) = 0;        

};

template <typename A, typename C>
class SimplePropertyImpl : public SimpleProperty
{
    public:
        SimplePropertyImpl(const std::string& name, C& parent, A(C::* getter)() const, void (C::* setter)(A type), const std::string& attributes)
        : SimpleProperty()
        , _obj(&parent)
        {
            _info = propertyInfo(&typeid(C), name); 

            if (_info == nullptr)   
            {
                _info = new SimplePropertyInfo<A, C>(name, getter, setter, attributes);
                registerProperty(&typeid(C), _info);    
            }
        }

        virtual std::string get() const
        {
            return dynamic_cast<SimplePropertyInfo<A, C>*>(_info)->get(*_obj);
            
        }

        virtual void set(const std::string& v)
        {
            return dynamic_cast<SimplePropertyInfo<A, C>*>(_info)->set(*_obj, v);
        }

        virtual const std::type_info& typeInfo() const
        {
            return typeid(A);
        }

    private:
        C*  _obj;
};

}}}

#endif

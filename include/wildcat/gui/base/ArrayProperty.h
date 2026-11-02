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
#ifndef WILDCAT_GUI_BASE_ARRAYPROPERTY_H
#define WILDCAT_GUI_BASE_ARRAYPROPERTY_H

#include <wildcat/gui/base/Property.h>
#include <wildcat/gui/base/ArrayPropertyInfo.h>
#include <wildcat/gui/base/String.h>
#include <Pt/Any.h>
#include <Pt/Types.h>
#include <string>
#include <sstream>
#include <map>

namespace wildcat {
namespace gui{
namespace base {

class Object;

class WILDCAT_GUI_BASE_API ArrayProperty : public Property
{
    public:
        ArrayProperty();
        
        virtual ~ArrayProperty();

        virtual uint32_t count() const = 0;

        virtual Kind kind() const
        {
            return Kind::Array;
        }

        virtual Kind kindOf() const = 0;
};

class WILDCAT_GUI_BASE_API SimpleArrayProperty : public ArrayProperty
{
    public:
        SimpleArrayProperty();

        virtual ~SimpleArrayProperty();


        virtual Kind kindOf() const
        {
            return Kind::Simple;
        }

        virtual std::string get(uint32_t index) const = 0;

        virtual void set(uint32_t index, const std::string& v) = 0;

        virtual void set(const std::vector<std::string>& v) = 0;

};

class WILDCAT_GUI_BASE_API ObjectArrayProperty : public ArrayProperty
{
    public:
        ObjectArrayProperty();

        virtual ~ObjectArrayProperty();


        virtual Kind kindOf() const
        {
            return Kind::Struct;
        }

        virtual Object* get(uint32_t index) const = 0;

        virtual void set(uint32_t index, Object*  v) = 0;     

        virtual void add(Object* v) = 0;
        
        virtual const std::type_info& typeInfo() const;        
};

template <typename A, typename C>
class ArrayPropertyImpl : public SimpleArrayProperty
{
    public:
        ArrayPropertyImpl(const std::string& name, C& parent, A(C::* getter)(uint32_t index) const, void (C::* setter)(uint32_t index, A), uint32_t(C::* count)() const, const std::string& comment)
        : SimpleArrayProperty()
        , _obj(&parent)
        {
            _info = propertyInfo(&typeid(C), name);

            if (_info == nullptr)
            {
                _info = new ArrayPropertyInfo<A, C>(name, getter, setter, attributes);
                registerProperty(&typeid(C), _info);
            }
        }
             
        virtual ~ArrayPropertyImpl()
        {         
        }        

        virtual uint32_t count() const
        {
            return dynamic_cast<ArrayPropertyInfo<A, C>*>(_info)->count(*_obj);
        }

        virtual std::string get(uint32_t index) const
        {
            return dynamic_cast<ArrayPropertyInfo<A, C>*>(_info)->get(*_obj, index);    
        }

        virtual void set( uint32_t index, const std::string& v)
        {
            dynamic_cast<ArrayPropertyInfo<A, C>*>(_info)->set(*_obj, index, v);
        }

        virtual void add(const std::string& v)
        {
            uint32_t index = count();
            set(index, v);
        }

        virtual void set(const std::vector<std::string>& v)
        {
            for (uint32_t i = 0; i < v.size(); ++i)
                set(i, v[i]);
        }

        virtual const std::type_info& typeInfo() const
        {
            return typeid(A);
        }

        virtual void onCopy(const Property& other)
        {
            const SimpleArrayProperty* sp = dynamic_cast<const SimpleArrayProperty*>(&other);

            if (sp)
            {
                std::vector<std::string> data;

                for (uint32_t i = 0; i < sp->count(); ++i)
                    data.push_back(sp->get(i));

                set(data);
            }
        }

    private:
        C* _obj;
};

template <typename C>
class ObjArrayPropertyImpl : public ObjectArrayProperty
{
    public:
        ObjArrayPropertyImpl(const std::string& name, C& parent, Object* (C::* getter)(uint32_t index) const, void (C::* setter)(uint32_t index, Object*), uint32_t(C::* count)() const, const std::string& attributes)
        : ObjectArrayProperty()
        , _obj(&parent)
        {
            _info = propertyInfo(&typeid(C), name);

            if (_info == nullptr)
            {
                _info = new ObjArrayPropertyInfo<C>(name, getter, setter, count, attributes);
                registerProperty(&typeid(C), _info);
            }
        }

        virtual ~ObjArrayPropertyImpl()
        {
        }

        virtual uint32_t count() const
        {
            return dynamic_cast<ObjArrayPropertyInfo<C>*>(_info)->count(*_obj);
        }
 
        virtual Object* get(uint32_t index) const
        {
            return dynamic_cast<ObjArrayPropertyInfo<C>*>(_info)->get(*_obj,index);
        }

        virtual void set(uint32_t index, Object* v)
        {
            dynamic_cast<ObjArrayPropertyInfo<C>*>(_info)->set(*_obj, index, v);
        }

        virtual void add(Object* v)
        {
            uint32_t index = count();
            set(index, v);
        }

    private:
        C* _obj;        
};

}}}

#endif



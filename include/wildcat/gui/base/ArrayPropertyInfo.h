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
#ifndef WILDCAT_GUI_BASE_ARRAYPROPERTYINFO_H
#define WILDCAT_GUI_BASE_ARRAYPROPERTYINFO_H  

#include <wildcat/gui/base/PropertyInfo.h>
#include <Pt/String.h>
#include <string>
#include <iostream>
#include <map>

namespace wildcat {
namespace gui {
namespace base {

class Object;

//Simple type
template <typename A, typename C>
class ArrayPropertyInfo : public PropertyInfo
{
    public:
        ArrayPropertyInfo(const std::string& name, A(C::* getter)() const, void (C::* setter)(A type), const std::string& attributes)
        : PropertyInfo(name, attributes)
        , _count(count)
        , _getter(getter)
        , _setter(setter)
        {
        }

        virtual ~ArrayPropertyInfo()
        {
        }

        uint32_t count(const C& obj) const
        {
            return (obj.*_count)();
        }

        std::string get(const C& obj, uint32_t index) const
        {
            std::stringstream ss;
            A a = (obj.*_getter)(index);
            ss << a;
            return ss.str();
        }

        void set(const std::string& v, const C& obj, uint32_t index)
        {
            std::stringstream ss;

            typename std::remove_const<typename std::remove_reference<A>::type>::type a;
            ss << v;
            ss >> a;
            (obj.*_setter)(index, a);
        }

    private:
        uint32_t (C::* _count)() const;
        A(C::* _getter)() const;
        void (C::* _setter)(A);
};

//Object array 
template <typename C>
class ObjArrayPropertyInfo : public PropertyInfo
{
    public:
        ObjArrayPropertyInfo(const std::string& name, Object* (C::* getter)(uint32_t index) const, void (C::* setter)(uint32_t index, Object*), Pt::uint32_t(C::* count)() const, const std::string& attributes)
        : PropertyInfo(name, attributes)
        , _count(count)
        , _getter(getter)
        , _setter(setter)
        {
        }

        virtual ~ObjArrayPropertyInfo()
        {
        }

        uint32_t count(const C& obj) const
        {
            return (obj.*_count)();
        }

        Object* get(const C& obj, uint32_t index) const
        {
            return (obj.*_getter)(index);
        }


        void set(C& obj, uint32_t index, Object* v)
        {
            (obj.*_setter)(index, v);
        }

    private:
        uint32_t (C::* _count)() const;
        Object* (C::* _getter)(uint32_t) const;
        void (C::* _setter)(uint32_t, Object*);
};

}}}

#endif

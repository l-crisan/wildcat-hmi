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
#ifndef WILDCAT_GUI_BASE_SIMPLEPROPERTYINFO_H
#define WILDCAT_GUI_BASE_SIMPLEPROPERTYINFO_H  

#include <wildcat/gui/base/PropertyInfo.h>
#include <Pt/String.h>
#include <string>
#include <iostream>
#include <map>

namespace wildcat {
namespace gui {
namespace base {

//Simple type
template <typename A, typename C>
class SimplePropertyInfo : public PropertyInfo
{
    public:
        SimplePropertyInfo(const std::string& name, A(C::* getter)() const, void (C::* setter)(A type), const std::string& attributes)
        : PropertyInfo(name, attributes)
        , _getter(getter)
        , _setter(setter)
        {
        }

        virtual ~SimplePropertyInfo()
        {
        }   

        std::string get(const C& obj) const
        {
            std::stringstream ss;
            A a = (obj.*_getter)();
            ss << a;
            ss.flush();
            return ss.str();
        }

        void set(C& obj, const std::string& v)
        {
            std::stringstream ss(v);
            typename std::remove_const<typename std::remove_reference<A>::type>::type a;

            ss >> a;
            (obj.*_setter)(a);
        }

    private:
        A(C::* _getter)() const;
        void (C::* _setter)(A);
};

//const std::string type
template <typename C>
class SimplePropertyInfo<const std::string&, C> : public PropertyInfo
{
    public:
        SimplePropertyInfo(const std::string& name, const std::string& (C::* getter)() const, void (C::* setter)(const std::string& v), const std::string& attributes)
        : PropertyInfo(name, attributes)
        , _getter(getter)
        , _setter(setter)
        {
        }

        std::string get(const C& obj) const
        {
            return (obj.*_getter)();
        }

        void set(C& obj, const std::string& v)
        {
            (obj.*_setter)(v);
        }

    private:
        const std::string& (C::* _getter)() const;
        void (C::* _setter)(const std::string&);

};


//std::string type
template <typename C>
class SimplePropertyInfo<std::string, C> : public PropertyInfo
{
public:
    SimplePropertyInfo(const std::string& name,std::string(C::* getter)() const, void (C::* setter)(std::string v), const std::string& attributes)
    : PropertyInfo(name, attributes)
    , _getter(getter)
    , _setter(setter)
    {
    }

    std::string get(const C& obj) const
    {
        return (obj.*_getter)();
    }

    void set(C& obj, const std::string& v)
    {
        (obj.*_setter)(v);
    }


private:
    std::string(C::* _getter)() const;
    void (C::* _setter)(std::string);
};

//const Pt::String type
template <typename C>
class SimplePropertyInfo<const Pt::String&, C> : public PropertyInfo
{
    public:
        SimplePropertyInfo(const  std::string& name, const  Pt::String& (C::* getter)() const, void (C::* setter)(const  Pt::String& v), const std::string& attributes)
        : PropertyInfo(name, attributes)
        , _getter(getter)
        , _setter(setter)
        {
        }

        std::string get(const C& obj) const
        {
            const Pt::String& s = (obj.*_getter)();
            return base::String::toUtf8(s);
        }

        void set(C& obj, const std::string& v)
        {
            const Pt::String s = base::String::fromUtf8(v);
            (obj.*_setter)(s);
        }

    private:
        const Pt::String& (C::* _getter)() const;

        void (C::* _setter)(const Pt::String&);
};


//const Pt::Char type
template <typename C>
class SimplePropertyInfo<const Pt::Char&, C> : public PropertyInfo
{
    public:
        SimplePropertyInfo(const std::string& name, const Pt::Char& (C::* getter)() const, void (C::* setter)(const Pt::Char& v), const std::string& attributes)
        : PropertyInfo(name, attributes)
        , _getter(getter)
        , _setter(setter)
        {
        }

        std::string get(const C& obj) const
        {
            std::stringstream ss;
            Pt::Char c = (obj.*_getter)();
            ss << (char)c;
            ss.flush();
            return ss.str();
        }

        void set(C& obj, const std::string& v)
        {
            Pt::Char ch = v.empty() ? Pt::Char() : Pt::Char(v[0]);

            (obj.*_setter)(ch);
        }


    private:
        const Pt::Char& (C::* _getter)() const;
        void (C::* _setter)(const Pt::Char&);
};

//Pt::Char type
template <typename C>
class SimplePropertyInfo<Pt::Char, C> : public PropertyInfo
{
    public:
        SimplePropertyInfo(const std::string& name, Pt::Char(C::* getter)() const, void (C::* setter)(Pt::Char  v), const std::string& attributes)
        : PropertyInfo(name, attributes)
        , _getter(getter)
        , _setter(setter)
        {
        }

        std::string get(const C& obj) const
        {
            std::stringstream ss;
            const Pt::Char c = (obj.*_getter)();
            ss << (char)c;
            ss.flush();
            return ss.str();
        }

        void set(C& obj, const std::string& v)
        {
            const Pt::Char ch = v.empty() ? Pt::Char() : Pt::Char(v[0]);

            (obj.*_setter)(ch);
        }


    private:
        Pt::Char(C::* _getter)() const;

        void (C::* _setter)(Pt::Char);

};

}}}

#endif

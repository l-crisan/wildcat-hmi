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
#ifndef WILDCAT_GUI_BASE_TYPEFACTORY_H
#define WILDCAT_GUI_BASE_TYPEFACTORY_H

#include <wildcat/gui/base/Api.h>
#include <Pt/Singleton.h>
#include <map>
#include <string>
#include <typeinfo>

namespace wildcat {
namespace gui {
namespace base {


class TypeInfoBase
{
    protected:
        TypeInfoBase(const std::string& n)
        : _name(n)
        {
        }

    public:
        virtual ~TypeInfoBase()
        {
        }
    
        virtual void* create() = 0;

        const std::string& name() const
        {
            return _name;
        }


    private:
        std::string _name;
};


template<typename T> 
class TypeInfo : public TypeInfoBase
{
    public:
        TypeInfo(const std::string& n)
        : TypeInfoBase(n)
        {

        }

        virtual ~TypeInfo()
        {

        }

        virtual void* create()
        {
            return new T();
        }

};

typedef std::map<std::string, TypeInfoBase*> TypeInfoMap;
typedef std::map<std::string, std::string> CppTypeMap;

class WILDCAT_GUI_BASE_API TypeFactory : public Pt::Singleton<TypeFactory>
{
    public:
        friend class Pt::Singleton<TypeFactory>;

    protected:
        TypeFactory();

    public:

        virtual ~TypeFactory();


        template<typename T>
        void regType(const std::string& typeName)
        {            
            TypeInfoBase* ti = new TypeInfo<T>(typeName);

            TypeInfoMap::iterator it = _typeMap.find(typeName);

            if (it != _typeMap.end())
                return;

            _typeMap[typeName] = ti;

            _cppTypeMap[typeid(T).name()] = typeName;
        }

        template<typename T>
        T* create(const std::string& typeName)
        {
            TypeInfoMap::iterator it = _typeMap.find(typeName);

            if (it == _typeMap.end())
                return 0;

            return (T*) it->second->create();
        }

        std::string typeName(const std::string& cppType) const
        {
            auto it = _cppTypeMap.find(cppType);

            if(it != _cppTypeMap.end())
                return it->second;

            return "";
        }

    private:
        TypeInfoMap _typeMap;
        CppTypeMap _cppTypeMap;
};

template<typename T>
class TypeInit
{
    public:
        TypeInit(const std::string& name)
        {
            TypeFactory::instance().regType<T>(name);
        }
};


}}}

#endif

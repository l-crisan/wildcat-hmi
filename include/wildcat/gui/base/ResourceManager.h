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
#ifndef WILDCAT_GUI_BASE_RESOURCEMANAGER_H
#define WILDCAT_GUI_BASE_RESOURCEMANAGER_H

#include <wildcat/gui/base/Api.h>
#include <wildcat/gui/base/Resource.h>
#include <map>
#include <string>
#include <vector>
#include <iostream>

namespace wildcat {
namespace gui {
namespace base {

class WILDCAT_GUI_BASE_API ResourceManager
{
    public:
        friend WILDCAT_GUI_BASE_API std::ostream& operator<<(std::ostream& os, const ResourceManager& rm);

        friend WILDCAT_GUI_BASE_API std::istream& operator>>(std::istream& is, ResourceManager& rm);

    public:
        ResourceManager();

        virtual ~ResourceManager();
        
        const Resource* getResource(uint32_t id) const;

        const Resource* getResource(const std::string& name) const;

        Resource* getResource(uint32_t id);

        Resource* getResource(const std::string& name);


        uint32_t add(Resource& resource);

        uint32_t add(Resource& resource, const std::string& name);

        void remove(uint32_t id);   

        bool contains(uint32_t id) const;

        uint32_t contains(const std::string& name) const;

        bool contains(Resource& resource) const;

        void clear();   

    public:

        template<typename T>
        const T* get(uint32_t id) const
        {
            return dynamic_cast<const T*>(getResource(id));
        }

        template<typename T>
        const T* get(const std::string& name) const
        {
            return dynamic_cast<const T*>(getResource(id(name)));
        }

        template<typename T>
        std::vector<const T*> get() const
        {
           std::vector<const T*> result;

            auto it = _map.begin();

            for(; it != _map.end(); ++it)
            {
                const T* p = dynamic_cast<const T*>(it->second);
                if(p)
                    result.push_back(p);
            }
            return result;
        }

        template<typename T>
        std::vector<T*> getAll() const
        {
            std::vector<T*> result;

            auto it = _map.begin();

            for (; it != _map.end(); ++it)
            {
                T* p = dynamic_cast<T*>(it->second);
                if (p)
                    result.push_back(p);
            }
            return result;
        }


        template<typename T>
        T* get(uint32_t id)
        {
            return dynamic_cast<T*>(getResource(id));
        }

        template<typename T>
        T* get(const std::string& name) 
        {
            return dynamic_cast<T*>(getResource(id(name)));
        }

        template<typename T>
        T* getOrCreate(const std::string& name)
        {
            T* resource = get<T>(name);

            if (!resource)
            {
                resource = new T();
                add(*resource, name);
            }
            return resource;
        }

    public:
        static uint32_t id(const std::string& name);

    private:
        typedef std::map<uint32_t, Resource*> ResourceMap;
        ResourceMap _map;

};

WILDCAT_GUI_BASE_API std::ostream& operator<<(std::ostream& os, const ResourceManager& rm);

WILDCAT_GUI_BASE_API std::istream& operator>>(std::istream& is, ResourceManager& rm);

}}}

#endif
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
#include <wildcat/gui/base/ResourceManager.h>
#include <wildcat/gui/base/TypeFactory.h>
#include <wildcat/gui/base/String.h>
#include <wildcat/gui/base/Hash.h>
#include <wildcat/gui/base/File.h>
#include <Pt/System/Directory.h>
#include <Pt/System/FileInfo.h>
#include <fstream>
#include <iostream>

namespace wildcat {
namespace gui {
namespace base {

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
    clear();
}

void ResourceManager::clear()
{
    auto it = _map.begin();

    for(; it != _map.end(); ++it)
        delete it->second;

    _map.clear();
}
 Resource* ResourceManager::getResource(const std::string& name)
 {
    return getResource(this->id(name));
}

Resource* ResourceManager::getResource(uint32_t id)
{
    if (id == 0)
        return nullptr;

    auto it = _map.find(id);

    if (it != _map.end())
        return it->second;

    return nullptr;
}

const Resource* ResourceManager::getResource(const std::string& name) const
{
    return getResource(this->id(name));
}

const Resource* ResourceManager::getResource(uint32_t id) const
{
    if( id == 0)
        return nullptr;

    auto it = _map.find(id);

    if( it != _map.end())
        return it->second;

    return nullptr;
}

uint32_t ResourceManager::add(Resource& resource)
{
    const std::string& name = resource.name();

    if( name.empty())
        return 0;

    return add(resource, name);
}


uint32_t ResourceManager::add(Resource& resource, const std::string& name)
{
    const uint32_t id = base::Hash::gen32(name);
    
    if( id== 0)
        return 0;   

    auto it = _map.find(id);

    if (it != _map.end())
    {
        delete it->second;
        _map.erase(it); 
    }
    
    resource.setName(name);
    _map[id] = &resource;

    return id;
}   

uint32_t ResourceManager::id(const std::string& name)
{
    return base::Hash::gen32(name); 
}

void ResourceManager::remove(uint32_t id)
{
    if( id == 0)
        return; 

    auto it = _map.find(id);

    if( it != _map.end())
    {
        delete it->second;
        _map.erase(it);
    }   
}

bool ResourceManager::contains(uint32_t id) const
{
    if( id == 0)
        return false;   

    auto it = _map.find(id);
    return it != _map.end();
}

uint32_t ResourceManager::contains(const std::string& name) const
{
    const uint32_t id = this->id(name);

    if( id == 0)
        return 0;   

    auto it = _map.find(id);

    if(it != _map.end())
        return it->first;

    return 0;
}

bool ResourceManager::contains(Resource& resource) const
{
    auto it = _map.begin();

    for (; it != _map.end(); ++it)
    {
        if( it->second == &resource)
            return true;    
    }

    return false;
}

WILDCAT_GUI_BASE_API std::ostream& operator<<(std::ostream& os, const ResourceManager& rm)
{
    auto it = rm._map.begin();

    os << "(" << rm._map.size() << "," << std::endl;

    for(;it!= rm._map.end(); ++it)
    {
        Resource* res = it->second;

        std::string type = TypeFactory::instance().typeName(typeid(*res).name());

        os << "(" << type << "," << it->first << ","<< *res<<")"<<std::endl;
    }
    os << ")" << std::endl;
    return os;
}

WILDCAT_GUI_BASE_API std::istream& operator>>(std::istream& is, ResourceManager& rm)
{
    char ch;
    
    std::string sizeStr;
    std::string type;
    std::string id;

    rm.clear();

    is.read(&ch, 1); //"("
    
    std::getline(is, sizeStr, ',');
    base::File::skippWhiteSpace(is);    
      
    
    size_t count = std::stoi(sizeStr);

    for(size_t i = 0; i < count; ++i)
    {
        
        is.read(&ch, 1); //"("

        std::getline(is, type, ',');

        std::getline(is, id, ',');
        
        uint32_t resourceId = base::String::to<uint32_t>(id);   

        Resource* rs = TypeFactory::instance().create<Resource>(type);
        
        is>> *rs;
        rm._map[resourceId] = rs;

        is.read(&ch, 1);//")"
        base::File::skippWhiteSpace(is);

    }

    is.read(&ch, 1);//")"
    return is;  
}

}}}


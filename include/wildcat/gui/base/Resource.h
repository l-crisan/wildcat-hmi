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
#ifndef WILDCAT_GUI_BASE_RESOURCE_H
#define WILDCAT_GUI_BASE_RESOURCE_H

#include <wildcat/gui/base/Api.h>
#include <iostream>

namespace wildcat {
namespace gui {
namespace base {

class WILDCAT_GUI_BASE_API Resource
{
    friend std::ostream& operator<<(std::ostream& os, const Resource& rm);

    friend std::istream& operator>>(std::istream& is, Resource& rm);

    public:
        Resource();

        Resource(const std::string& name);

        virtual ~Resource();

        const std::string& name() const
        {
            return onGetName();
        }   

        void setName(const std::string& name)
        {
            onSetName(name);
        }

        void append(Resource* resource)
        {
            if( resource == nullptr)
                return;

            onAppend(resource);
        }   

    protected:

        virtual void onAppend(Resource* resource)
        { 
            delete resource;        
        }

        virtual void onSave(std::ostream& os) const = 0;

        virtual void onLoad(std::istream& is) = 0;

        virtual const std::string& onGetName() const;

        virtual void onSetName(const std::string& name);
        

    private:
        std::string _name;
        
};

inline std::ostream& operator<<(std::ostream& os, const Resource& rm)
{
    rm.onSave(os);
    return os;  
}

inline  std::istream& operator>>( std::istream& is, Resource& rm)
{
    rm.onLoad(is);
    return is;  
}

}}}

#endif
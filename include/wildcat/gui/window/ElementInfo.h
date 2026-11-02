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
#ifndef WILDCAT_GUI_WINDOW_ELEMENTINFO_H
#define WILDCAT_GUI_WINDOW_ELEMENTINFO_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/gfx/Icon.h>


namespace wildcat {
namespace gui {
namespace window {

class WILDCAT_GUI_WINDOW_API ElementInfo
{
    public:
        ElementInfo();

        ElementInfo(const std::string& name, const std::string& type, const std::string& cat,  const std::string& desc = "", const gfx::Pixmap icon = gfx::Pixmap());

        virtual ~ElementInfo();


        const gfx::Pixmap& icon() const
        {
            return _icon;
        }

        void setIcon(const gfx::Pixmap& icon)
        {
            _icon = icon;
            _icon.setName(type());
        }

        const std::string& name() const
        {
            return _name;
        }

        void setName(const std::string& n)
        {
            _name = n;
        }

        const std::string& type() const
        {
            return _type;
        }

        void setType(const std::string& n)
        {
            _type = n;
        }


        const std::string& categoty() const
        {
            return _categoty;
        }

        void setCategoty(const std::string& n)
        {
            _categoty = n;
        }

        const std::string& description() const
        {
            return _descr;
        }

        void setDescription(const std::string& n)
        {
            _descr = n;
        }

        bool operator==(const ElementInfo& a)
        {
            return _name == a.name();
        }

        bool operator<(const ElementInfo& a)
        {
            return _name < a.name();
        }

    private:
        std::string _name;
        std::string _type;
        std::string _categoty;
        std::string _descr;
        gfx::Pixmap _icon;
};


}}}

#endif

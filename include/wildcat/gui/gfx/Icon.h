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
#ifndef WILDCAT_GUI_GFX_ICON_H
#define WILDCAT_GUI_GFX_ICON_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/base/Resource.h>  
#include <Pt/System/Path.h>

#include <vector>

namespace wildcat {
namespace gui {
namespace gfx {

class IconImpl;


class WILDCAT_GUI_GFX_API Icon : public wildcat::gui::base::Resource
{
    public:
        Icon();

        Icon(const gfx::Pixmap& img);

        Icon(const Icon& icon);

        virtual ~Icon();

        Icon& operator=(const Icon& icon);

        bool empty() const;

        void clear();

        void addImage(const gfx::Pixmap& image);

        void addImage(const gfx::Size& size, const gfx::Pixmap& image);

        const gfx::Pixmap& getImage(double scaling = 1) const;

        gfx::Size minimumSize() const;

        gfx::Size maximumSize() const;

        const Size& size() const;

        void load(const char* file, ImageFileType ftype);

        void loadXpm(const char** xpmData);

        void loadXpm(char** xpmData);

        size_t count() const;

        const gfx::Pixmap& image(size_t index) const; 

        gfx::Pixmap& image(size_t index);

        void remove(size_t index);

        void invert();

        void setInvertible(bool v);

        bool isInvertible() const;  


    protected:

        virtual void onSave(std::ostream& os) const
        {
            os<<*this;
        }

        virtual void onLoad(std::istream& is)
        {
            is >> *this;    
        }


    private:
        mutable class IconImpl* _impl;

    private:
        static wildcat::gui::base::TypeInit<Icon> _typeInit;
};

WILDCAT_GUI_GFX_API std::ostream& operator<<(std::ostream& os, const Icon& obj);

WILDCAT_GUI_GFX_API  std::istream& operator>>(std::istream& is, Icon& obj);


}}}

#endif

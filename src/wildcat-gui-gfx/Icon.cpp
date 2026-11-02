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
#include <wildcat/gui/gfx/Icon.h>
#include <wildcat/gui/gfx/PngReader.h>
#include <wildcat/gui/gfx/IcoReader.h>
#include <Pt/System/FileInfo.h>
#include <map>
#include <algorithm>
#include <cstddef>
#include <fstream>

namespace wildcat {
namespace gui {
namespace gfx {


static gfx::Pixmap gEmptyImage;

class IconImpl
{
    public:
        IconImpl()
        :_isInvertible(true)
        { 
        }

        IconImpl(const IconImpl& other)
        { 
          
        }

        virtual ~IconImpl()
        {
           
        }

        bool empty() const
        {
            return _images.empty(); 
        }

        void clear()
        {
            _images.clear();
        }

        void addImage(const gfx::Size& size, const gfx::Pixmap& image)
        {
            _images[size] = image;
        }

        const gfx::Pixmap& getImage(const gfx::Size& size)
        {         
            if (_images.empty())            
                return gEmptyImage;            

            auto it = _images.find(size);   

            if( it != _images.end())
                return it->second;

            it = _images.begin();

            for( ; it != _images.end(); ++it)
            {
                if( it->first.width() >= size.width() && it->first.height() >= size.height())
                    return it->second;
            }

            return _images.begin()->second; 
        }

        const gfx::Pixmap& getImage()
        {
            if (_images.empty())
                return gEmptyImage;

            return _images.begin()->second; 
        }

        gfx::Size minimumSize() const
        {
            if (_images.empty())
                return gfx::Size();

            return _images.begin()->first;
        }

        gfx::Size maximumSize() const
        {
            if (_images.empty())
                return gfx::Size();

            return _images.rbegin()->first;
        }

        const Size& size() const
        {                
            if (_images.empty())
            {
                static gfx::Size s;
                return s;
            }

            return _images.begin()->first;
        }

        size_t count() const
        {
            return _images.size();
        }   

        gfx::Pixmap& image(size_t index)
        {
            if (index >= _images.size())
                throw std::out_of_range("IconImpl::image: index out of range");

            auto it = _images.begin();

            std::advance(it, index);

            return it->second;
        }

        const gfx::Pixmap& image(size_t index) const
        {
            if (index >= _images.size())
                throw std::out_of_range("IconImpl::image: index out of range");

            auto it = _images.begin();

            std::advance(it, index);

            return it->second;
        }

        void remove(size_t index)
        {
            if (index >= _images.size())
                return;

            auto it = _images.begin();

            std::advance(it, index);

            _images.erase(it);
        }   

        bool isInvertible() const
        {
            return _isInvertible;
        }   

        void setInvertible(bool v)
        {
            _isInvertible = v;
        }   

        void invert()
        {
            if (!_isInvertible)     
                return;

            for (auto& pair : _images)
                pair.second.invert();            
        }   

    private:
        std::map<gfx::Size, gfx::Pixmap> _images;   
        bool _isInvertible;
};


wildcat::gui::base::TypeInit<Icon> Icon::_typeInit("wildcat.gfx.Icon");

Icon::Icon()
: _impl(0)
{
    _impl = new IconImpl();
}

Icon::Icon(const gfx::Pixmap& img)
{
    _impl = new IconImpl();
    addImage(img);
}

Icon::~Icon()
{
    delete _impl;
}

Icon::Icon(const Icon& icon)
: _impl(0)
{
    _impl = new IconImpl();
    *_impl = *icon._impl;    
    setName(icon.name());
}

Icon& Icon::operator=(const Icon& other)
{
    *_impl = *other._impl;
    setName(other.name());  
    return *this;
}

bool Icon::empty() const
{ 
    return _impl->empty();
}

void Icon::clear()
{
    _impl->clear();
}

void Icon::addImage(const gfx::Pixmap& image)
{
    gfx::Size size( image.width(), image.height() );
    addImage(size, image);
}

void Icon::addImage(const gfx::Size& size, const gfx::Pixmap& image)
{ 
    if( empty())
        setName(image.name());

    _impl->addImage(size, image);
}

size_t Icon::count() const
{
    return _impl->count();
}

const gfx::Pixmap& Icon::image(size_t index) const
{
    return _impl->image(index);
}

gfx::Pixmap& Icon::image(size_t index)
{
   return _impl->image(index);
}


void Icon::remove(size_t index)
{
    _impl->remove(index);
}

void Icon::invert()
{
    _impl->invert();
}

void Icon::setInvertible(bool v)
{
    _impl->setInvertible(v);
}

bool Icon::isInvertible() const
{
    return _impl->isInvertible();
}   

void Icon::load(const char* file, ImageFileType ftype)
{
    if (ftype == ImageFileType::Ico)
    {
        std::fstream fs(file, std::ios::in | std::ios::binary);
        gfx::IcoReader reader(fs, *this);
        reader.read();
    }
    else
    {
        gfx::Pixmap image;
        image.load(file, ftype);
        if (image.empty())
            return;
        addImage(image);
    }
}

void Icon::loadXpm(const char** xpmData)
{
    gfx::Pixmap image;  
    image.loadXpm(xpmData);

    if( image.empty())
        return;

    addImage(image);    
}

void Icon::loadXpm(char** xpmData)
{
    gfx::Pixmap image;
    image.loadXpm(xpmData);

    if (image.empty())
        return;

    addImage(image);
}

const gfx::Pixmap& Icon::getImage(double scaling) const
{
    Size is = size();

    is.scale(scaling);

    return _impl->getImage(is);
}

const Size& Icon::size() const
{
    return _impl->size();
}

gfx::Size Icon::minimumSize() const
{
    return _impl->minimumSize();
}

gfx::Size Icon::maximumSize() const
{
    return _impl->maximumSize();
}


WILDCAT_GUI_GFX_API std::ostream& operator<<(std::ostream& os, const Icon& obj)
{
    os << '(';
    os << obj.isInvertible() << ',';    
    os << obj.count() << ',';

    for(size_t i = 0; i < obj.count(); ++i)
    {
        const gfx::Pixmap& img = obj.image(i);

        os << '(' << img.size() << ',' << img << ')';

        if (i < obj.count() - 1)
            os << ',';
    }

    os << ')';
    return os;
}

WILDCAT_GUI_GFX_API  std::istream& operator>>(std::istream& is, Icon& obj)
{
    char ch;
    bool invertible = true;
    obj.clear();

    is >> ch;    
    is>>invertible;
    obj.setInvertible(invertible);

    is >> ch;   
    size_t count = 0;
    is >> count;

    is >> ch;

    for (size_t i = 0; i < count; ++i)
    {
        is >> ch;

        gfx::Size size;
        is >> size;

        is >> ch;

        gfx::Pixmap img;
        is >> img;

        is >> ch;

        obj.addImage(size, img);

        if (i < count - 1)
            is >> ch;
    }

    is >> ch;

    return is;
}

}}}

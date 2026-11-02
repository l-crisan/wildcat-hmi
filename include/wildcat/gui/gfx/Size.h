/*
   Copyright (C) 2017 Marc Boris Duerner
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
#ifndef WILDCAT_GUI_GFX_SIZE_H
#define WILDCAT_GUI_GFX_SIZE_H

#include <wildcat/gui/gfx/Api.h>
#include <wildcat/gui/gfx/Point.h>

namespace wildcat {
namespace gui {
namespace gfx {

template<typename T>
class BasicSize {
    public:
        typedef T ValueT;

    public:
        BasicSize()
        : _w(0), _h(0)
        {}

        /** @brief Construct with width and height.
        */
        BasicSize(T w, T h)
        : _w(w), _h(h)
        {}

        void set(T w, T h)
        {
            _w = w;
            _h = h;
        }

        bool isNull() const
        {
            return _w == 0 || _h == 0;
        }

        //! @brief Returns the width
        inline T width() const
        { return _w; }

        //! @brief Returns the height
        inline T height() const
        { return _h; }

        //! @brief Sets the width
        inline void setWidth(T w)
        { _w = w; }

        //! @brief Sets the height
        inline void setHeight(T h)
        { _h = h; }

        //! @brief Sets the widht and height.
        inline void setWidthHeight(T w, T h)
        {
            _w = w;
            _h = h;
        }

        //! @brief Increment the width of the BasicSize by the given value
        const BasicSize& addWidth(T w)
        {
            _w += w;
            return *this;
        }

        //! @brief Decrement the width of the BasicSize by the given value
        const BasicSize& subWidth(T w)
        {
            _w -= w;
            return *this;
        }

        //! @brief Increment the height of the BasicSize by the given value
        const BasicSize& addHeight(T h)
        {
            _h += h;
            return *this;
        }

        //! @brief Decrement the height of the BasicSize by the given value
        const BasicSize& subHeight(T h)
        {
            _h -= h;
            return *this;
        }

        void add(const BasicSize& s)
        {
            _w += s._w;
            _h += s._h;
        }

        BasicSize& operator=(const BasicSize& other)
        {
            _w = other._w; _h = other._h;
            return *this;
        }

        BasicSize& operator+=(const BasicSize& s)
        {
            _w += s._w;
            _h += s._h;
            return *this;
        }

        bool operator==(const BasicSize& other) const
        {
            return (_w == other._w && _h == other._h);
        }

        bool operator!=(const BasicSize& other) const
        {
            return (_w != other._w || _h != other._h);
        }

        bool operator>(const BasicSize& other) const
        {
            return _h > other._h || (_h == other._h && _w > other._w);
        }

        bool operator<(const BasicSize& other) const
        {
            return _h < other._h || (_h == other._h && _w < other._w);
        }

        BasicSize operator*(T v) const
        {
            return BasicSize(_w *v, _h * v);
        }

        BasicSize operator/(T v) const
        {
            return BasicSize(_w/v, _h/v);
        }

        BasicSize operator+(T v) const
        {
            return BasicSize(_w +v, _h+v);
        }

        BasicSize operator-(T v) const
        {
            return BasicSize(_w - v, _h - v);
        }

        BasicSize& operator*=(T v)
        {
            _w *= v;
            _h *= v;
            return *this;
        }

        BasicSize& operator/=(T v)
        {
            _w /= v;
            _h /= v;
            return *this;
        }

        BasicSize& operator+=(T v)
        {
            _w += v;
            _h += v;
            return *this;
        }

        BasicSize& operator-=(T v)
        {
            _w -= v;
            _h -= v;
            return *this;
        }


        BasicSize operator+(const BasicSize& v)
        {
            BasicSize s(*this); 
            s._w += v._w;
            s._h += v._h;
            return s;
        }

        BasicSize operator-(const BasicSize& v)
        {
            BasicSize s(*this);
            s._w -= v._w;
            s._h -= v._h;
            return s;
        }

        BasicPoint<T> center(const BasicSize& s) const
        {
            return BasicPoint<T>(_w/2 - s._w/2, _h/2 - s._h/2);
        }

        void scale(double factor)
        {
            _w = static_cast<T>(std::round(_w * factor));
            _h = static_cast<T>(std::round(_h * factor));
        }   

    protected:
        T _w;
        T _h;
};


typedef BasicSize<int> Size;
typedef BasicSize<double> SizeF;


template<typename T>
inline std::ostream& operator<<(std::ostream& os, const BasicSize<T>& obj)
{
    os << "(" << obj.width();
    os << ',';
    os << obj.height() << ")";
    return os;
}


template<typename T>
inline std::istream& operator>>(std::istream& is, BasicSize<T>& obj)
{
    char ch;
    T w, h;
    is >> ch;
    is >> w;
    is >> ch;
    is >> h;
    is >> ch;

    obj.setWidth(w);
    obj.setHeight(h);
    return is;
}


}}}

#endif

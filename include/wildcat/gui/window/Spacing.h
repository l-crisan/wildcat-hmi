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
#ifndef WILDCAT_GUI_WINDOW_SPACING_H
#define WILDCAT_GUI_WINDOW_SPACING_H

#include <wildcat/gui/window/Api.h>
#include <Pt/SerializationInfo.h>

namespace wildcat{
namespace gui {
namespace window {

class Spacing 
{
  public:
    Spacing(int all)
    {
      set(all);
    }

    Spacing(int horizontal, int vertical)
    {
      set(horizontal, vertical);
    }

    Spacing(int left, int top, int right, int bottom)
    {
      set(left, top, right, bottom);
    }

    Spacing()    
    {
      set( 0, 0, 0, 0);
    }

    void set(int value)
    {
      set(value, value);
    }

    void set(int horizontal, int vertical)
    {
      _left = horizontal;
      _top = vertical;
      _right = horizontal;
      _bottom = vertical;
    }

    void set(int left, int top, int right, int bottom)
    {
      _left = left;
      _top = top;
      _right = right;
      _bottom = bottom;
    }

    int left() const
    {
      return _left;
    }
    
    void setLeft(int left)
    {
      _left = left;
    }

    int topBottom() const
    {
      return _top + _bottom;
    }

    int leftRight() const
    {
      return _left + _right;
    }

    int top() const
    {
      return _top;
    }
    
    void setTop(int top)
    {
      _top = top;
    } 

    int right() const
    {
      return _right;
    }
    
    void setRight(int right)
    {
      _right = right;
    } 

    int bottom() const
    {
      return _bottom;
    }
    
    void setBottom(int bottom)
    {
      _bottom = bottom;
    }

    bool operator==(const Spacing& s) const
    {
        return (s._left == _left)   &&
               (s._top == _top)     &&
               (s._right == _right) &&
               (s._bottom == _bottom);
    }

  private:
      int _left;
      int _top;
      int _right;
      int _bottom;
};



inline std::ostream& operator<<(std::ostream& os, const Spacing& obj)
{
    os << "(" << obj.left()<< ','<<obj.top() << ',' << obj.right() << ',' << obj.bottom()<<')';
    return os;
}


inline std::istream& operator>>(std::istream& is, Spacing& obj)
{
    char ch;
    int l, t, r, b;
    is >> ch;
    is >> l;
    is >> ch;
    is >> t;
    is >> ch;
    is >> r;
    is >> ch;
    is >> b;
    is >> ch;

    obj.set(l, t, r, b);
    return is;
}

}}}

#endif

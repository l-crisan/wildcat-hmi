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
#ifndef WILDCAT_GUI_WINDOW_EVENT_H
#define WILDCAT_GUI_WINDOW_EVENT_H

#include <wildcat/gui/window/Api.h>
#include <wildcat/gui/window/Element.h>
#include <Pt/Types.h>
#include <Pt/Event.h>

namespace wildcat {
namespace gui {
namespace window {

template <typename T>
class Event : public Pt::BasicEvent<T>
{
    public:
        Event(gui::window::Element& element)
        : _id(element.id())
        , _element(&element)
        {
        }

        Event(uint64_t id = 0)
        : _id(id)
        , _element(0)
        {
        }


        virtual ~Event()
        {
        }

        uint64_t id() const
        {
            return _id;
        }

        gui::window::Element* element() const
        {
            return _element;
        }

        void setElement(gui::window::Element* v)
        {
            _element = v;
            _id = v ? v->id() : 0;
        }

        void setId(uint64_t v)
        {
            _id = v;
        }

    private:
        uint64_t _id;
        gui::window::Element* _element;       
};

}}}

#endif

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
#ifndef WILDCAT_GUI_GFX_XPMLEXER_H
#define WILDCAT_GUI_GFX_XPMLEXER_H

#include <wildcat/gui/gfx/Api.h>
#include <iostream>
#include <string>

namespace wildcat {
namespace gui {
namespace gfx {

class XpmToken
{
    public:
        enum Type
        {
            None,
            Variable,
            Comment,
            String,
            End
        };

        XpmToken();

        XpmToken(const std::string& v, Type t = String, int line = 0, int col = 0);

        virtual ~XpmToken();

        const std::string text() const
        {
            return _text;
        }

        void setText(const std::string& v)
        {
            _text = v;
        }

        Type type() const
        {
            return _type;
        }

        void setType(Type v)
        {
            _type = v;
        }


        int line() const
        {
            return _line;
        }
        void setLine(int v)
        {
            _line = v;
        }

        int col() const
        {
            return _col;
        }

        void setCol(int v)
        {
            _col = v;
        }

        void init(const std::string& v, Type t = String, int line = 0, int col = 0);
        
        void reset()
        {
            _text.clear();
            _type = Type::None;
            _line = 0;
            _col = 0;
        }

        void incLine()
        {
            _line++;
            _col = 0;

        }

        void incCol()
        {
            _col++;
        }

    private:
        std::string _text;
        Type _type;
        int _line;
        int _col;
};



class XpmLexer
{
    public:
        XpmLexer( std::istream& stream, bool raw = true);

        XpmLexer();

        virtual ~XpmLexer();

        void reset(std::istream* is = 0, bool raw = true);
   
        const XpmToken& readNext();

        void setSkipComment(bool v)
        {
            _skipComment = v;
        }
    private:

        void skipSpaces();

        void readComment();

        void readVariable();

        void readString();

        char getChar();

        void ungetChar();

        void readWord();

    private:
        XpmToken _current;
        std::istream*  _is;
        bool _raw;
        long _beginPos;
        int _line;
        int _col;
        char _currentChar;
        bool _skipComment;
};

}}}

#endif

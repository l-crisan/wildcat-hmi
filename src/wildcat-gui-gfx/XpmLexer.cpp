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
#include "XpmLexer.h"
#include <sstream>

namespace wildcat {
namespace gui {
namespace gfx {


bool isspace(char ch)
{

    switch (ch)
    {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
    case '\f':
    case '\v':
        return true;
    }

    return false;
}



XpmToken::XpmToken()
: _text()
, _type(None)
, _line(0)
, _col(0)
{

}

XpmToken::XpmToken(const std::string& v, Type t, int line, int col)
{
    init(v, t, line, col);
}

XpmToken::~XpmToken()
{

}

void XpmToken::init(const std::string& v, Type t, int line, int col)
{
    _text = v;
    _type = t;
    _line = line;
    _col = col;

}


XpmLexer::XpmLexer(std::istream& stream, bool raw)
: _current()
, _is(&stream)
, _raw(raw)
, _beginPos(0)
, _line(0)
, _col(0)
, _skipComment(true)
{
    _beginPos = _is->tellg();
}

XpmLexer::XpmLexer()
: _current()
, _is(0)
, _raw(false)
, _beginPos(0)
, _line(0)
, _col(0)
, _skipComment(true)
{

}


XpmLexer::~XpmLexer()
{

}

void XpmLexer::reset(std::istream* is, bool raw)
{
    _raw = raw;
   _line = 0;
   _col = 0;
    
    if( is == 0)
    {
        if( _is)
            _is->seekg( _beginPos, std::ios_base::beg);

        return;
    }

    _is = is;
    _beginPos = _is->tellg();
}

char XpmLexer::getChar()
{
    _currentChar = _is->get();

    if (_currentChar == '\n')
    {
        _line++;
        _col = 0;
    }
    else
    {
        _col++;
    }

    return _currentChar;
}


void XpmLexer::ungetChar()
{
    _is->unget();
   
     _col--;

_currentChar = '\0';
}


void XpmLexer::skipSpaces()
{
    getChar();

    while (*_is && isspace((unsigned)_currentChar))
        getChar();

    ungetChar();
}

void XpmLexer::readComment()
{
    _current.setType(XpmToken::Comment);
    _current.setLine(_line);
    _current.setCol(_col);

    std::string comment = "";

    while (true)
    {
        getChar();

        if (_currentChar == '*')
        {
            getChar();

            if (_currentChar == '/')
                break;


            comment += '*';
            ungetChar();
        }
        else
        {
            comment += _currentChar;
        }
    }

    _current.setText(comment);
}

void XpmLexer::readVariable()
{
    _current.setType(XpmToken::Variable);
    _current.setLine(_line);
    _current.setCol(_col);

    std::string variable = "";
    ungetChar(); //'s'

    while (true)
    {
        getChar();

        if (_currentChar == '{')
            break;

        if (_currentChar == '/')
            readComment();

        variable += _currentChar;
    }

    _current.setText(variable);
}

void XpmLexer::readString()
{
    _current.setType(XpmToken::String);
    _current.setLine(_line);
    _current.setCol(_col);

    std::string text = "";    

    while (true)
    {
        getChar();

        if (_currentChar == '"')
            break;

        text += _currentChar;
    }

    skipSpaces();
    getChar();
    

    _current.setText(text);
}


const XpmToken& XpmLexer::readNext()
{

    _current.setType(XpmToken::None);

    if (_skipComment)
    {
        do
        {
            readWord();
        }
        while (_current.type() == XpmToken::Comment);
    }
    else
    {
        readWord();
    }

    return _current;
}


void XpmLexer::readWord()
{
    skipSpaces();
    getChar();

    switch (_currentChar)
    {
        case '/':
            getChar();

            if (_currentChar == '*')
            {
                readComment();
            }
            else
            {
                std::stringstream ss;

                ss<< "Syntax error (" << _line << "," << _col << ")";
                throw std::logic_error(ss.str());   
            }

        break;

        case 's':
            readVariable();
            break;

        case '"':
            readString();
        break;

        default:
            _current.setType(XpmToken::None);
        break;
    }
}

}}}



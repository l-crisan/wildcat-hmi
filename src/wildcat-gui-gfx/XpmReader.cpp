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
#include <wildcat/gui/gfx/XpmReader.h>
#include <wildcat/gui/gfx/Pixmap.h>
#include <wildcat/gui/gfx/Size.h>
#include <wildcat/gui/base/String.h>
#include "XpmLexer.h"
#include <fstream>
#include <sstream>

namespace wildcat {
namespace gui {
namespace gfx {

class XpmColor
{
    public:
        XpmColor()
        {

        }

        std::string symbolic;   // symbolic name
        std::string monoColor;	// monochrom default
        std::string g4Color;    // 4 level grayscale default
        std::string gColor;		// other level grayscale default
        std::string color;		// color default
};

typedef std::map<std::string, XpmColor> XpmColorTable;


class XpmReaderImpl
{
    public:
        XpmReaderImpl(Pixmap& image);

        void read(std::istream& stream, bool raw);

        void read( const char* stream[]);

        const Point& hotspot() const
        {
            return _hotspot;
        }

    private:
        void parseVariable(const XpmToken& token);

        void parseHeader(const XpmToken& token);

        void parseColorTableRow(const XpmToken& token);

        void parsePixelRow(const XpmToken& token, int row);
        
        ArgbColor toGfxColor( const XpmColor& xpm);
            
        ArgbColor toGfxColor(const std::string& c)
        {
            return toGfxColor( _colorTable[c]);
        }

    private:
        Pixmap& _image;
        std::istream* _stream;
        XpmColorTable _colorTable;
        XpmLexer _lexer;
        std::string _variable;
        Point _hotspot;
        int _colorTableSize;
        int _cpp;
};

XpmReaderImpl::XpmReaderImpl(Pixmap& image)
: _image(image)
, _stream(0)
, _colorTable()
, _lexer()
, _colorTableSize(0)
, _cpp(0)
{

}

ArgbColor XpmReaderImpl::toGfxColor(const XpmColor& xpm)
{
    if (!xpm.color.empty())
    {
        return ArgbColor::fromName(xpm.color);
    }
    else if (!xpm.monoColor.empty())
    {
        return ArgbColor::fromName(xpm.monoColor);
    }
    else if (!xpm.g4Color.empty())
    {
        return ArgbColor::fromName(xpm.g4Color);
    }
    else if (!xpm.gColor.empty())
    {
        return ArgbColor::fromName(xpm.gColor);
    }

    return ArgbColor(0, 0, 0, 0);
}

void XpmReaderImpl::read(std::istream& stream, bool raw)
{
    _stream = &stream;

    if( raw)
    { 
        int line = 1;
        std::string txt;
        std::getline(stream, txt);

        XpmToken tk(txt, XpmToken::String, line, 1);
        parseHeader(tk);
        line++;

        _colorTable.clear();

        for (size_t i = 0; i < _colorTableSize; ++i, ++line)
        {
            std::getline(stream, txt);
            tk.init(txt, XpmToken::String, line, 1);
            parseColorTableRow(tk);
        }

        for (size_t i = 0; i < _image.height(); ++i, ++line)
        {
            std::getline(stream, txt);
            tk.init(txt, XpmToken::String, line, 1);
            parsePixelRow(tk, i);
        }

    }
    else
    { 
         _lexer.setSkipComment(true);
         _lexer.reset(&stream, raw);
         
        XpmToken token = _lexer.readNext();

        if (token.type() != XpmToken::Variable)
        {
            std::stringstream ss;
            ss << "Unexpected symbol at (" << token.line() << "," << token.col() << ")";
            throw std::logic_error(ss.str());
        }

        parseVariable(token);

        token = _lexer.readNext();

        if (token.type() != XpmToken::String)
        {
            std::stringstream ss;
            ss << "Unexpected symbol at (" << token.line() << "," << token.col() << ")";
            throw std::logic_error(ss.str());
        }

        parseHeader(token);

        _colorTable.clear();

        for (size_t i = 0; i < _colorTableSize; ++i)
        {
            token = _lexer.readNext();

            parseColorTableRow(token);
        }

        for (size_t i = 0; i < _image.height(); ++i)
        {
            token = _lexer.readNext();

            parsePixelRow(token, i);
        }
    }
}

void XpmReaderImpl::read(const char* stream[])
{
    int line = 0;

    XpmToken tk(stream[line], XpmToken::String, line, 1);
    parseHeader(tk);
    line++;

    std::stringstream ss;

    _colorTable.clear();
    
    for (size_t i = 0; i < _colorTableSize; ++i, ++line)
    {
        tk.init(stream[line], XpmToken::String, line, 1);
   
         parseColorTableRow(tk);
    }

    for (size_t i = 0; i < _image.height(); ++i, ++line)
    {
        tk.init(stream[line], XpmToken::String, line, 1);

        parsePixelRow(tk, i);
    }
  
}

void XpmReaderImpl::parseColorTableRow(const XpmToken& token)
{
  std::string str = token.text();
  std::string color = str;

  color.erase(_cpp, color.size() - _cpp);
  str.erase( 0, _cpp);

  base::StringVector data = base::String::split<std::string>( str, " \t");

  if (data.size() % 2 != 0)
  {
      std::stringstream ss;
      ss << "Wrong format at (" << token.line() << "," << token.col() << ")";
      throw std::logic_error(ss.str());
  }

   XpmColor colorInfo;

  for (size_t i = 0; i < data.size(); i += 2)
  {
      std::string key = data[i];
      std::string value = data[i+1];

      if (key == "m")
      {
         colorInfo.monoColor = value;
      }
      else if (key == "s")
      {
          colorInfo.symbolic = value;
      }
      else if (key == "g4")
      {
          colorInfo.g4Color = value;
      }
      else if (key == "g")
      {
          colorInfo.gColor = value;
      }
      else if (key == "c")
      {
          colorInfo.color = value;
      }
  }

  _colorTable[color] = colorInfo;
}

void XpmReaderImpl::parsePixelRow(const XpmToken& token, int row)
{
    const std::string line = token.text().c_str();
    size_t offset = 0;

    for (size_t col = 0; col < _image.width(); ++col, offset += _cpp)
    {
        std::string colorText = "";

        for (size_t i = 0; i < _cpp; ++i)
            colorText += line[offset + i];

        ArgbColor cl = toGfxColor(colorText);
        _image.setColor(cl, Point(col, row));
    }
}

void XpmReaderImpl::parseVariable(const XpmToken& token)
{
   const std::string text = token.text();
   bool begin = false;
   _variable = "";

   for (size_t i = 0; i < text.size(); ++i)
   {
       if (!begin)
       {
           if( text[i] == '*')
                begin = true;
       }
       else
       {
           if (text[i] == '[')
           {
               break;
           }

            _variable += text[i];
       }
   }

   _variable = base::String::trim( _variable, ' ');

}

void XpmReaderImpl::parseHeader(const XpmToken& token)
{
    base::StringVector data = base::String::split<std::string>(token.text()," ");

    if( data.size() < 4)
    {
        std::stringstream ss;
        ss<<"Wrong format (" << token.line() << "," << token.col() << ")";
        throw std::logic_error(ss.str());
    }

    int width = base::String::to<int>( data[0]);
    int height = base::String::to<int>(data[1]);

    _image.resize(gfx::Size( width, height));

    _colorTableSize = base::String::to<int>(data[2]);
    _cpp = base::String::to<int>(data[3]);


    if (data.size() >= 6)
    {
        int x = base::String::to<int>(data[4]);

        int y = base::String::to<int>(data[4]);

        _hotspot.set(x,y);
    }
}

XpmReader::XpmReader(Pixmap& image)
: _impl( new XpmReaderImpl(image))
{
}

XpmReader::~XpmReader()
{
    delete _impl;
}

void XpmReader::read(const char* file)
{
    std::fstream is(file);
    read( is, false);
}

void XpmReader::read( std::istream& stream, bool raw)
{
    _impl->read(stream ,raw);
}

void XpmReader::read(const char* stream[])
{
    _impl->read(stream);
}

const Point& XpmReader::hotspot() const
{
    return _impl->hotspot();
}

}}}

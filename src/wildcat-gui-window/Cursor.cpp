/*
   Copyright (C) 2026 Marc Boris Duerner
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
#include <wildcat/gui/window/Cursor.h>
#include <wildcat/gui/window/Application.h>
#include <wildcat/gui/gfx/XpmReader.h>
#include <wildcat/gui/gfx/PixmapGraphics.h>
#include <wildcat/gui/gfx/JpegWriter.h>
#include <wildcat/gui/gfx/PngWriter.h>
#include <wildcat/gui/base/Hash.h>
#include <Pt/Byteorder.h>
#include <Pt/System/Path.h>
#include <fstream>
#include <sstream>


namespace wildcat {
namespace gui {
namespace window {

wildcat::gui::base::TypeInit<Cursor> Cursor::_typeInit("wildcat.gui.window.Cursor");

Cursor::Cursor()
: _width(0)
, _height(0)
, _xHotspot(0)
, _yHotspot(0)
, _dataHash(base::Hash::gen64())
, _spanHeight(16)
, _pixmap()
{
	
}

Cursor::Cursor(const Cursor& copy) 
: _andMask(copy._andMask)
, _xorMask	(copy._xorMask)
, _width(copy._width)
, _height(copy._height)
, _xHotspot(copy._xHotspot)
, _yHotspot(copy._yHotspot)
, _dataHash(copy._dataHash)
, _spanHeight(copy._spanHeight)
, _pixmap(copy._pixmap)
{
	setName(copy.name());
}

Cursor::~Cursor()
{

}


void Cursor::loadCursor( const char* xpmFile)
{
	clear();
	
	gfx::XpmReader reader(_pixmap);
	reader.read(xpmFile);

	update();

    setName(Pt::System::Path(xpmFile).baseName().narrow());		
	_xHotspot = reader.hotspot().x();
	_yHotspot = reader.hotspot().y();

}

void Cursor::loadCursor(std::istream& xpmStream, bool raw)
{
	clear();

	gfx::XpmReader reader(_pixmap);
	reader.read(xpmStream, raw);

	update();

	setName("");

	_xHotspot = reader.hotspot().x();
	_yHotspot = reader.hotspot().y();
}

void Cursor::loadCursor(const char* xpmBuffer[])
{						
	clear();

	gfx::XpmReader reader(_pixmap);
	reader.read(xpmBuffer);

	update();
	
	setName("");

	_xHotspot = reader.hotspot().x();
	_yHotspot = reader.hotspot().y();
}

void Cursor::loadCursor(char* xpmBuffer[])
{	
	clear();

	gfx::XpmReader reader(_pixmap);
	reader.read((const char**)xpmBuffer);

	update();

	setName("");
	_xHotspot = reader.hotspot().x();
	_yHotspot = reader.hotspot().y();
}

void Cursor::operator=(const Cursor& copy)
{
	if(this == &copy)
        return;

	_andMask	= copy._andMask;
	_xorMask	= copy._xorMask;
	_width		= copy._width;
	_height		= copy._height;
	_xHotspot	= copy._xHotspot;
	_yHotspot	= copy._yHotspot;
	_dataHash	= copy._dataHash;
	_spanHeight	= copy._spanHeight;
	_pixmap		= copy._pixmap;
	setName(copy.name());
}

Cursor* Cursor::createItemCursor(const Pt::String& text)
{	
	int xHotSpot =  5;
	int yHotSpot  = 5;
	gfx::PixmapGraphics gf;
	gfx::Font font = Application::instance().font();	

	gfx::FontMetrics fm = gfx::Graphics::fontMetrics(font, text);

    const int height = (fm.height() + 15)*2;	
	const gfx::Size imgSize(fm.width() + 15 + xHotSpot, height);
	gf.reset(imgSize);

	gf.clear(gfx::ArgbColor(0, 255, 255, 255));	 // Alpha  = 0 => background transparent

	const gfx::ArgbColor txtColor(0,0,0);
	
	const gfx::Point textPos(xHotSpot * 2 + 2, fm.height());

	const gfx::Rect textRect(gfx::Point(textPos.x()-1, 0), gfx::Size(fm.width()+5, fm.height()+10));

	//Draw hotspot cross
	const gfx::Point from1(0, yHotSpot);;
	const gfx::Point to1(xHotSpot * 2, yHotSpot);

	const gfx::Point from2(xHotSpot-1, 0);
	const gfx::Point to2(xHotSpot-1, yHotSpot * 2 + 1);

	//Outter
	gf.setPen(gfx::Pen(gfx::ArgbColor(0, 0, 0), 3));

	gf.drawLine(from1, to1);
	gf.drawLine(from2, to2);

	//Inner
	gf.setPen(gfx::Pen(gfx::ArgbColor(255, 255, 255), 1));

	gf.drawLine(from1, to1);
	gf.drawLine(from2, to2);
	

	//Draw frame
    gf.setBrush(gfx::ArgbColor(255, 255, 255));
	gf.setPen(gfx::ArgbColor(0,0,0));
	gf.fillRect(textRect);
	gf.drawRect(textRect);

	//Draw item text
	gf.setPen(txtColor);
	gf.setFont(font);

	gf.drawText(textPos, text);

	//Fill the cursor
	Cursor* csr = new Cursor();
	csr->fromImage(gf.image());
	csr->setXHotspot(xHotSpot);
	csr->setYHotspot(yHotSpot);
		
	return csr;
}

void Cursor::fromImage(const gfx::Pixmap& image)
{
	clear();

	_pixmap = image;
	update();
}

void Cursor::update()
{
	_height   = _pixmap.height();
	_width    = _pixmap.width();
    _dataHash = base::Hash::gen64(_pixmap.data(), _pixmap.byteSize());

	for( int y = 0; y < _height; ++y )
	{		
		for( int x = 0; x < _width; ++x )
		{
			const gfx::Point pixel(x, y);

			gfx::ArgbColor color = _pixmap.color(pixel);

			if( color.alpha() == 0 )
			{//Transparent
				_andMask.push_back( 0xff );
				_andMask.push_back( 0xff );
				_andMask.push_back( 0xff );				
				_andMask.push_back( 0xff );	

				_xorMask.push_back( 0 );
				_xorMask.push_back( 0 );
				_xorMask.push_back( 0 );
				_xorMask.push_back( 0);
			}
			else
			{
				_andMask.push_back( 0 );
				_andMask.push_back( 0 );
				_andMask.push_back( 0 );
				_andMask.push_back( 0 );
					
				_xorMask.push_back( (Pt::uint8_t) (color.blue()) );
				_xorMask.push_back( (Pt::uint8_t) (color.green()) );
				_xorMask.push_back( (Pt::uint8_t) (color.red()));
				_xorMask.push_back( 0xFF);
			}
		}
	}

    _spanHeight = _pixmap.height();

	for (int y = 0; y < _spanHeight; ++y)
	{
		for (int x = 0; x < _width ; ++x)
		{
			_andMask.push_back(0xff);
			_andMask.push_back(0xff);
			_andMask.push_back(0xff);
			_andMask.push_back(0xff);

			_xorMask.push_back(0);
			_xorMask.push_back(0);
			_xorMask.push_back(0);
			_xorMask.push_back(0);
		}
	}

	_height += _spanHeight;
}

void Cursor::clear()
{
	setName("NoneCursor");
	_andMask.clear();
	_xorMask.clear();
	_width = 0;
	_height = 0;
	_dataHash = base::Hash::gen64();
    _pixmap.reset();	
}

const gfx::Pixmap& Cursor::pixmap() const
{
	return _pixmap;
}

bool Cursor::operator == (const Cursor& c) const
{
	if(_xHotspot != c._xHotspot || _yHotspot != c._yHotspot)
        return false;	

	if (_width != c._width)
		return false;

	if (_height != c._height)
        return false;	

	if(_dataHash != c._dataHash)
        return false;

    return true;	
}

bool Cursor::operator != (const Cursor& c) const
{
    return !(*this == c);	
}


WILDCAT_GUI_WINDOW_API std::ostream& operator<<(std::ostream& os, const Cursor& obj)
{
    os << "(" << obj.hotSpot()<< ","<<obj.name()<<","<<obj.pixmap() << ")";

    return os;
}

WILDCAT_GUI_WINDOW_API std::istream& operator>>(std::istream& is, Cursor& obj)
{
	char ch;
	gfx::Point hotspot;
    std::string name;	
	
	is >> ch; //(    
	is >> hotspot;
	is >> ch; //,
	name = base::String::read(is, ','); //+,
	
    obj._pixmap.reset();	

    is >> obj._pixmap;	
	is >> ch; //),
    
    obj.update();
	obj.setHotSpot(hotspot);
	obj.setName(name.c_str());

	return is;
}


}}}

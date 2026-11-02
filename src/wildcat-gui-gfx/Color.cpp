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
#include <wildcat/gui/gfx/Color.h>
#include <wildcat/gui/gfx/ColorManager.h>
#include <wildcat/gui/base/String.h>
#include <map>

namespace wildcat {
namespace gui {
namespace gfx {

std::map<std::string, std::string> g_colorMap = { 
{"aliceblue","0xf0f8ff"},
{"antiquewhite","0xfaebd7"},
{"antiquewhite1","0xffefdb"},
{"antiquewhite2","0xeedfcc"},
{"antiquewhite3","0xcdc0b0"},
{"antiquewhite4","0x8b8378"},
{"aquamarine","0x7fffd4"},
{"aquamarine2","0x76eec6"},
{"aquamarine4","0x458b74"},
{"azure","0xf0ffff"},
{"azure1","0xf0ffff"},
{"azure2","0xe0eeee"},
{"azure3","0xc1cdcd"},
{"azure4","0x838b8b"},
{"beige","0xf5f5dc"},
{"bisque","0xffe4c4"},
{"bisque1","0xffe4c4"},
{"bisque2","0xeed5b7"},
{"bisque3","0xcdb79e"},
{"bisque4","0x8b7d6b"},
{"black","0x000000"},
{"blanchedalmond","0xffebcd"},
{"blue1","0x0000ff"},
{"blue2","0x0000ee"},
{"blue4","0x00008b"},
{"blueviolet","0x8a2be2"},
{"brown","0xa52a2a"},
{"brown1","0xff4040"},
{"brown2","0xee3b3b"},
{"brown3","0xcd3333"},
{"brown4","0x8b2323"},
{"burlywood","0xdeb887"},
{"burlywood1","0xffd39b"},
{"burlywood2","0xeec591"},
{"burlywood3","0xcdaa7d"},
{"burlywood4","0x8b7355"},
{"cadetblue","0x5f9ea0"},
{"cadetblue1","0x98f5ff"},
{"cadetblue2","0x8ee5ee"},
{"cadetblue3","0x7ac5cd"},
{"cadetblue4","0x53868b"},
{"chartreuse1","0x7fff00"},
{"chartreuse2","0x76ee00"},
{"chartreuse3","0x66cd00"},
{"chartreuse4","0x458b00"},
{"chocolate","0xd2691e"},
{"chocolate1","0xff7f24"},
{"chocolate2","0xee7621"},
{"chocolate3","0xcd661d"},
{"coral","0xff7f50"},
{"coral1","0xff7256"},
{"coral2","0xee6a50"},
{"coral3","0xcd5b45"},
{"coral4","0x8b3e2f"},
{"cornflowerblue","0x6495ed"},
{"cornsilk1","0xfff8dc"},
{"cornsilk2","0xeee8cd"},
{"cornsilk3","0xcdc8b1"},
{"cornsilk4","0x8b8878"},
{"cyan1","0x00ffff"},
{"cyan2","0x00eeee"},
{"cyan3","0x00cdcd"},
{"cyan4","0x008b8b"},
{"darkgoldenrod","0xb8860b"},
{"darkgoldenrod1","0xffb90f"},
{"darkgoldenrod2","0xeead0e"},
{"darkgoldenrod3","0xcd950c"},
{"darkgoldenrod4","0x8b6508"},
{"darkgreen","0x006400"},
{"darkkhaki","0xbdb76b"},
{"darkolivegreen","0x556b2f"},
{"darkolivegreen1","0xcaff70"},
{"darkolivegreen2","0xbcee68"},
{"darkolivegreen3","0xa2cd5a"},
{"darkolivegreen4","0x6e8b3d"},
{"darkorange","0xff8c00"},
{"darkorange1","0xff7f00"},
{"darkorange2","0xee7600"},
{"darkorange3","0xcd6600"},
{"darkorange4","0x8b4500"},
{"darkorchid","0x9932cc"},
{"darkorchid1","0xbf3eff"},
{"darkorchid2","0xb23aee"},
{"darkorchid3","0x9a32cd"},
{"darkorchid4","0x68228b"},
{"darksalmon","0xe9967a"},
{"darkseagreen","0x8fbc8f"},
{"darkseagreen1","0xc1ffc1"},
{"darkseagreen2","0xb4eeb4"},
{"darkseagreen3","0x9bcd9b"},
{"darkseagreen4","0x698b69"},
{"darkslateblue","0x483d8b"},
{"darkslategray","0x2f4f4f"},
{"darkslategray1","0x97ffff"},
{"darkslategray2","0x8deeee"},
{"darkslategray3","0x79cdcd"},
{"darkslategray4","0x528b8b"},
{"darkturquoise","0x00ced1"},
{"darkviolet","0x9400d3"},
{"deeppink1","0xff1493"},
{"deeppink2","0xee1289"},
{"deeppink3","0xcd1076"},
{"deeppink4","0x8b0a50"},
{"deepskyblue1","0x00bfff"},
{"deepskyblue2","0x00b2ee"},
{"deepskyblue3","0x009acd"},
{"deepskyblue4","0x00688b"},
{"dimgray","0x696969"},
{"dodgerblue1","0x1e90ff"},
{"dodgerblue2","0x1c86ee"},
{"dodgerblue3","0x1874cd"},
{"dodgerblue4","0x104e8b"},
{"firebrick","0xb22222"},
{"firebrick1","0xff3030"},
{"firebrick2","0xee2c2c"},
{"firebrick3","0xcd2626"},
{"firebrick4","0x8b1a1a"},
{"floralwhite","0xfffaf0"},
{"forestgreen","0x228b22"},
{"gainsboro","0xdcdcdc"},
{"ghostwhite","0xf8f8ff"},
{"gold1","0xffd700"},
{"gold2","0xeec900"},
{"gold3","0xcdad00"},
{"gold4","0x8b7500"},
{"goldenrod","0xdaa520"},
{"goldenrod1","0xffc125"},
{"goldenrod2","0xeeb422"},
{"goldenrod3","0xcd9b1d"},
{"goldenrod4","0x8b6914"},
{"gray","0xbebebe"},
{"gray1","0x030303"},
{"gray2","0x050505"},
{"gray3","0x080808"},
{"gray4","0x0a0a0a"},
{"gray5","0x0d0d0d"},
{"gray6","0x0f0f0f"},
{"gray7","0x121212"},
{"gray8","0x141414"},
{"gray9","0x171717"},
{"gray10","0x1a1a1a"},
{"gray11","0x1c1c1c"},
{"gray12","0x1f1f1f"},
{"gray13","0x212121"},
{"gray14","0x242424"},
{"gray15","0x262626"},
{"gray16","0x292929"},
{"gray17","0x2b2b2b"},
{"gray18","0x2e2e2e"},
{"gray19","0x303030"},
{"gray20","0x333333"},
{"gray21","0x363636"},
{"gray22","0x383838"},
{"gray23","0x3b3b3b"},
{"gray24","0x3d3d3d"},
{"gray25","0x404040"},
{"gray26","0x424242"},
{"gray27","0x454545"},
{"gray28","0x474747"},
{"gray29","0x4a4a4a"},
{"gray30","0x4d4d4d"},
{"gray31","0x4f4f4f"},
{"gray32","0x525252"},
{"gray33","0x545454"},
{"gray34","0x575757"},
{"gray35","0x595959"},
{"gray36","0x5c5c5c"},
{"gray37","0x5e5e5e"},
{"gray38","0x616161"},
{"gray39","0x636363"},
{"gray40","0x666666"},
{"gray41","0x696969"},
{"gray42","0x6b6b6b"},
{"gray43","0x6e6e6e"},
{"gray44","0x707070"},
{"gray45","0x737373"},
{"gray46","0x757575"},
{"gray47","0x787878"},
{"gray48","0x7a7a7a"},
{"gray49","0x7d7d7d"},
{"gray50","0x7f7f7f"},
{"gray51","0x828282"},
{"gray52","0x858585"},
{"gray53","0x878787"},
{"gray54","0x8a8a8a"},
{"gray55","0x8c8c8c"},
{"gray56","0x8f8f8f"},
{"gray57","0x919191"},
{"gray58","0x949494"},
{"gray59","0x969696"},
{"gray60","0x999999"},
{"gray61","0x9c9c9c"},
{"gray62","0x9e9e9e"},
{"gray63","0xa1a1a1"},
{"gray64","0xa3a3a3"},
{"gray65","0xa6a6a6"},
{"gray66","0xa8a8a8"},
{"gray67","0xababab"},
{"gray68","0xadadad"},
{"gray69","0xb0b0b0"},
{"gray70","0xb3b3b3"},
{"gray71","0xb5b5b5"},
{"gray72","0xb8b8b8"},
{"gray73","0xbababa"},
{"gray74","0xbdbdbd"},
{"gray75","0xbfbfbf"},
{"gray76","0xc2c2c2"},
{"gray77","0xc4c4c4"},
{"gray78","0xc7c7c7"},
{"gray79","0xc9c9c9"},
{"gray80","0xcccccc"},
{"gray81","0xcfcfcf"},
{"gray82","0xd1d1d1"},
{"gray83","0xd4d4d4"},
{"gray84","0xd6d6d6"},
{"gray85","0xd9d9d9"},
{"gray86","0xdbdbdb"},
{"gray87","0xdedede"},
{"gray88","0xe0e0e0"},
{"gray89","0xe3e3e3"},
{"gray90","0xe5e5e5"},
{"gray91","0xe8e8e8"},
{"gray92","0xebebeb"},
{"gray93","0xededed"},
{"gray94","0xf0f0f0"},
{"gray95","0xf2f2f2"},
{"gray97","0xf7f7f7"},
{"gray98","0xfafafa"},
{"gray99","0xfcfcfc"},
{"green1","0x00ff00"},
{"green2","0x00ee00"},
{"green3","0x00cd00"},
{"green4","0x008b00"},
{"greenyellow","0xadff2f"},
{"honeydew1","0xf0fff0"},
{"honeydew2","0xe0eee0"},
{"honeydew3","0xc1cdc1"},
{"honeydew4","0x838b83"},
{"hotpink","0xff69b4"},
{"hotpink1","0xff6eb4"},
{"hotpink2","0xee6aa7"},
{"hotpink3","0xcd6090"},
{"hotpink4","0x8b3a62"},
{"indianred","0xcd5c5c"},
{"indianred1","0xff6a6a"},
{"indianred2","0xee6363"},
{"indianred3","0xcd5555"},
{"indianred4","0x8b3a3a"},
{"ivory1","0xfffff0"},
{"ivory2","0xeeeee0"},
{"ivory3","0xcdcdc1"},
{"ivory4","0x8b8b83"},
{"khaki","0xf0e68c"},
{"khaki1","0xfff68f"},
{"khaki2","0xeee685"},
{"khaki3","0xcdc673"},
{"khaki4","0x8b864e"},
{"lavender","0xe6e6fa"},
{"lavenderblush1","0xfff0f5"},
{"lavenderblush2","0xeee0e5"},
{"lavenderblush3","0xcdc1c5"},
{"lavenderblush4","0x8b8386"},
{"lawngreen","0x7cfc00"},
{"lemonchiffon1","0xfffacd"},
{"lemonchiffon2","0xeee9bf"},
{"lemonchiffon3","0xcdc9a5"},
{"lemonchiffon4","0x8b8970"},
{"light","0xeedd82"},
{"lightblue","0xadd8e6"},
{"lightblue1","0xbfefff"},
{"lightblue2","0xb2dfee"},
{"lightblue3","0x9ac0cd"},
{"lightblue4","0x68838b"},
{"lightcoral","0xf08080"},
{"lightcyan1","0xe0ffff"},
{"lightcyan2","0xd1eeee"},
{"lightcyan3","0xb4cdcd"},
{"lightcyan4","0x7a8b8b"},
{"lightgoldenrod1","0xffec8b"},
{"lightgoldenrod2","0xeedc82"},
{"lightgoldenrod3","0xcdbe70"},
{"lightgoldenrod4","0x8b814c"},
{"lightgoldenrodyellow","0xfafad2"},
{"lightgray","0xd3d3d3"},
{"lightpink","0xffb6c1"},
{"lightpink1","0xffaeb9"},
{"lightpink2","0xeea2ad"},
{"lightpink3","0xcd8c95"},
{"lightpink4","0x8b5f65"},
{"lightsalmon1","0xffa07a"},
{"lightsalmon2","0xee9572"},
{"lightsalmon3","0xcd8162"},
{"lightsalmon4","0x8b5742"},
{"lightseagreen","0x20b2aa"},
{"lightskyblue","0x87cefa"},
{"lightskyblue1","0xb0e2ff"},
{"lightskyblue2","0xa4d3ee"},
{"lightskyblue3","0x8db6cd"},
{"lightskyblue4","0x607b8b"},
{"lightslateblue","0x8470ff"},
{"lightslategray","0x778899"},
{"lightsteelblue","0xb0c4de"},
{"lightsteelblue1","0xcae1ff"},
{"lightsteelblue2","0xbcd2ee"},
{"lightsteelblue3","0xa2b5cd"},
{"lightsteelblue4","0x6e7b8b"},
{"lightyellow1","0xffffe0"},
{"lightyellow2","0xeeeed1"},
{"lightyellow3","0xcdcdb4"},
{"lightyellow4","0x8b8b7a"},
{"limegreen","0x32cd32"},
{"linen","0xfaf0e6"},
{"magenta","0xff00ff"},
{"magenta2","0xee00ee"},
{"magenta3","0xcd00cd"},
{"magenta4","0x8b008b"},
{"maroon","0xb03060"},
{"maroon1","0xff34b3"},
{"maroon2","0xee30a7"},
{"maroon3","0xcd2990"},
{"maroon4","0x8b1c62"},
{"medium","0x66cdaa"},
{"mediumaquamarine","0x66cdaa"},
{"mediumblue","0x0000cd"},
{"mediumorchid","0xba55d3"},
{"mediumorchid1","0xe066ff"},
{"mediumorchid2","0xd15fee"},
{"mediumorchid3","0xb452cd"},
{"mediumorchid4","0x7a378b"},
{"mediumpurple","0x9370db"},
{"mediumpurple1","0xab82ff"},
{"mediumpurple2","0x9f79ee"},
{"mediumpurple3","0x8968cd"},
{"mediumpurple4","0x5d478b"},
{"mediumseagreen","0x3cb371"},
{"mediumslateblue","0x7b68ee"},
{"mediumspringgreen","0x00fa9a"},
{"mediumturquoise","0x48d1cc"},
{"mediumvioletred","0xc71585"},
{"midnightblue","0x191970"},
{"mintcream","0xf5fffa"},
{"mistyrose1","0xffe4e1"},
{"mistyrose2","0xeed5d2"},
{"mistyrose3","0xcdb7b5"},
{"mistyrose4","0x8b7d7b"},
{"moccasin","0xffe4b5"},
{"navajowhite1","0xffdead"},
{"navajowhite2","0xeecfa1"},
{"navajowhite3","0xcdb38b"},
{"navajowhite4","0x8b795e"},
{"navyblue","0x000080"},
{"oldlace","0xfdf5e6"},
{"olivedrab","0x6b8e23"},
{"olivedrab1","0xc0ff3e"},
{"olivedrab2","0xb3ee3a"},
{"olivedrab4","0x698b22"},
{"orange1","0xffa500"},
{"orange2","0xee9a00"},
{"orange3","0xcd8500"},
{"orange4","0x8b5a00"},
{"orangered1","0xff4500"},
{"orangered2","0xee4000"},
{"orangered3","0xcd3700"},
{"orangered4","0x8b2500"},
{"orchid","0xda70d6"},
{"orchid1","0xff83fa"},
{"orchid2","0xee7ae9"},
{"orchid3","0xcd69c9"},
{"orchid4","0x8b4789"},
{"pale","0xdb7093"},
{"palegoldenrod","0xeee8aa"},
{"palegreen","0x98fb98"},
{"palegreen1","0x9aff9a"},
{"palegreen2","0x90ee90"},
{"palegreen3","0x7ccd7c"},
{"palegreen4","0x548b54"},
{"paleturquoise","0xafeeee"},
{"paleturquoise1","0xbbffff"},
{"paleturquoise2","0xaeeeee"},
{"paleturquoise3","0x96cdcd"},
{"paleturquoise4","0x668b8b"},
{"palevioletred","0xdb7093"},
{"palevioletred1","0xff82ab"},
{"palevioletred2","0xee799f"},
{"palevioletred3","0xcd6889"},
{"palevioletred4","0x8b475d"},
{"papayawhip","0xffefd5"},
{"peachpuff1","0xffdab9"},
{"peachpuff2","0xeecbad"},
{"peachpuff3","0xcdaf95"},
{"peachpuff4","0x8b7765"},
{"pink","0xffc0cb"},
{"pink1","0xffb5c5"},
{"pink2","0xeea9b8"},
{"pink3","0xcd919e"},
{"pink4","0x8b636c"},
{"plum","0xdda0dd"},
{"plum1","0xffbbff"},
{"plum2","0xeeaeee"},
{"plum3","0xcd96cd"},
{"plum4","0x8b668b"},
{"powderblue","0xb0e0e6"},
{"purple","0xa020f0"},
{"purple1","0x9b30ff"},
{"purple2","0x912cee"},
{"purple3","0x7d26cd"},
{"purple4","0x551a8b"},
{"red1","0xff0000"},
{"red2","0xee0000"},
{"red3","0xcd0000"},
{"red4","0x8b0000"},
{"rosybrown","0xbc8f8f"},
{"rosybrown1","0xffc1c1"},
{"rosybrown2","0xeeb4b4"},
{"rosybrown3","0xcd9b9b"},
{"rosybrown4","0x8b6969"},
{"royalblue","0x4169e1"},
{"royalblue1","0x4876ff"},
{"royalblue2","0x436eee"},
{"royalblue3","0x3a5fcd"},
{"royalblue4","0x27408b"},
{"saddlebrown","0x8b4513"},
{"salmon","0xfa8072"},
{"salmon1","0xff8c69"},
{"salmon2","0xee8262"},
{"salmon3","0xcd7054"},
{"salmon4","0x8b4c39"},
{"sandybrown","0xf4a460"},
{"seagreen1","0x54ff9f"},
{"seagreen2","0x4eee94"},
{"seagreen3","0x43cd80"},
{"seagreen4","0x2e8b57"},
{"seashell1","0xfff5ee"},
{"seashell2","0xeee5de"},
{"seashell3","0xcdc5bf"},
{"seashell4","0x8b8682"},
{"sienna","0xa0522d"},
{"sienna1","0xff8247"},
{"sienna2","0xee7942"},
{"sienna3","0xcd6839"},
{"sienna4","0x8b4726"},
{"skyblue","0x87ceeb"},
{"skyblue1","0x87ceff"},
{"skyblue2","0x7ec0ee"},
{"skyblue3","0x6ca6cd"},
{"skyblue4","0x4a708b"},
{"slateblue","0x6a5acd"},
{"slateblue1","0x836fff"},
{"slateblue2","0x7a67ee"},
{"slateblue3","0x6959cd"},
{"slateblue4","0x473c8b"},
{"slategray","0x708090"},
{"slategray1","0xc6e2ff"},
{"slategray2","0xb9d3ee"},
{"slategray3","0x9fb6cd"},
{"slategray4","0x6c7b8b"},
{"snow","0xfffafa" },
{"snow1","0xfffafa"},
{"snow2","0xeee9e9"},
{"snow3","0xcdc9c9"},
{"snow4","0x8b8989"},
{"springgreen","0x00ff7f" },
{"springgreen1","0x00ff7f"},
{"springgreen2","0x00ee76"},
{"springgreen3","0x00cd66"},
{"springgreen4","0x008b45"},
{"steelblue","0x4682b4"},
{"steelblue1","0x63b8ff"},
{"steelblue2","0x5cacee"},
{"steelblue3","0x4f94cd"},
{"steelblue4","0x36648b"},
{"tan","0xd2b48c"},
{"tan1","0xffa54f"},
{"tan2","0xee9a49"},
{"tan3","0xcd853f"},
{"tan4","0x8b5a2b"},
{"thistle","0xd8bfd8"},
{"thistle1","0xffe1ff"},
{"thistle2","0xeed2ee"},
{"thistle3","0xcdb5cd"},
{"thistle4","0x8b7b8b"},
{"tomato1","0xff6347"},
{"tomato2","0xee5c42"},
{"tomato3","0xcd4f39"},
{"tomato4","0x8b3626"},
{"turquoise","0x40e0d0"},
{"turquoise1","0x00f5ff"},
{"turquoise2","0x00e5ee"},
{"turquoise3","0x00c5cd"},
{"turquoise4","0x00868b"},
{"violet","0xee82ee"},
{"violetred","0xd02090"},
{"violetred1","0xff3e96"},
{"violetred2","0xee3a8c"},
{"violetred3","0xcd3278"},
{"violetred4","0x8b2252"},
{"wheat","0xf5deb3"},
{"wheat1","0xffe7ba"},
{"wheat2","0xeed8ae"},
{"wheat3","0xcdba96"},
{"wheat4","0x8b7e66"},
{"white","0xffffff"},
{"whitesmoke","0xf5f5f5"},
{"yellow1","0xffff00"},
{"yellow2","0xeeee00"},
{"yellow3","0xcdcd00"},
{"yellow4","0x8b8b00"},
{"yellowgreen","0x9acd32"} 
};

ArgbColor::ArgbColor()
: _color(0)
{ 
    setAlpha(255);
}

ArgbColor::ArgbColor(uint32_t v)
: _color(v)
{
    
}

ArgbColor::ArgbColor(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
: _color(0)
{ 
    setAlpha(a);
    setRed(r);
    setGreen(g);
    setBlue(b);
}

ArgbColor::ArgbColor(uint8_t r, uint8_t g, uint8_t b)
: _color(0)
{ 
    setAlpha(255);
    setRed(r);
    setGreen(g);
    setBlue(b);
}

ArgbColor::ArgbColor(const std::string& name)
{
    *this = fromName( name);
}

ArgbColor ArgbColor::fromRgb8(Pt::uint8_t r, Pt::uint8_t g, Pt::uint8_t b, Pt::uint8_t a)
{
    return ArgbColor(a, r, g, b);
}

ArgbColor ArgbColor::fromName(const std::string& name)
 {
     std::string lower = base::String::toLower(name);

     if (lower == "none")
         return ArgbColor(0, 0, 0, 0);

     std::map<std::string, std::string>::const_iterator it = g_colorMap.find(lower);

     if (it != g_colorMap.end())
         lower = it->second;

     const uint32_t color = base::String::to<uint32_t>(lower);

     ArgbColor c = ArgbColor(color);
     c.setAlpha(255);
     return c;
 }

void ArgbColor::invert()
{    
    setAlpha(alpha());
    setRed(255 - red());
    setGreen(255 - green());
    setBlue(255 - blue());
}


WILDCAT_GUI_GFX_API std::ostream& operator<<(std::ostream& os, const ArgbColor& obj)
{
    os << '(' << (int)obj.red() << ',' << (int)obj.green() << ',' << (int)obj.blue() << ',' << (int)obj.alpha() << ')';
    return os;
}

WILDCAT_GUI_GFX_API std::istream& operator>>(std::istream& is, ArgbColor& obj)
{
    char ch;
    int r, g, b, a;
    is >> ch;
    is >> r;
    is >> ch;
    is >> g;
    is >> ch;
    is >> b;
    is >> ch;
    is >> a;
    is >> ch;

    obj.setRed(r );
    obj.setGreen(g);
    obj.setBlue(b);
    obj.setAlpha(a);
    return is;
}


Color::Color()
: _color()
, _palette(-1)
, _index(-1)
{
    setAlpha(255);
}

Color::Color(const ArgbColor& c)
: _color(c)
, _palette(-1)
, _index(-1)
{

}

Color::Color(int palette, int index)
: _color()
, _palette(palette)
, _index(index)
{

}

Color::Color(const std::string& name)
: _color(ArgbColor::fromName(name))
, _palette(-1)
, _index(-1)
{

}

Color::Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
: _color(a, r, g, b)
, _palette(-1)
, _index(-1)
{

}

Color::Color(uint8_t r, uint8_t g, uint8_t b)
: _color(r, g, b)
, _palette(-1)
, _index(-1)
{

}

uint8_t Color::alpha() const
{
    if(_palette != -1 && _index != -1)
        return ColorManager::instance().get(_palette)->get(_index)->alpha();

    return _color.alpha();
}

void Color::load()
{
    if (_palette != -1 && _index != -1)
    {        
        const gfx::ArgbColor * c = ColorManager::instance().get(_palette)->get(_index);
        setColor(*c);
    }
}

void Color::invert()
{
    setAlpha(alpha());
    setRed(255 - red());
    setGreen(255 - green());
    setBlue(255 - blue());
}

uint8_t Color::red() const
{
    if (_palette != -1 && _index != -1)
        return ColorManager::instance().get(_palette)->get(_index)->red();

    return _color.red();
}

uint8_t Color::green() const
{
    if (_palette != -1 && _index != -1)
        return ColorManager::instance().get(_palette)->get(_index)->green();

    return _color.green();
}

uint8_t Color::blue() const
{
    if (_palette != -1 && _index != -1)
        return ColorManager::instance().get(_palette)->get(_index)->blue();

    return _color.blue();
}

uint32_t Color::color() const
{
    if (_palette != -1 && _index != -1)
        return ColorManager::instance().get(_palette)->get(_index)->color();

    return _color.color();
}

ArgbColor Color::getColor() const
{
    if (_palette != -1 && _index != -1)
        return *ColorManager::instance().get(_palette)->get(_index);

    return _color;
}

std::string Color::toString() const
{
    std::stringstream ss;
    ss<< "("<<(int)alpha() << ',' << (int)red() << ',' << (int)green() << "," << (int)blue()<<") 0x" << std::hex<<_color.color();
    return ss.str();
}

WILDCAT_GUI_GFX_API std::ostream& operator<<(std::ostream& os, const Color& obj)
{
    os << '(' << obj.palette() << ',' << obj.index() << ',' << obj.getColor()<< ')';
    return os;
}


WILDCAT_GUI_GFX_API std::istream& operator>>(std::istream& is, Color& obj)
{
    char ch;
    int palette, index;
    ArgbColor color;
    is >> ch;
    is >> palette;
    is >> ch;    
    is >> index;    
    is >> ch;
    is >> color;
    is >> ch;

    obj.setColor(color);
    obj.setPalette(palette);
    obj.setIndex(index);
    return is;
}

}}}

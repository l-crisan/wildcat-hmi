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
#include <wildcat/gui/base/Serialisation.h>
#include <wildcat/gui/base/TypeFactory.h>
#include <Pt/Settings.h>
#include <Pt/TextStream.h>
#include <fstream>

namespace wildcat {
namespace gui {
namespace base {


void Serialisation::serialize(const Object& obj, const char* file)
{    
    std::ofstream fs(file);
    serialize(obj, fs);
    fs.close();
}

void Serialisation::serialize(const Object& obj, std::ostream& os)
{
    Pt::Settings settings;

    settings.addEntry(obj.name()).set(obj);
    
    Pt::TextOStream tx(os, new Pt::Utf8Codec());

    settings.save(tx);

    tx.flush();
    os.flush();
}

void Serialisation::serialize(const Object& obj, const ResourceManager& rm, const char* file)
{
    std::ofstream fs(file);
    serialize(obj, rm,  fs);
    fs.close();
}

void Serialisation::serialize(const Object& obj, const ResourceManager& rm, std::ostream& os)
{
    Pt::Settings settings;
    Pt::Settings::Entry entry = settings.addEntry("Design");

    std::stringstream ss;
    ss<< rm;

    std::stringstream base64ss;
    String::toBase64(ss, base64ss);

    entry.addEntry(obj.name()).set(obj);
    entry.addEntry("Resources").set(base64ss.str());

    Pt::TextOStream tx(os, new Pt::Utf8Codec());
    settings.save(tx);
    tx.flush();
    os.flush();
}


Object* Serialisation::deserialize(const char* file, ResourceManager& rm)
{
    std::ifstream is(file);
    return deserialize(is, rm);
}

Object* Serialisation::deserialize(std::istream& is, ResourceManager& rm)
{
    Pt::Settings settings;

    rm.clear();

    Pt::TextIStream tx(is, new Pt::Utf8Codec());

    settings.load(tx);

    Pt::Settings::Entry design = settings.begin();    

    Object* obj = nullptr;

    
    {//Resources
        Pt::Settings::Entry resEntry = design.entry("Resources");
        
        std::string resStr;
        
        resEntry.get(resStr);
        std::stringstream base64ss(resStr);  

        if(!resStr.empty())
        {
            std::stringstream ss;

            String::fromBase64(base64ss,ss);

            ss >> rm;
        }
    }   

    {//Object

        Pt::Settings::Entry objEntry = design.begin();

        Pt::Settings::Entry objTypeEntry = objEntry.entry("Class");

        std::string objType;
        objTypeEntry.get(objType);

        obj = TypeFactory::instance().create<Object>(objType);

        if (obj == nullptr)
            throw std::runtime_error("Unknown object type: " + objType);
        
        obj->setContext(new ObjectContext());

        objEntry.get(*obj);

        obj->resolve();
    }

    return obj;
}

Object* Serialisation::deserialize(const char* file)
{
    std::ifstream is(file);
    return deserialize(is);
}

Object* Serialisation::deserialize(std::istream& is)
{
    Pt::Settings settings;

    Pt::TextIStream tx(is, new Pt::Utf8Codec());

    settings.load(tx);

    Pt::Settings::Entry root = settings.begin();

    Pt::Settings::Entry objTypeEntry = root.entry("Class");


    std::string objType;
    objTypeEntry.get(objType);

    Object* obj = TypeFactory::instance().create<Object>(objType);

    if( obj == nullptr)
        throw std::runtime_error("Unknown object type: " + objType);

    obj->setContext(new ObjectContext());
    
    root.get(*obj);

    obj->resolve();
    return obj;
}

}}}

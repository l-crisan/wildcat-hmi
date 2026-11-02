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
#include <wildcat/gui/base/Object.h>
#include <wildcat/gui/base/TypeFactory.h>
#include <wildcat/gui/base/ResourceManager.h>
#include <wildcat/gui/base/Resource.h>  
#include <wildcat/gui/base/Hash.h>

namespace wildcat {
namespace gui { 
namespace base { 

Object::Object()
: _id(Hash::gen64()) 
, _context() 
, _resourceMng(nullptr)
, _tag(nullptr)
{ 
    initProp();
}


Object::Object(const std::string& name, const std::string& attribute)
: _name(name)
, _attribute(attribute)
, _context(nullptr)
, _id(base::Hash::gen64())
, _resourceMng(nullptr)
, _tag(nullptr)
{ 
    initProp();
}

Object::~Object()
{
    clear();
    delete _context;
}

void Object::initProp()
{
    registerProperty("Name", *this, &Object::name, &Object::setName, "Cat=Development;Design=true;Serialize=true");
    registerProperty("Class", *this, &Object::typeName, &Object::setTypeName, "Cat=Development;Readonly;Design=true;Serialize=true");
}

uint64_t Object::id() const
{
    return _id;
}

void Object::setId(uint64_t id)
{
    _id = id;
}

std::string Object::typeName() const
{
    return  TypeFactory::instance().typeName(typeid(*this).name());
}

void Object::setContext(ObjectContext* c)
{
    if (_context)
        delete _context;

    _context = c;
}


ResourceManager& Object::resourceMng()
{
    return *_resourceMng;
}

const ResourceManager& Object::resourceMng() const
{
    return *_resourceMng;
}

void Object::onSetResourceManager( ResourceManager& v)
{
    _resourceMng = &v;
}


void Object::setResourceManager(ResourceManager& m)
{
    onSetResourceManager(m);
}

void Object::onSerialize(Pt::SerializationInfo& si) const
{    
    for (size_t i = 0; i < _properties.size(); ++i)
    {
        Property* prop = _properties[i];

        if (prop->hasAttribute("Serialize"))
        {
            if(prop->attributeValue("Serialize") != "true")
                continue;
        }

        Pt::SerializationInfo& member = si.addMember(prop->name());

        switch (prop->kind())
        {
            case Property::Kind::Simple:
            {
                SimpleProperty* sp = dynamic_cast<SimpleProperty*>(prop);

                member <<= sp->get();
            }
            break;

            case Property::Kind::Struct:
            {
                ObjectProperty* op  = dynamic_cast<ObjectProperty*>(prop);
                Object*         obj = op->get();

                if( obj)
                    obj->serialize(member);
                else
                    member.setString("null");
            }
            break;

            case Property::Kind::Array:
            {
                ArrayProperty* ap = dynamic_cast<ArrayProperty*>(prop);

                switch (ap->kindOf())
                {
                    case Property::Kind::Simple:
                    {
                        SimpleArrayProperty*     spa = dynamic_cast<SimpleArrayProperty*>(ap);
                        std::vector<std::string> data;

                        uint32_t count = ap->count();

                        for (uint32_t i = 0; i < count; ++i)
                        {
                            const std::string value = spa->get(i);
                            data.push_back(value);
                        }

                        member <<= data;
                    }
                    break;

                    case Property::Kind::Struct:
                    {
                        ObjectArrayProperty* opa = dynamic_cast<ObjectArrayProperty*>(ap);

                        member.setSequence();

                        uint32_t count = ap->count();

                        for (uint32_t i = 0; i < count; ++i)
                        {
                            const Object* value = opa->get(i);

                            Pt::SerializationInfo& el = member.addElement();

                            if( value)
                                value->serialize(el);
                            else
                                el.setString("null");
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}

Object* Object::deserializeObj(const Pt::SerializationInfo& mi)
{
    if (!mi.isStruct())
        return nullptr;

    std::string type;
    
    mi.getMember("Class") >>= type;
    

    Object* obj = TypeFactory::instance().create<Object>(type);
    obj->setContext(new ObjectContext());

    _context->add(obj);

    obj->deserialize(mi);

    
    return obj;
}

void Object::onDeserialize(const Pt::SerializationInfo& si)
{    
    PropertyVector::iterator it = _properties.begin();

    for (; it != _properties.end(); ++it)
    {
        const Pt::SerializationInfo* mi = si.findMember((*it)->name());
    
        if( mi == nullptr)
            continue;

        if( mi->isVoid())
            continue;

        if(!hasProperty(mi->name()))
            continue;

        Property* prop = dynamic_cast<Property*>(&property(mi->name()));

        if (mi->isSequence())
        {
            ArrayProperty* ap = dynamic_cast<ArrayProperty*>(prop);

            switch (ap->kindOf())
            {
                case Property::Kind::Simple:
                {
                     std::vector<std::string> data;

                     *mi >>= data;
                     SimpleArrayProperty* spa = dynamic_cast<SimpleArrayProperty*>(ap);
                     spa->set(data);
                }
                break;

                case Property::Struct:
                {
                    Pt::SerializationInfo::ConstIterator it = mi->begin();

                    size_t index = 0;

                    for (; it != mi->end(); ++it)
                    {
                        Object* obj = deserializeObj(*it);

                        ObjectArrayProperty* objProp = dynamic_cast<ObjectArrayProperty*>(prop);
                        objProp->set(index, obj);
                        ++index;
                    }
                }
                break;
            }
        }
        else if( mi->isStruct())
        {        
            Object* obj = deserializeObj(*mi);

            ObjectProperty* objProp = dynamic_cast<ObjectProperty*>(prop);
            objProp->set(obj);
        }
        else
        {
            std::string value;
            *mi >>= value;

            SimpleProperty* sp = dynamic_cast<SimpleProperty*>(prop);

            if(sp)
                sp->set(value);
        }        
    }
}

void Object::onResolve()
{
    for (size_t i = 0; i < _properties.size(); ++i)
    {
        Property* prop = _properties[i];

        switch (prop->kind())
        {
            case Property::Struct:
            {
                ObjectProperty* objProp = dynamic_cast<ObjectProperty*>(prop);
                Object* obj = objProp->get();

                if(obj)
                    obj->resolve();
            }
            break;

            case Property::Array:
            {
                ArrayProperty* arrProp = dynamic_cast<ArrayProperty*>(prop);

                switch(arrProp->kindOf())
                {
                    case Property::Struct:
                    {
                        ObjectArrayProperty* objArrProp = dynamic_cast<ObjectArrayProperty*>(arrProp);

                        for( size_t i = 0; i < objArrProp->count(); ++i)
                        {
                            Object* obj = objArrProp->get(i);
                        
                            if (obj)
                                obj->resolve();
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}

ObjectContext::ObjectContext()
: _objects()
{
}

ObjectContext::~ObjectContext()
{
    clear();
}

void ObjectContext::clear()
{
    for (size_t i = 0; i < _objects.size(); ++i)
    {
        Object* obj = _objects[i];  

        if( obj->context())
            obj->context()->clear();

        delete obj;
    }

    _objects.clear();
}

Object* ObjectContext::find(const Object* obj) const
{
    auto it = std::find(_objects.begin(), _objects.end(), obj);

    return it != _objects.end() ? *it : nullptr;    
}

}}}
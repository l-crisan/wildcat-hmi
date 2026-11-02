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
#ifndef WILDCAT_GUI_BASE_OBJECT_H
#define WILDCAT_GUI_BASE_OBJECT_H

#include <wildcat/gui/base/SimpleProperty.h>
#include <wildcat/gui/base/ArrayProperty.h>
#include <wildcat/gui/base/ObjectProperty.h>
#include <wildcat/gui/base/Property.h>
#include <wildcat/gui/base/Signal.h>
#include <wildcat/gui/base/String.h>
#include <wildcat/gui/base/Serializable.h>
#include <Pt/TypeTraits.h>
#include <Pt/Signal.h>
#include <Pt/Any.h>
#include <Pt/Slot.h>
#include <Pt/Types.h>
#include <Pt/SerializationInfo.h>

#include <vector>
#include <map>
#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace wildcat{
namespace gui {
namespace base {

class ObjectContext;   
class ResourceManager;
class Resource;

class WILDCAT_GUI_BASE_API Object : public Pt::Connectable, public base::Serializable
{
    public:
        Object();

        Object(const std::string& name, const std::string& attribute = "");
                
        virtual ~Object();

        uint64_t id() const;

        void setId(uint64_t id);

        void registerSignal( const std::string& name, Signal& signal, const std::string& attr = "", const std::string& desc = "")
        {
            _signals.insert(std::make_pair(name, SignalInfo(name, signal, attr, desc)));
        }   

        bool hasSignal(const std::string& name) const
        {
            return _signals.find(name) != _signals.end();
        }   

        Signal& signal(const std::string& name)
        {
            auto it = _signals.find(name);

            if (it == _signals.end())
            {
                std::stringstream ss;
                ss << "Signal '" << name << "' not found";
                throw std::invalid_argument(ss.str());
            }
            return it->second.signal();
        }   

        const SignalInfo& signalInfo(const std::string& name)
        {
            auto it = _signals.find(name);

            if (it == _signals.end())
            {
                std::stringstream ss;
                ss << "Signal '" << name << "' not found";
                throw std::invalid_argument(ss.str());
            }
            return it->second;
        }


        template <class R, class ClassT, class A1, class A2>        
        bool connectSignal(const std::string& name, Pt::MethodSlot<R, ClassT, A1, A2>& slot)
        {            
            if(!hasSignal(name))
                return false;

            Signal& s = signal(name);

            s += slot;
            
            return true;    
        }       

        template <class R, class ClassT, class A1, class A2>
        bool disconnectSignal(const std::string& name, const Pt::MethodSlot<R, ClassT, A1, A2>& slot)
        {
            if(!hasSignal(name))
                return false;

            Signal& s = signal(name);

            s -= slot;

            return true;    
        }   

        //Simple
        template <typename A, typename C>
        Property* registerProperty(const std::string& name, C& parent, A (C::*getter)() const, void (C::*setter)(A type), const std::string& attributes = "")
        {
            throwExists(name);
            Property* p = new SimplePropertyImpl<A, C>(name, parent, getter, setter, attributes);
            _properties.push_back(p);
            return p;
        }

        //Object
        template <typename C>
        Property* registerProperty(const std::string& name, C& parent, Object* (C::* getter)() const, void (C::* setter)(Object* v), const std::string& attributes = "")
        {
            throwExists(name);
            Property* p = new ObjectPropertyImpl<C>(name, parent, getter, setter, attributes);
            _properties.push_back(p);
            return p;
        }
            
        //Object Array
        template <typename C>
        Property* registerProperty(const std::string& name, C& parent, Object*(C::* getter)(uint32_t) const, void (C::* setter)(uint32_t, Object*), uint32_t (C::*count)() const, const std::string& attributes = "")
        {
            throwExists(name);
            Property* p = new ObjArrayPropertyImpl<C>(name, parent, getter, setter, count, attributes);
            _properties.push_back(p);
            return p;
        }
   
        //Simple array
        template <typename A, typename C>
        Property* registerProperty(const std::string& name, C& parent, A(C::* getter)(uint32_t) const, void (C::* setter)(uint32_t, A), uint32_t(C::* count)() const, const std::string& attributes = "")
        {
            throwExists(name);
            Property* p = new ArrayPropertyImpl<A, C>(name, parent, getter, setter, count, attributes);
            _properties.push_back(p);
            return p;
        }

        bool unregisterProperty(const std::string& name)
        {
            for (uint32_t i = 0; i < _properties.size(); ++i)
            {
                Property* prop = _properties[i];

                if (prop->name() == name)
                {
                    _properties.erase(_properties.begin() + i);
                    delete prop;
                    return true;
                }
            }

            return false;
        }
        
        bool unregisterProperty(const Property& prop)
        {
            return unregisterProperty(prop.name());
        }

        bool hasProperty(const std::string& name) const
        {
            for (uint32_t i = 0; i < _properties.size(); ++i)
            {
                Property* prop = _properties[i];

                if (prop->name() == name)
                    return true;
            }

            return false;
        }

        const Property* findProperty(const std::string& name) const
        {
            for (uint32_t i = 0; i < _properties.size(); ++i)
            {
                Property* prop = _properties[i];
                if (prop->name() == name)
                    return prop;
            }

            return nullptr;
        }

        Property* findProperty(const std::string& name)
        {
            for (uint32_t i = 0; i < _properties.size(); ++i)
            {
                Property* prop = _properties[i];
                if (prop->name() == name)
                    return prop;
            }

            return nullptr;
        }


        void clear()
        {
            for (size_t i = 0; i < _properties.size(); ++i)
            {
                Property* pp = _properties[i];
                delete pp;
            }

            _properties.clear();            
        }

        const Property& property(const std::string& name) const
        {
            std::vector<Property*>::const_iterator it;

            for (it = _properties.begin(); it != _properties.end(); ++it)
            {
                if (name == (*it)->name())
                    return *(*it);
            }

            std::stringstream ss;
            ss << "Property '" << name << "' not found";
            throw std::invalid_argument(ss.str());
        }

        Property& property(const std::string& name)
        {
            std::vector<Property*>::iterator it;

            for (it = _properties.begin(); it != _properties.end(); ++it)
            {
                if (name == (*it)->name())
                    return *(*it);
            }

            std::stringstream ss;
            ss << "Property '" << name << "' not found";
            throw std::invalid_argument(ss.str());
        }

        uint32_t properties() const
        {
            return (uint32_t)_properties.size();
        }

        const Property& property(uint32_t i) const
        {
            return *_properties[i];
        }

        Property& property(uint32_t i)
        {
            return *_properties[i];
        }

        size_t propertyCount() const
        {
            return _properties.size();
        }

        const std::string& name() const
        {
            return _name;
        }

        void setName(const std::string& v)
        {
            _name = v;
        }

        virtual std::string attribute() const
        {
            return _attribute;
        }

        const PropertyVector& items() const
        {
            return _properties;
        }


        ObjectContext* context()
        {
            return _context;
        }

        const ObjectContext* context() const
        {
            return _context;
        }

        void setContext(ObjectContext* c);
        

        void resolve()
        {
            onResolve();
        }

        std::string typeName() const;

        std::string getPropValue(const std::string name) const
        {
            if(!hasProperty(name))
                return "";

            const Property& prop = property(name);

            if (prop.kind() != Property::Simple)
                return "";
            
            const SimpleProperty& sp = dynamic_cast<const SimpleProperty&>(prop);

            return sp.get();    
        }

        void setPropValue(const std::string name, const std::string& v)
        {
            if(!hasProperty(name))
                return;

            Property& prop = property(name);

            if (prop.kind() != Property::Simple)
                return;
            
            SimpleProperty& sp = dynamic_cast<SimpleProperty&>(prop);

            sp.set(v);    
        }       


        void setPropValue(const std::string name, const Property& v);
        

        void* tag() const
        {
            return _tag;
        }

        void setTag(void* v)
        {
            _tag = v;
        }

        const std::map<std::string, SignalInfo>& signals() const
        {
            return _signals;
        }   

        ResourceManager& resourceMng();

        const ResourceManager& resourceMng() const;

        void setResourceManager(ResourceManager& m);


    protected:
         virtual void onSetResourceManager(base::ResourceManager& v);


    protected:        
        virtual void onSerialize(Pt::SerializationInfo& si) const;
       
        virtual void onDeserialize(const Pt::SerializationInfo& si);

        virtual void onResolve();

    private:
        Object* deserializeObj(const Pt::SerializationInfo& mi);

        void throwExists(const std::string& name)
        {
            if (!hasProperty(name))
                return;

            std::stringstream ss;
            ss << "property name '" <<_name<<"."<<name << "' all ready in use";
            throw std::invalid_argument(ss.str());
        }


        void setTypeName(std::string v)
        {            
        }

        void initProp();

    private:
        PropertyVector      _properties;
        std::string         _name;
        std::string         _attribute;
        ObjectContext*      _context;
        uint64_t            _id;
        std::map<std::string, SignalInfo> _signals;
        ResourceManager* _resourceMng;
        void* _tag;
};

inline void operator <<=(Pt::SerializationInfo& si, const Object& obj)
{
    obj.serialize(si);
}

inline void operator >>=(const Pt::SerializationInfo& si, Object& obj)
{
    obj.deserialize(si);
}

class WILDCAT_GUI_BASE_API ObjectContext
{
    public:
        ObjectContext();

        virtual ~ObjectContext();

        const std::vector<Object*>& objects() const
        {
            return _objects;
        }

        std::vector<Object*>& objects()
        {
            return _objects;
        }

        void add(Object* obj)
        {
            _objects.push_back(obj);    
        }

        bool remove(Object* obj)
        {
            auto it = std::find(_objects.begin(), _objects.end(), obj);

            if (it != _objects.end())
            {
                _objects.erase(it);
                return true;
            }

            return false;
        }

        bool destroy(Object* obj)
        {
            auto it = std::find(_objects.begin(), _objects.end(), obj);

            if (it != _objects.end())
            {
                _objects.erase(it);
                delete obj;
                return true;
            }

            return false;
        }

        void clear();

        bool empty() const
        {
            return _objects.empty();
        }

        Object* find(const Object* obj) const;

    private:
        std::vector<Object*> _objects;
        
};

}}}

#endif

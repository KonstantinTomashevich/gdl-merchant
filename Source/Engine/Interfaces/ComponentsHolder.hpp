#pragma once
#include "Component.hpp"
#include <Urho3D/Container/Vector.h>
#include <Engine/Engine.hpp>

namespace GameEngine
{
template <class T> class ComponentsHolderInterface
{
public:
    ComponentsHolderInterface () {}
    virtual void AddComponent (Urho3D::SharedPtr <T> component) = 0;
    virtual bool RemoveComponent(Urho3D::SharedPtr <T> component) = 0;
    virtual Urho3D::SharedPtr<T> CreateComponent(Urho3D::String typeName, Urho3D::XMLElement rootElement = Urho3D::XMLElement::EMPTY) = 0;
    virtual Urho3D::SharedPtr <T> GetComponent (Urho3D::String typeName) = 0;
    virtual Urho3D::Vector <Urho3D::SharedPtr <T> > *GetAllComponents (Urho3D::String typeName) = 0;
    virtual void RemoveAllComponents (Urho3D::String typeName) = 0;
    virtual bool HasComponent (Urho3D::String typeName) = 0;
    virtual int CountOfComponents (Urho3D::String typeName) = 0;
    virtual ~ComponentsHolderInterface () {}
};

template <class T> class ComponentsHolder : public ComponentsHolderInterface <T>
{
protected:
    Urho3D::Vector <Urho3D::SharedPtr <T> > components_;
    Urho3D::Context *contextPointer_;

    virtual void OnComponentAdded (Urho3D::SharedPtr <T> component) = 0;
    virtual void OnComponentRemoved (Urho3D::SharedPtr <T> component) = 0;
    virtual void OnComponentCreated (Urho3D::SharedPtr <T> component) = 0;
public:
    ComponentsHolder (Urho3D::Context *context) : components_ (), contextPointer_ (context) {}
    virtual void AddComponent (Urho3D::SharedPtr <T> component)
    {
        components_.Push (component);
        OnComponentAdded (component);
    }

    virtual bool RemoveComponent (Urho3D::SharedPtr <T> component)
    {
        assert (component.NotNull ());
        OnComponentRemoved (component);
        return components_.Remove (component);
    }

    virtual Urho3D::SharedPtr<T> CreateComponent (Urho3D::String typeName, Urho3D::XMLElement rootElement = Urho3D::XMLElement::EMPTY)
    {
        assert (typeName.Length () > 0);
        Urho3D::SharedPtr <T> component;
        component.StaticCast (contextPointer_->CreateObject (Urho3D::StringHash (typeName)));
        assert (component.NotNull ());
        if (component.NotNull ())
        {
            if (rootElement != Urho3D::XMLElement::EMPTY)
                component->LoadFromXML (rootElement);
            AddComponent (component);
        }
        OnComponentCreated (component);
        return component;
    }

    virtual Urho3D::SharedPtr <T> GetComponent (Urho3D::String typeName)
    {
        assert (typeName.Length () > 0);
        for (int index = 0; index < components_.Size (); index++)
            if (components_.At (index)->GetTypeInfo ()->IsTypeOf (typeName) || typeName == "any")
                return components_.At (index);
        return Urho3D::SharedPtr <T> ();
    }

    virtual Urho3D::Vector <Urho3D::SharedPtr <T> > *GetAllComponents (Urho3D::String typeName)
    {
        assert (typeName.Length () > 0);
        Urho3D::Vector <Urho3D::SharedPtr <T> > *array = new Urho3D::Vector <Urho3D::SharedPtr <T> > ();
        for (int index = 0; index < components_.Size (); index++)
            if (components_.At (index)->GetTypeInfo ()->IsTypeOf (typeName) || typeName == "any")
                array->Push (components_.At (index));
        return array;
    }

    virtual void RemoveAllComponents (Urho3D::String typeName)
    {
        assert (typeName.Length () > 0);
        Urho3D::Vector <Urho3D::SharedPtr <T> > *array = GetAllComponents (typeName);
        for (int index = 0; index < array->Size (); index++)
            RemoveComponent (array->At (index));
        array->Clear ();
        delete array;
    }

    virtual bool HasComponent (Urho3D::String typeName)
    {
        assert (typeName.Length () > 0);
        return GetComponent (typeName).NotNull ();
    }

    virtual int CountOfComponents (Urho3D::String typeName)
    {
        assert (typeName.Length () > 0);
        Urho3D::Vector<Urho3D::SharedPtr <T> > *objects = GetAllComponents (typeName);
        int count = objects->Size ();
        delete objects;
        return count;
    }

    template <class U> inline Urho3D::SharedPtr <U> CreateComponent (Urho3D::XMLElement rootElement = Urho3D::XMLElement::EMPTY)
    {
        Urho3D::SharedPtr <U> component;
        component.StaticCast (CreateComponent (U::GetTypeNameStatic (), rootElement));
        return component;
    }

    template <class U> inline Urho3D::SharedPtr <U> GetComponent ()
    {
        Urho3D::SharedPtr <U> component;
        component.StaticCast (GetComponent (U::GetTypeNameStatic ()));
        return component;
    }

    template <class U> inline Urho3D::Vector <Urho3D::SharedPtr <U> > *GetAllComponents ()
    {
        return (Urho3D::Vector <Urho3D::SharedPtr <U> > *) GetAllComponents (U::GetTypeNameStatic ());
    }

    template <class U> inline void RemoveAllComponents ()
    {
        RemoveAllComponents (U::GetTypeNameStatic ());
    }

    template <class U> inline bool HasComponent ()
    {
        return HasComponent (U::GetTypeNameStatic ());
    }

    template <class U> inline int CountOfComponents ()
    {
        return CountOfComponents (U::GetTypeNameStatic ());
    }
    virtual ~ComponentsHolder () {}
};
}


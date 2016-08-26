#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Variant.h>
#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Resource/XMLElement.h>
#include <StatesEngine/StateObject.hpp>

namespace GameEngine
{
class Player;
class PlayerProtectedData;
class Map;
class XMLSerializable
{
public:
    virtual bool LoadFromXML (Urho3D::XMLElement rootElement) = 0;
    virtual bool SaveToXML (Urho3D::XMLElement &parentElement) = 0;
};

class EventListener
{
protected:
    Urho3D::Vector <Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> > deferredEvents_;
    virtual void ProcessDeferredEvents ();
public:
    virtual bool ProcessEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) = 0;
    virtual bool SendDeferredEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
};

class Component : public StatesEngine::StateObject, public XMLSerializable, public EventListener
{
URHO3D_OBJECT (Component, Urho3D::Object)
public:
    Component (Urho3D::Context *context);
    virtual bool Init () = 0;
    virtual bool Update (float timeStep) = 0;
    virtual bool Dispose () = 0;
    virtual ~Component ();
};

class PlayerComponent : public Component
{
URHO3D_OBJECT (PlayerComponent, Component)
protected:
    Player *player_;
    PlayerProtectedData *playerData_;
public:
    PlayerComponent (Urho3D::Context *context);
    Player *GetPlayer ();
    void SetPlayer (Player *player, PlayerProtectedData *playerData);
    virtual ~PlayerComponent ();
};

class MapComponent : public Component
{
URHO3D_OBJECT (MapComponent, Component)
protected:
    Map *map_;
public:
    MapComponent (Urho3D::Context *context);
    Map *GetMap ();
    void SetMap (Map *map);
    virtual ~MapComponent ();
};

template <class T> void RegisterType (Urho3D::Context *context)
{
    if (context->CreateObject <T> ().Null ())
        context->RegisterFactory (new Urho3D::ObjectFactoryImpl <T> (context));
}
}

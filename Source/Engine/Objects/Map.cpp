#include "Map.hpp"
#include "Events.hpp"
#include "MapObject.hpp"
#include "Player.hpp"
#include <StatesEngine/StatesEngineEvents.hpp>
#include <Engine/Engine.hpp>

namespace GameEngine
{
StatesEngine::StateObject *Map::HubAsStateObject ()
{
    return this;
}

Urho3D::Context *Map::GetContext ()
{
    return context_;
}

void Map::SendUrho3DEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    SendEvent (eventType, eventData);
}

void Map::OnComponentAdded (Urho3D::SharedPtr <MapComponent> component)
{
    if (component->GetMap () != this)
        component->SetMap (this);
    if (!component->IsReady ())
        component->Init ();
}

void Map::OnComponentRemoved (Urho3D::SharedPtr <MapComponent> component)
{

}

void Map::OnComponentCreated (Urho3D::SharedPtr <MapComponent> component)
{

}

void Map::CheckAndSendCollisions ()
{
    Urho3D::Vector <Urho3D::SharedPtr <MapObject> > *objects = GetAll <MapObject> ();
    if (objects->Size () > 1)
        for (int indexA = 0; indexA < objects->Size () - 1; indexA++)
        {
            Urho3D::SharedPtr <MapObject> objectA = objects->At (indexA);
            for (int indexB = indexA + 1; indexB < objects->Size (); indexB++)
            {
                Urho3D::SharedPtr <MapObject> objectB = objects->At (indexB);
                float distance = (objectA->GetPosition () - objectB->GetPosition ()).Length ();
                if (distance < objectA->GetMaxCollisionDistance () && distance < objectB->GetMaxCollisionDistance ())
                    if (objectA->GetBounds ()->IsIntersects (*objectB->GetBounds ()))
                    {
                        Urho3D::VariantMap eventData;
                        eventData [Events::ObjectsCollideData::ObjectA] = objectA.Get ();
                        eventData [Events::ObjectsCollideData::ObjectB] = objectB.Get ();
                        objectA->SendDeferredEvent (Events::ObjectsCollide, eventData);
                        objectB->SendDeferredEvent (Events::ObjectsCollide, eventData);
                    }
            }
        }
    delete objects;
}

Map::Map (Urho3D::Context *context) : Component (context), ComponentsHolder (context), StatesEngine::StateObjectsHub ()
{
    SubscribeToEvent (this, StatesEngine::Events::E_STATE_OBJECT_ADDED_TO_HUB, URHO3D_HANDLER (Map, OnObjectAdded));
}

bool Map::Init ()
{
    if (!ready_)
    {
        ReInitAll ("any");
        for (int index = 0; index < components_.Size (); index++)
        {
            Urho3D::SharedPtr <MapComponent> component = components_.At (index);
            if (component->GetMap () != this)
                component->SetMap (this);
            if (!component->IsReady ())
                component->Init ();
        }
        ready_ = true;
        return true;
    }
    else
    {
        ready_ = false;
        return false;
    }
}

bool Map::Update (float timeStep)
{
    CheckAndSendCollisions ();
    UpdateAll ("any", timeStep);
    ProcessDeferredEvents ();
    for (int index = 0; index < components_.Size (); index++)
    {
        Urho3D::SharedPtr <MapComponent> component = components_.At (index);
        if (component->IsWillBeUpdated () && component->IsReady ())
            component->Update (timeStep);
    }

    int index = 0;
    while (index < components_.Size ())
    {
        Urho3D::SharedPtr <MapComponent> component = components_.At (index);
        if (component->IsWillBeDeleted ())
        {
            component->Dispose ();
            components_.Remove (component);
            components_.Compact ();
        }
        else
            index++;
    }
    return true;
}

bool Map::Dispose ()
{
    if (ready_)
    {
        ready_ = false;
        for (int index = 0; index < components_.Size (); index++)
            if (components_.At (index)->IsReady ())
                components_.At (index)->Dispose ();
        DisposeAll ("any");
        return true;
    }
    else
        return false;
}

bool Map::LoadFromXML (Urho3D::XMLElement rootElement)
{
    assert (rootElement.HasChild ("components"));
    assert (rootElement.HasChild ("objects"));
    if (!rootElement.HasChild ("components") || !rootElement.HasChild ("objects"))
        return false;

    Urho3D::XMLElement componentXML = rootElement.GetChild ("components").GetChild ("component");
    while (componentXML.NotNull () && componentXML != Urho3D::XMLElement::EMPTY)
    {
        Urho3D::String typeName = componentXML.GetAttribute ("type");
        Urho3D::SharedPtr <MapComponent> component = CreateComponent (typeName);
        if (component.NotNull ())
        {
            assert (component->LoadFromXML (componentXML));
            component->Init ();
        }
        componentXML = componentXML.GetNext ("component");
    }

    Urho3D::XMLElement objectXML = rootElement.GetChild ("objects").GetChild ("object");
    while (objectXML.NotNull () && objectXML != Urho3D::XMLElement::EMPTY)
    {
        Urho3D::String typeName = objectXML.GetAttribute ("type");
        Urho3D::SharedPtr <Component> object;
        object.StaticCast (Create (typeName));
        if (object.NotNull ())
        {
            assert (object->LoadFromXML (objectXML));
            object->Init ();
        }
        objectXML = objectXML.GetNext ("object");
    }
    return true;
}

Urho3D::XMLElement Map::SaveToXML (Urho3D::XMLElement &parentElement)
{
    Urho3D::XMLElement saveElement = parentElement.CreateChild ("map");
    Urho3D::XMLElement componentsXML = saveElement.CreateChild ("components");
    for (int index = 0; index < components_.Size (); index++)
        components_.At (index)->SaveToXML (componentsXML);

    Urho3D::XMLElement objectsXML = saveElement.CreateChild ("objects");
    for (int index = 0; index < objects_.Size (); index++)
        ( (Component *) (objects_.At (index).Get () ) )->SaveToXML (objectsXML);
    return saveElement;
}

bool Map::ProcessEvent(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    bool isProcessed = false;
    for (int index = 0; index < components_.Size (); index++)
    {
        Urho3D::SharedPtr <MapComponent> component = components_.At (index);
        bool localResult = false;
        if (component->IsReady ())
            localResult = component->ProcessEvent (eventType, eventData);
        if (localResult)
            isProcessed = true;
    }
    return isProcessed;
}

Urho3D::SharedPtr <Player> Map::GetPlayerById (Urho3D::StringHash id)
{
    Urho3D::Vector <Urho3D::SharedPtr <Player> > *players = GetAll <Player> ();
    Urho3D::SharedPtr <Player> selected;
    for (int index = 0; index < players->Size (); index++)
        if (Urho3D::StringHash (players->At (index)->GetId ()) == id)
        {
            selected =  players->At (index);
            index = players->Size ();
        }
    delete players;
    return selected;
}

void Map::OnObjectAdded (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Component *component = (Component *) eventData [StatesEngine::Events::StateObjectAddedToHub::P_STATE_OBJECT].GetPtr ();
    if (component->GetTypeInfo ()->IsTypeOf <MapObject> ())
    {
        MapObject *object = (MapObject *) component;
        if (object->GetIdentifierNumber () == -1)
            if (objects_.Size () == 1)
                object->SetIdentifierNumber (0);
            else
            {
                MapObject *lastObject = 0;
                int lastScannedIndex = objects_.Size () - 1;
                while (!lastObject && lastScannedIndex > 0)
                {
                    lastScannedIndex -= 1;
                    if (objects_.At (lastScannedIndex)->GetTypeInfo ()->IsTypeOf <MapObject> ())
                        lastObject = (MapObject *) objects_.At (lastScannedIndex).Get ();
                }

                if (lastObject)
                    object->SetIdentifierNumber (lastObject->GetIdentifierNumber () + 1);
                else
                    object->SetIdentifierNumber (0);
            }
    }
}

Map::~Map ()
{

}
}

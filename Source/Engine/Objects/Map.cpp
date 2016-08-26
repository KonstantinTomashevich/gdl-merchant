#include "Map.hpp"
#include "Events.hpp"
#include "MapObject.hpp"
#include "Player.hpp"

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
    // TODO: Implement this.
    return true;
}

bool Map::SaveToXML (Urho3D::XMLElement parentElement)
{
    //TODO: Implement this.
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

Urho3D::SharedPtr <Player> Map::GetPlayerById (Urho3D::String id)
{
    Urho3D::Vector <Urho3D::SharedPtr <Player> > *players = GetAll <Player> ();
    Urho3D::SharedPtr <Player> selected;
    for (int index = 0; index < players->Size (); index++)
        if (players->At (index)->GetId () == id)
        {
            selected =  players->At (index);
            index = players->Size ();
        }
    delete players;
    return selected;
}

Map::~Map ()
{

}
}

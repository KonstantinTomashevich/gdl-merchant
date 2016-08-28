#include "Map.hpp"
#include "Events.hpp"
#include "MapObject.hpp"
#include "Player.hpp"
#include "MapVisualizer.hpp"
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

Map::Map (Urho3D::Context *context) : Component (context), ComponentsHolder (context), StatesEngine::StateObjectsHub (), ambients_ ()
{
    SubscribeToEvent (this, StatesEngine::Events::E_STATE_OBJECT_ADDED_TO_HUB, URHO3D_HANDLER (Map, OnObjectAdded));
    SubscribeToEvent (this, StatesEngine::Events::E_STATE_OBJECT_REMOVED_FROM_HUB, URHO3D_HANDLER (Map, OnObjectRemoved));
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
    ClearAmbients ();
    assert (rootElement.HasChild ("components"));
    assert (rootElement.HasChild ("objects"));
    if (!rootElement.HasChild ("components") || !rootElement.HasChild ("objects"))
        return false;

    if (rootElement.HasChild ("ambients"))
    {
        Urho3D::XMLElement ambientXML = rootElement.GetChild ("ambients").GetChild ("ambient");
        while (ambientXML.NotNull () && ambientXML != Urho3D::XMLElement::EMPTY)
        {
            assert (ambientXML.HasAttribute ("position"));
            assert (ambientXML.HasAttribute ("xmlPrefabPath"));
            assert (ambientXML.HasAttribute ("rotation"));
            assert (ambientXML.HasAttribute ("scale"));
            if (ambientXML.HasAttribute ("position") && ambientXML.HasAttribute ("xmlPrefabPath") &&
                    ambientXML.HasAttribute ("rotation") && ambientXML.HasAttribute ("scale"))
            {
                AmbientObject *ambient = new AmbientObject ();
                ambient->position_ = ambientXML.GetVector2 ("position");
                ambient->xmlPrefabPath_ = ambientXML.GetAttribute ("xmlPrefabPath");
                ambient->rotation_ = ambientXML.GetFloat ("rotation");
                ambient->scale_ = ambientXML.GetFloat ("scale");
                AddAmbient (ambient);
            }
            ambientXML = ambientXML.GetNext ("ambient");
        }
    }

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
    Urho3D::XMLElement ambientsXML = parentElement.CreateChild ("ambients");
    if (!ambients_.Empty ())
        for (int index = 0; index < ambients_.Size (); index++)
        {
            AmbientObject *ambient = ambients_.At (index);
            Urho3D::XMLElement ambientXML = ambientsXML.CreateChild ("ambient");
            ambientXML.SetVector2 ("position", ambient->position_);
            ambientXML.SetAttribute ("xmlPrefabPath", ambient->xmlPrefabPath_);
            ambientXML.SetFloat ("rotation", ambient->rotation_);
            ambientXML.SetFloat ("scale", ambient->scale_);
        }

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
        if (HasComponent <MapVisualizer> ())
            GetComponent <MapVisualizer> ()->AddObject (object);
    }
}

void Map::OnObjectRemoved (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Component *component = (Component *) eventData [StatesEngine::Events::StateObjectAddedToHub::P_STATE_OBJECT].GetPtr ();
    if (component->GetTypeInfo ()->IsTypeOf <MapObject> ())
    {
        MapObject *object = (MapObject *) component;
        if (HasComponent <MapVisualizer> ())
            GetComponent <MapVisualizer> ()->RemoveObject (object);
    }
}

Urho3D::SharedPtr <MapObject> Map::GetMapObjectById (int id)
{
    Urho3D::Vector <Urho3D::SharedPtr <MapObject> > *objects = GetAll <MapObject> ();
    Urho3D::SharedPtr <MapObject> selected;
    for (int index = 0; index < objects->Size (); index++)
        if (objects->At (index)->GetIdentifierNumber () == id)
        {
            selected = objects->At (index);
            index = objects->Size ();
        }
    delete objects;
    return selected;
}

Urho3D::Vector <AmbientObject *> *Map::GetAmbientsVector ()
{
    return &ambients_;
}

void Map::ClearAmbients ()
{
    while (!ambients_.Empty ())
        RemoveAmbient (ambients_.Front ());
}

void Map::AddAmbient (AmbientObject *ambient)
{
    if (HasComponent <MapVisualizer> ())
        GetComponent <MapVisualizer> ()->AddAmbient (ambient);
    ambients_.Push (ambient);
}

bool Map::RemoveAmbient (AmbientObject *ambient)
{
    if (ambients_.Contains (ambient) && HasComponent <MapVisualizer> ())
        GetComponent <MapVisualizer> ()->RemoveAmbient (ambient);
    bool result = ambients_.Remove (ambient);
    if (result)
        delete ambient;
    return result;
}

void Map::RecreateVisualization()
{
    if (HasComponent <MapVisualizer> ())
    {
        Urho3D::SharedPtr <MapVisualizer> mapVisualizer = GetComponent <MapVisualizer> ();
        mapVisualizer->ClearAll ();
        mapVisualizer->UpdateTerrain ();
        for (int index = 0; index < ambients_.Size (); index++)
            mapVisualizer->AddAmbient (ambients_.At (index));

        for (int index = 0; index < objects_.Size (); index++)
            if (objects_.At (index)->GetTypeInfo ()->IsTypeOf <MapObject> ())
                mapVisualizer->AddObject ((MapObject *) objects_.At (index).Get ());
    }
}

Map::~Map ()
{
    ClearAmbients ();
}
}

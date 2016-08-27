#pragma once
#include <Urho3D/Core/Context.h>
#include <Engine/Interfaces/Component.hpp>
#include <Engine/Interfaces/ComponentsHolder.hpp>
#include <Engine/Objects/MapObject.hpp>
#include <StatesEngine/StateObjectsHub.hpp>
#include <Engine/Utils/AmbientObject.hpp>

namespace GameEngine
{
class Map : public Component, public ComponentsHolder <MapComponent>, public StatesEngine::StateObjectsHub
{
URHO3D_OBJECT (Map, Component)
protected:
    Urho3D::Vector <AmbientObject *> ambients_;
    virtual StateObject *HubAsStateObject ();
    virtual Urho3D::Context *GetContext ();
    virtual void SendUrho3DEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    virtual void OnComponentAdded (Urho3D::SharedPtr <MapComponent> component);
    virtual void OnComponentRemoved (Urho3D::SharedPtr <MapComponent> component);
    virtual void OnComponentCreated (Urho3D::SharedPtr <MapComponent> component);
    virtual void CheckAndSendCollisions ();
public:
    Map (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep) ;
    virtual bool Dispose ();
    virtual bool LoadFromXML (Urho3D::XMLElement rootElement);
    virtual Urho3D::XMLElement SaveToXML (Urho3D::XMLElement &parentElement);
    virtual bool ProcessEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    Urho3D::SharedPtr <Player> GetPlayerById (Urho3D::StringHash id);
    void OnObjectAdded (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void OnObjectRemoved (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    Urho3D::SharedPtr <MapObject> GetMapObjectById (int id);

    Urho3D::Vector <AmbientObject *> *GetAmbientsVector ();
    void ClearAmbients ();
    void AddAmbient (AmbientObject *ambient);
    bool RemoveAmbient (AmbientObject *ambient);
    virtual ~Map ();
};
}

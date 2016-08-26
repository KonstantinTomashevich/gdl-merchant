#pragma once
#include <Urho3D/Core/Context.h>
#include <Engine/Interfaces/Component.hpp>
#include <Engine/Interfaces/ComponentsHolder.hpp>
#include "PlayerProtectedData.hpp"

namespace GameEngine
{
class Caravan;
class Player : public Component, public ComponentsHolder <PlayerComponent>
{
friend class PlayerProtectedData;
URHO3D_OBJECT (Player, Component)
protected:
    Urho3D::String id_;
    Urho3D::Color color_;
    float gold_;
    Urho3D::HashMap <Urho3D::StringHash, float> relations_;
    PlayerProtectedData protectedData_;

    virtual void OnComponentAdded (Urho3D::SharedPtr <PlayerComponent> component);
    virtual void OnComponentRemoved (Urho3D::SharedPtr <PlayerComponent> component);
    virtual void OnComponentCreated (Urho3D::SharedPtr <PlayerComponent> component);
public:
    Player (Urho3D::Context *context, Urho3D::String id = Urho3D::String::EMPTY, float gold = 0.0f, Urho3D::Color color = Urho3D::Color ());
    virtual bool Init ();
    virtual bool Update (float timeStep) ;
    virtual bool Dispose ();
    virtual bool LoadFromXML (Urho3D::XMLElement rootElement);
    virtual bool SaveToXML (Urho3D::XMLElement parentElement);
    virtual bool ProcessEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    Urho3D::String GetId ();
    float GetGold ();
    Urho3D::Color GetColor ();
    float GetRelationsWith (Urho3D::StringHash id);
    Urho3D::SharedPtr <Caravan> CreateCaravan (Map *map, int storageSlots = 8, Urho3D::Vector2 position = Urho3D::Vector2::ZERO);
    virtual ~Player ();
};
}

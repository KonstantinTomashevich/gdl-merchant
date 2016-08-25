#include "Component.hpp"
#include <Engine/Engine.hpp>
#include <Engine/Objects/Player.hpp>
#include <Engine/Objects/PlayerProtectedData.hpp>
#include <Engine/Objects/Map.hpp>

namespace GameEngine
{
Component::Component (Urho3D::Context *context) : StatesEngine::StateObject (context)
{

}

Component::~Component ()
{

}

PlayerComponent::PlayerComponent (Urho3D::Context *context) : Component (context)
{
    player_ = 0;
    playerData_ = 0;
}

Player *PlayerComponent::GetPlayer ()
{
    return player_;
}

void PlayerComponent::SetPlayer (Player *player, PlayerProtectedData *playerData)
{
    assert (player);
    assert (playerData);
    player_ = player;
    playerData_ = playerData;
}

PlayerComponent::~PlayerComponent ()
{

}

void EventListener::ProcessDeferredEvents()
{
    if (!deferredEvents_.Empty ())
    {
        for (int index = 0; index < deferredEvents_.Size (); index++)
        {
            Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> event = deferredEvents_.At (index);
            ProcessEvent (event.first_, event.second_);
        }
        deferredEvents_.Clear ();
    }
}

bool EventListener::SendDeferredEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    deferredEvents_.Push (Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> (eventType, eventData));
}

MapComponent::MapComponent (Urho3D::Context *context) : Component (context)
{

}

Map *MapComponent::GetMap ()
{
    return map_;
}

void MapComponent::SetMap (Map *map)
{
    assert (map);
    map_ = map;
}

MapComponent::~MapComponent ()
{

}
}


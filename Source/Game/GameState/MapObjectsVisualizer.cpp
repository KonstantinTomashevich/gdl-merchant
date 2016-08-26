#include "MapObjectsVisualizer.hpp"
#include <StatesEngine/StatesEngineEvents.hpp>
#include <Urho3D/IO/Log.h>
#include <Engine/Objects/Map.hpp>

namespace GameState
{
MapObjectsVisualizer::MapObjectsVisualizer (Urho3D::Context *context) : GameEngine::MapComponent (context),
    sceneContainer_ (), villages_ (), caravans_ ()
{

}

bool MapObjectsVisualizer::Init ()
{
    if (map_ && sceneContainer_)
    {
        SubscribeToEvent (map_, StatesEngine::Events::E_STATE_OBJECT_ADDED_TO_HUB,
                          URHO3D_HANDLER (MapObjectsVisualizer, OnMapChildAdded));
        SubscribeToEvent (map_, StatesEngine::Events::E_STATE_OBJECT_REMOVED_FROM_HUB,
                          URHO3D_HANDLER (MapObjectsVisualizer, OnMapChildRemoved));
        ready_ = true;
        return true;
    }
    else
        return false;
}

bool MapObjectsVisualizer::Update (float timeStep)
{

}

bool MapObjectsVisualizer::Dispose ()
{
    if (ready_)
    {
        UnsubscribeFromAllEvents ();
        ready_ = false;
        return true;
    }
    else
        return false;
}

bool MapObjectsVisualizer::ProcessEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    return false;
}

bool MapObjectsVisualizer::LoadFromXML (Urho3D::XMLElement rootElement)
{
    villages_.Clear ();
    caravans_.Clear ();
    return true;
}

bool MapObjectsVisualizer::SaveToXML (Urho3D::XMLElement &parentElement)
{
    //TODO: Implement this.
}

void MapObjectsVisualizer::OnMapChildAdded (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    context_->GetSubsystem <Urho3D::Log> ()->Write (Urho3D::LOG_INFO, "Added");
}

void MapObjectsVisualizer::OnMapChildRemoved (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    context_->GetSubsystem <Urho3D::Log> ()->Write (Urho3D::LOG_INFO, "Removed");
}

StatesEngine::SceneContainer *MapObjectsVisualizer::GetSceneContainer ()
{
    return sceneContainer_;
}

void MapObjectsVisualizer::SetSceneContainer (StatesEngine::SceneContainer *sceneContainer)
{
    sceneContainer_ = sceneContainer;
}

MapObjectsVisualizer::~MapObjectsVisualizer()
{

}
}


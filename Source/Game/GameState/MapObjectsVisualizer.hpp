#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Node.h>

#include <StatesEngine/SceneContainer.hpp>
#include <Game/InputMapper.hpp>
#include <Engine/Interfaces/Component.hpp>

namespace GameState
{
class MapObjectsVisualizer : public GameEngine::MapComponent
{
URHO3D_OBJECT (MapObjectsVisualizer, StatesEngine::StateObject)
protected:
    StatesEngine::SceneContainer *sceneContainer_;
    Urho3D::Vector <Urho3D::Node *> villages_;
    Urho3D::Vector <Urho3D::Node *> caravans_;
public:
    MapObjectsVisualizer (Urho3D::Context *context);
    bool Init ();
    bool Update (float timeStep);
    bool Dispose ();

    bool ProcessEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    bool LoadFromXML (Urho3D::XMLElement rootElement);
    virtual bool SaveToXML (Urho3D::XMLElement parentElement);
    void OnMapChildAdded (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void OnMapChildRemoved (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    StatesEngine::SceneContainer *GetSceneContainer ();
    void SetSceneContainer (StatesEngine::SceneContainer *sceneContainer);
    virtual ~MapObjectsVisualizer ();
};
}

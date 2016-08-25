#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Node.h>
#include <StatesEngine/StateObject.hpp>
#include <Game/InputMapper.hpp>

namespace GameState
{
class GameInputListener : public StatesEngine::StateObject
{
URHO3D_OBJECT (GameInputListener, StatesEngine::StateObject)
protected:
    Urho3D::Node *cameraNode_;
    float cameraSpeed_;

    void ListenCameraInput (InputMapper *inputMapper, float timeStep);
public:
    GameInputListener (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep);
    virtual bool Dispose ();

    Urho3D::Node *GetCameraNode ();
    void SetCameraNode (Urho3D::Node *cameraNode);
    float GetCameraSpeed ();
    void SetCameraSpeed (float cameraSpeed);
    virtual ~GameInputListener ();
};
}

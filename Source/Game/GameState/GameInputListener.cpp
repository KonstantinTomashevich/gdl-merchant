#include "GameInputListener.hpp"
#include <Game/BuildConfig.hpp>

namespace GameState
{
void GameInputListener::ListenCameraInput (InputMapper *inputMapper, float timeStep)
{
    Urho3D::Vector2 move = Urho3D::Vector2::ZERO;
    if (inputMapper->Is ("move_up"))
        move.y_ += 1;
    if (inputMapper->Is ("move_down"))
        move.y_ -= 1;
    if (inputMapper->Is ("move_right"))
        move.x_ += 1;
    if (inputMapper->Is ("move_left"))
        move.x_ -= 1;
    move *= cameraSpeed_ * timeStep;
    cameraNode_->SetPosition2D (cameraNode_->GetPosition2D () + move);
}

GameInputListener::GameInputListener (Urho3D::Context *context) : StatesEngine::StateObject (context)
{
    cameraNode_ = 0;
    cameraSpeed_ = 10.0f;
}

bool GameInputListener::Init ()
{
    if (!ready_ && cameraNode_)
    {
        ready_ = true;
        return true;
    }
    else
        return false;
}

bool GameInputListener::Update (float timeStep)
{
    InputMapper *inputMapper = context_->GetSubsystem <InputMapper> ();
    ListenCameraInput (inputMapper, timeStep);
    return true;
}

bool GameInputListener::Dispose ()
{
    if (ready_)
    {
        UnsubscribeFromAllEvents ();
        return true;
    }
    else
        return false;
}

Urho3D::Node *GameInputListener::GetCameraNode ()
{
    return cameraNode_;
}

void GameInputListener::SetCameraNode (Urho3D::Node *cameraNode)
{
    assert (cameraNode);
    cameraNode_ = cameraNode;
}

float GameInputListener::GetCameraSpeed ()
{
    return cameraSpeed_;
}

void GameInputListener::SetCameraSpeed (float cameraSpeed)
{
    cameraSpeed_ = cameraSpeed;
}

GameInputListener::~GameInputListener()
{
    Dispose ();
}
}


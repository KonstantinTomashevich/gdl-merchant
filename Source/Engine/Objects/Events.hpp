#pragma once
#include <Engine/Engine.hpp>

namespace GameEngine
{
namespace Events
{
/// Map sends this if this 2 objects BoundingCircle's IsIntersects returns true.
GameEngineEvent (ObjectsCollide)
{
    /// MapObject pointer.
    GameEngineEventParameter (ObjectA);
    /// MapObject pointer.
    GameEngineEventParameter (ObjectB);
}

GameEngineEvent (Order)
{
    /// Urho3D::StringHash.
    GameEngineEventParameter (Type);
    /// bool.
    GameEngineEventParameter (IsDeletePrevious);
}
}
}


#pragma once
#include <Urho3D/Math/Vector2.h>

namespace GameEngine
{
class BoundingCircle
{
public:
    Urho3D::Vector2 position_;
    float radius_;

    BoundingCircle ();
    BoundingCircle (Urho3D::Vector2 position, float radius);

    bool IsIntersects (BoundingCircle &other);
    bool IsContains (BoundingCircle &other);

    operator == (const BoundingCircle &other);
    operator != (const BoundingCircle &other);
};
}

#include "BoundingCircle.hpp"

namespace GameEngine
{
BoundingCircle::BoundingCircle ()
{
    position_ = Urho3D::Vector2 ();
    radius_ = 0.0f;
}

BoundingCircle::BoundingCircle (Urho3D::Vector2 position, float radius)
{
    position_ = position;
    radius_ = radius;
}

bool BoundingCircle::IsIntersects (BoundingCircle &other)
{
    return ( (position_ - other.position_).Length () <= radius_ + other.radius_);
}

bool BoundingCircle::IsContains (BoundingCircle &other)
{
    return ( (position_ - other.position_).Length () < radius_ - other.radius_);
}

BoundingCircle::operator ==(const BoundingCircle &other)
{
    return (position_ == other.position_ && radius_ == other.radius_);
}

BoundingCircle::operator !=(const BoundingCircle &other)
{
    return (position_ != other.position_ || radius_ != other.radius_);
}
}


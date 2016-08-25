#include "MapObject.hpp"
#include <Engine/Engine.hpp>

namespace GameEngine
{
MapObject::MapObject (Urho3D::Context *context) : Component (context), bounds_ (), maxCollisionDistance_ (1000)
{

}

bool MapObject::LoadFromXML (Urho3D::XMLElement rootElement)
{
    assert (rootElement.HasAttribute ("position"));
    assert (rootElement.HasAttribute ("radius"));
    if (!rootElement.HasAttribute ("position") || !rootElement.HasAttribute ("radius"))
        return false;

    bounds_.position_ = rootElement.GetVector2 ("position");
    bounds_.radius_ = rootElement.GetFloat ("radius");
    if (rootElement.HasAttribute ("maxCollisionDistance"))
        maxCollisionDistance_ = rootElement.GetFloat ("maxCollisionDistance");
    return true;
}

Urho3D::Vector2 MapObject::GetPosition ()
{
    return bounds_.position_;
}

void MapObject::SetPosition (Urho3D::Vector2 position)
{
    bounds_.position_ = position;
}

BoundingCircle *MapObject::GetBounds ()
{
    return &bounds_;
}

float MapObject::GetMaxCollisionDistance ()
{
    return maxCollisionDistance_;
}

void MapObject::SetMaxCollisionDistance (float maxCollisionDistance)
{
    maxCollisionDistance_ = maxCollisionDistance;
}

bool MapObject::IsCollides (MapObject *other)
{
    if ((GetPosition () - other->GetPosition ()).Length () > maxCollisionDistance_)
        return false;
    else
        return other->GetBounds ()->IsIntersects (bounds_);
}

MapObject::~MapObject ()
{

}
}


#include "MapObject.hpp"
#include <Engine/Engine.hpp>

namespace GameEngine
{
MapObject::MapObject (Urho3D::Context *context) : Component (context), bounds_ (), maxCollisionDistance_ (1000)
{
    xmlPrefabPath_ = Urho3D::String::EMPTY;
    prefabRotation_ = 0.0f;
    prefabScale_ = 1.0f;
}

bool MapObject::LoadFromXML (Urho3D::XMLElement rootElement)
{
    assert (rootElement.HasAttribute ("position"));
    assert (rootElement.HasAttribute ("radius"));
    assert (rootElement.HasAttribute ("xmlPrefabPath"));
    assert (rootElement.HasAttribute ("prefabRotation"));
    assert (rootElement.HasAttribute ("prefabScale"));
    if (!rootElement.HasAttribute ("position") || !rootElement.HasAttribute ("radius") ||
            !rootElement.HasAttribute ("xmlPrefabPath") || !rootElement.HasAttribute ("prefabRotation") ||
            !rootElement.HasAttribute ("prefabScale"))
        return false;

    bounds_.position_ = rootElement.GetVector2 ("position");
    bounds_.radius_ = rootElement.GetFloat ("radius");
    xmlPrefabPath_ = rootElement.GetAttribute ("xmlPrefabPath");
    prefabRotation_ = rootElement.GetFloat ("prefabRotation");
    prefabScale_ = rootElement.GetFloat ("prefabScale");
    if (rootElement.HasAttribute ("maxCollisionDistance"))
        maxCollisionDistance_ = rootElement.GetFloat ("maxCollisionDistance");
    return true;
}

bool MapObject::SaveToXML (Urho3D::XMLElement &parentElement)
{
    Urho3D::XMLElement saveElement = parentElement.CreateChild ("object");
    saveElement.SetAttribute ("type", this->GetTypeName ());
    saveElement.SetVector2 ("position", bounds_.position_);
    saveElement.SetFloat ("radius", bounds_.radius_);
    saveElement.SetAttribute ("xmlPrefabPath", xmlPrefabPath_);
    saveElement.SetFloat ("prefabRotation", prefabRotation_);
    saveElement.SetFloat ("prefabScale", prefabScale_);
    saveElement.SetFloat ("maxCollisionDistance", maxCollisionDistance_);
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

Urho3D::String MapObject::GetXMLPrefabPath ()
{
    return xmlPrefabPath_;
}

void MapObject::SetXMLPrefabPath (Urho3D::String xmlPrefabPath)
{
    xmlPrefabPath_ = xmlPrefabPath;
}

float MapObject::GetPrefabRotation ()
{
    return prefabRotation_;
}

void MapObject::SetPrefabRotation (float prefabRotation)
{
    prefabRotation_ = prefabRotation;
}

float MapObject::GetPrefabScale ()
{
    return prefabScale_;
}

void MapObject::SetPrefabScale (float prefabScale)
{
    prefabScale_ = prefabScale;
}

MapObject::~MapObject ()
{

}
}


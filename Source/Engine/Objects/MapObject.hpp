#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/Vector2.h>
#include <Engine/Utils/BoundingCircle.hpp>
#include <Engine/Interfaces/Component.hpp>

namespace GameEngine
{
class MapObject : public Component
{
URHO3D_OBJECT (MapObject, MapComponent)
protected:
    BoundingCircle bounds_;
    float maxCollisionDistance_;
public:
    MapObject (Urho3D::Context *context);
    virtual bool LoadFromXML (Urho3D::XMLElement rootElement);
    Urho3D::Vector2 GetPosition ();
    void SetPosition (Urho3D::Vector2 position);
    BoundingCircle *GetBounds ();
    float GetMaxCollisionDistance ();
    void SetMaxCollisionDistance (float maxCollisionDistance);
    bool IsCollides (MapObject *other);
    virtual ~MapObject ();
};
}

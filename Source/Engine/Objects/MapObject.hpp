#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/Vector2.h>
#include <Engine/Utils/BoundingCircle.hpp>
#include <Engine/Interfaces/Component.hpp>

namespace GameEngine
{
class MapObject : public Component
{
URHO3D_OBJECT (MapObject, Component)
protected:
    BoundingCircle bounds_;
    float maxCollisionDistance_;
    Urho3D::String xmlPrefabPath_;
    float prefabRotation_;
    float prefabScale_;
    int identifierNumber_;
public:
    MapObject (Urho3D::Context *context);
    virtual bool LoadFromXML (Urho3D::XMLElement rootElement);
    virtual Urho3D::XMLElement SaveToXML (Urho3D::XMLElement &parentElement);
    Urho3D::Vector2 GetPosition ();
    void SetPosition (Urho3D::Vector2 position);
    BoundingCircle *GetBounds ();
    float GetMaxCollisionDistance ();
    void SetMaxCollisionDistance (float maxCollisionDistance);
    bool IsCollides (MapObject *other);

    Urho3D::String GetXMLPrefabPath ();
    void SetXMLPrefabPath (Urho3D::String xmlPrefabPath);
    float GetPrefabRotation ();
    void SetPrefabRotation (float prefabRotation);
    float GetPrefabScale ();
    void SetPrefabScale (float prefabScale);
    int GetIdentifierNumber ();
    void SetIdentifierNumber (int identifierNumber);
    virtual ~MapObject ();
};
}

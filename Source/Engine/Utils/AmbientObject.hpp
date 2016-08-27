#pragma once
#include <Urho3D/Container/Str.h>
#include <Urho3D/Math/Vector2.h>

namespace GameEngine
{
class AmbientObject
{
public:
    AmbientObject ();
    virtual ~AmbientObject ();

    Urho3D::Vector2 position_;
    Urho3D::String xmlPrefabPath_;
    float rotation_;
    float scale_;
};
}

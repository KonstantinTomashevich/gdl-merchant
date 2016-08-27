#pragma once
#include <Engine/Interfaces/Component.hpp>
#include <Engine/Objects/MapObject.hpp>
#include <Engine/Utils/AmbientObject.hpp>

namespace GameEngine
{
class MapVisualizer : public MapComponent
{
URHO3D_OBJECT (MapVisualizer, MapComponent)
public:
    MapVisualizer (Urho3D::Context *context);
    virtual bool AddObject (MapObject *object) = 0;
    virtual bool AddAmbient (AmbientObject *ambient) = 0;
    virtual bool RemoveObject (MapObject *object) = 0;
    virtual bool RemoveAmbient (AmbientObject *ambient) = 0;
    virtual ~MapVisualizer ();
};
}

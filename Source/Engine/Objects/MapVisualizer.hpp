#pragma once
#include <Engine/Interfaces/Component.hpp>
#include <Engine/Objects/MapObject.hpp>
#include <Engine/Utils/AmbientObject.hpp>

namespace GameEngine
{
enum RenderingType
{
    RENDERING_2D,
    RENDERING_3D
};

class MapVisualizer : public MapComponent
{
URHO3D_OBJECT (MapVisualizer, MapComponent)
protected:
    Urho3D::String terrain2dImagePath_;
    Urho3D::Vector2 terrain2dSize_;
    Urho3D::String terrain3dHeightmapPath_;
    Urho3D::String terrain3dMaterialPath_;
    Urho3D::Vector3 terrain3dSpacings_;
    RenderingType renderingType_;
public:
    MapVisualizer (Urho3D::Context *context);
    virtual bool AddObject (MapObject *object) = 0;
    virtual bool AddAmbient (AmbientObject *ambient) = 0;
    virtual bool RemoveObject (MapObject *object) = 0;
    virtual bool RemoveAmbient (AmbientObject *ambient) = 0;

    virtual bool LoadFromXML (Urho3D::XMLElement rootElement);
    virtual Urho3D::XMLElement SaveToXML (Urho3D::XMLElement &parentElement);
    Urho3D::String GetTerrain2dImagePath ();
    void SetTerrain2dImagePath (Urho3D::String terrain2dImagePath);

    Urho3D::Vector2 GetTerrain2dSize ();
    void SetTerrain2dSize (Urho3D::Vector2 terrain2dSize);
    Urho3D::String GetTerrain3dHeightmapPath ();
    void SetTerrain3dHeightmapPath (Urho3D::String terrain3dHeightmapPath);

    Urho3D::String GetTerrain3dMaterialPath ();
    void SetTerrain3dMaterialPath (Urho3D::String terrain3dMaterialPath);
    Urho3D::Vector3 GetTerrain3dSpacings ();
    void SetTerrain3dSpacings (Urho3D::Vector3 terrain3DSpacings);

    RenderingType GetRenderingType ();
    void SetRenderingType (RenderingType renderingType);
    virtual void ClearAll () = 0;
    virtual void UpdateTerrain () = 0;
    virtual ~MapVisualizer ();    
};
}

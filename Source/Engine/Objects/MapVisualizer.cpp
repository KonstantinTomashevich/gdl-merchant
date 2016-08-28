#include "MapVisualizer.hpp"
#include <Engine/Engine.hpp>

namespace GameEngine
{
MapVisualizer::MapVisualizer (Urho3D::Context *context) : MapComponent (context)
{

}

bool MapVisualizer::LoadFromXML (Urho3D::XMLElement rootElement)
{
    assert (rootElement.HasAttribute ("terrain2dImagePath"));
    assert (rootElement.HasAttribute ("terrain2dSize"));
    assert (rootElement.HasAttribute ("terrain3dHeightmapPath"));
    assert (rootElement.HasAttribute ("terrain3dMaterialPath"));
    assert (rootElement.HasAttribute ("terrain3dSpacings"));

    if (!rootElement.HasAttribute ("terrain2dImagePath") || !rootElement.HasAttribute ("terrain2dSize") ||
            !rootElement.HasAttribute ("terrain3dHeightmapPath") ||
            !rootElement.HasAttribute ("terrain3dMaterialPath") || !rootElement.HasAttribute ("terrain3dSpacings"))
        return false;

    terrain2dImagePath_ = rootElement.GetAttribute ("terrain2dImagePath");
    terrain2dSize_ = rootElement.GetVector2 ("terrain2dSize");
    terrain3dHeightmapPath_ = rootElement.GetAttribute ("terrain3dHeightmapPath");
    terrain3dMaterialPath_ = rootElement.GetAttribute ("terrain3dMaterialPath");
    terrain3dSpacings_ = rootElement.GetVector3 ("terrain3dSpacings");
    return true;
}

Urho3D::XMLElement MapVisualizer::SaveToXML (Urho3D::XMLElement &parentElement)
{
    Urho3D::XMLElement saveElement = parentElement.CreateChild ("component");
    saveElement.SetAttribute ("type", GetTypeName ());
    saveElement.SetAttribute ("terrain2dImagePath", terrain2dImagePath_);
    saveElement.SetVector2 ("terrain2dSize", terrain2dSize_);
    saveElement.SetAttribute ("terrain3dHeightmapPath", terrain3dHeightmapPath_);
    saveElement.SetAttribute ("terrain3dMaterialPath", terrain3dMaterialPath_);
    saveElement.SetVector3 ("terrain3dSpacings", terrain3dSpacings_);
    return saveElement;
}

Urho3D::String MapVisualizer::GetTerrain2dImagePath ()
{
    return terrain2dImagePath_;
}

void MapVisualizer::SetTerrain2dImagePath (Urho3D::String terrain2dImagePath)
{
    terrain2dImagePath_ = terrain2dImagePath;
}

Urho3D::Vector2 MapVisualizer::GetTerrain2dSize ()
{
    return terrain2dSize_;
}

void MapVisualizer::SetTerrain2dSize (Urho3D::Vector2 terrain2dSize)
{
    terrain2dSize_ = terrain2dSize;
}

Urho3D::String MapVisualizer::GetTerrain3dHeightmapPath ()
{
    return terrain3dHeightmapPath_;
}

void MapVisualizer::SetTerrain3dHeightmapPath (Urho3D::String terrain3dHeightmapPath)
{
    terrain3dHeightmapPath_ = terrain3dHeightmapPath;
}

Urho3D::String MapVisualizer::GetTerrain3dMaterialPath ()
{
    return terrain3dMaterialPath_;
}

void MapVisualizer::SetTerrain3dMaterialPath (Urho3D::String terrain3dMaterialPath)
{
    terrain3dMaterialPath_ = terrain3dMaterialPath;
}

Urho3D::Vector3 MapVisualizer::GetTerrain3dSpacings ()
{
    return terrain3dSpacings_;
}

void MapVisualizer::SetTerrain3dSpacings (Urho3D::Vector3 terrain3dSpacings)
{
    terrain3dSpacings_ = terrain3dSpacings;
}

RenderingType MapVisualizer::GetRenderingType ()
{
    return renderingType_;
}

void MapVisualizer::SetRenderingType (RenderingType renderingType)
{
    renderingType_ = renderingType;
}

MapVisualizer::~MapVisualizer ()
{

}
}

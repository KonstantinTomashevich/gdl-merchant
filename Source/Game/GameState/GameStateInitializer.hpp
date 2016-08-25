#pragma once
#include <Engine/Objects/Map.hpp>
#include <Urho3D/Urho2D/TileMap2D.h>
#include <Urho3D/Core/Context.h>
namespace GameState
{
void SetupGameState (Urho3D::Context *context);
namespace GameStateInitializer
{
void ParseTileMap (Urho3D::Context *context, Urho3D::SharedPtr <GameEngine::Map> gameMap, Urho3D::TileMap2D *tileMap);
}
}

#include "GameStateInitializer.hpp"
#include <Game/BuildConfig.hpp>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Audio/SoundListener.h>

#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Urho2D/TmxFile2D.h>
#ifdef GetObject
#undef  GetObject
#endif
#include <Urho3D/Urho2D/TileMapLayer2D.h>

#include <StatesEngine/StatesEngine.hpp>
#include <StatesEngine/SceneContainer.hpp>

#include "GameInputListener.hpp"
#include "MapObjectsVisualizer.hpp"

#include <Engine/Objects/Player.hpp>
#include <Engine/Objects/Village.hpp>
#include <Engine/Objects/Caravan.hpp>

namespace GameState
{
void SetupGameState (Urho3D::Context *context)
{
    // --- Clear StatesEngine ---
    StatesEngine::StatesEngine *statesEngine = context->GetSubsystem <StatesEngine::StatesEngine> ();
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->DisposeAll ("any");
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->RemoveAll ("any");

    // --- Get required for setup subsystems --
    Urho3D::ResourceCache *cache = context->GetSubsystem <Urho3D::ResourceCache> ();
    Urho3D::Graphics *graphics = context->GetSubsystem <Urho3D::Graphics> ();

    // --- Create scene ---
    Urho3D::SharedPtr <StatesEngine::SceneContainer> scene (new StatesEngine::SceneContainer (context));
    scene->Set (Urho3D::SharedPtr <Urho3D::Scene> (new Urho3D::Scene (context)));
    scene->Get ()->CreateComponent <Urho3D::Octree> ();

    // --- Load tile map ---
    Urho3D::Node *tileMapNode = scene->Get ()->CreateChild ("tileMap");
    Urho3D::TileMap2D *tileMap = tileMapNode->CreateComponent <Urho3D::TileMap2D> ();
    tileMap->SetTmxFile (cache->GetResource <Urho3D::TmxFile2D> ("Maps/TestMap.tmx"));

    // --- Setup camera and viewport ---
    Urho3D::Node *cameraNode = scene->Get ()->CreateChild ("camera");
    cameraNode->SetPosition2D (Urho3D::Random (0.0f, tileMap->GetInfo ().GetMapWidth ()),
                               Urho3D::Random (0.0f, tileMap->GetInfo ().GetMapWidth ()));
    cameraNode->CreateComponent <Urho3D::SoundListener> ();
    Urho3D::Camera *camera = cameraNode->CreateComponent <Urho3D::Camera> ();
    camera->SetOrthographic (true);
    camera->SetAutoAspectRatio (true);
    camera->SetOrthoSize (40.0f);
    scene->SetupSceneViewport (0, cameraNode, Urho3D::IntRect (0, 0, graphics->GetWidth (), graphics->GetHeight ()));

    // --- Initialize game engine with this map ---
    Urho3D::SharedPtr <GameEngine::Map> map (new GameEngine::Map (context));
    map->CreateComponent <GameEngine::TradeGoodsTypesContainer> (
                cache->GetResource <Urho3D::XMLFile> ("XML/trade_goods_types.xml")->GetRoot ("document_root"));

    Urho3D::SharedPtr <MapObjectsVisualizer> mapObjectsVisualizer = map->CreateComponent <MapObjectsVisualizer> ();
    mapObjectsVisualizer->SetSceneContainer (scene.Get ());
    mapObjectsVisualizer->Init ();
    GameStateInitializer::ParseTileMap (context, map, tileMap);
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Add (map);

    // --- Setup input listener ---
    Urho3D::SharedPtr <GameInputListener> inputListener (new GameInputListener (context));
    inputListener->SetCameraNode (cameraNode);
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Add (inputListener);
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Add (scene);
}

void GameStateInitializer::ParseTileMap (Urho3D::Context *context, Urho3D::SharedPtr <GameEngine::Map> gameMap, Urho3D::TileMap2D *tileMap)
{
    bool isObjectsLayerParsed = false;
    int layerIndex = 0;
    while (!isObjectsLayerParsed && layerIndex < tileMap->GetNumLayers ())
    {
        Urho3D::TileMapLayer2D *layer = tileMap->GetLayer (layerIndex);
        if (Urho3D::ToBool (layer->GetProperty ("isObjectsLayer")))
        {
            Urho3D::Vector <Urho3D::String> playersIds = layer->GetProperty ("players").Split (';');
            assert (playersIds.Size ());
            for (int index = 0; index < playersIds.Size (); index++)
            {
                Urho3D::String id = playersIds.At (index);
                assert (layer->HasProperty (id + "Gold"));
                assert (layer->HasProperty (id + "Color"));
                Urho3D::SharedPtr <GameEngine::Player> player (new GameEngine::Player (
                                                                   context, id, Urho3D::ToFloat (layer->GetProperty (id + "Gold")),
                                                                   Urho3D::ToColor (layer->GetProperty (id + "Color"))));
                gameMap->Add (player);
            }

            for (int objectIndex = 0; objectIndex < layer->GetNumObjects (); objectIndex++)
            {
                Urho3D::TileMapObject2D *object = layer->GetObject (objectIndex);
                if (object->GetType () == "Village")
                {
                    Urho3D::SharedPtr <GameEngine::Village> village (new GameEngine::Village (context));
                    village->SetName (object->GetName ());
                    assert (object->HasProperty ("population"));
                    assert (object->HasProperty ("production"));
                    village->SetPopulation (Urho3D::ToInt (object->GetProperty ("population")));
                    village->SetPosition (object->GetPosition () / tileMap->GetInfo ().tileWidth_);
                    gameMap->Add (village);

                    Urho3D::Vector <Urho3D::String> productionTypes = object->GetProperty ("production").Split (';');
                    for (int productionIndex = 0; productionIndex < productionTypes.Size (); productionIndex++)
                    {
                        Urho3D::Vector <Urho3D::String> typeInfo = productionTypes.At (productionIndex).Split ('=');
                        assert (Urho3D::ToFloat (typeInfo.At (1)) > 0.0f);
                        village->SetProduction (Urho3D::StringHash (typeInfo.At (0)), Urho3D::ToFloat (typeInfo.At (1)), true);
                    }
                }
                else if (object->GetType () == "Caravan")
                {
                    assert (object->HasProperty ("player"));
                    Urho3D::SharedPtr <GameEngine::Player> player = gameMap->GetPlayerById (object->GetProperty ("player"));
                    assert (player.NotNull ());
                    if (player.NotNull ())
                        player->CreateCaravan (gameMap.Get (), 8, object->GetPosition () / tileMap->GetInfo ().tileWidth_);
                }
            }
            isObjectsLayerParsed = true;
        }
        layerIndex++;
    }
    assert (isObjectsLayerParsed);
}
}

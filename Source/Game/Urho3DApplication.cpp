#include "Urho3DApplication.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Scene/Node.h>

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Resource/XMLElement.h>

#include <StatesEngine/StatesEngine.hpp>
#include <StatesEngine/StateObjectsManager.hpp>
#include <Game/GameState/GameStateInitializer.hpp>
#include "InputMapper.hpp"

#include <Engine/Objects/TradeGoodsTypesContainer.hpp>
#include <Engine/Objects/Village.hpp>
#include <Engine/Objects/Caravan.hpp>
#include <Engine/Objects/Player.hpp>
#include <Engine/Objects/Map.hpp>
#include <StatesEngine/SceneContainer.hpp>
#include <Game/GameState/GameInputListener.hpp>
#include <Game/GameState/MapObjectsVisualizer.hpp>

Urho3DApplication::Urho3DApplication (Urho3D::Context *context) : Urho3D::Application (context)
{

}

void Urho3DApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
}

void Urho3DApplication::Start ()
{
    Urho3D::SetRandomSeed (Urho3D::Time::GetTimeSinceEpoch ());
    StatesEngine::StatesEngine *statesEngine = new StatesEngine::StatesEngine (context_);
    statesEngine->Init ();
    context_->RegisterSubsystem (statesEngine);
    statesEngine->SetupState (Urho3D::SharedPtr <StatesEngine::StateObjectsManager> (new StatesEngine::StateObjectsManager (context_)));
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->SetIsUseLog (true);

    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseMode (Urho3D::MM_FREE);
    input->SetMouseVisible (true);
    context_->RegisterSubsystem (this);

    GameEngine::RegisterType <GameEngine::TradeGoodsTypesContainer> (context_);
    GameEngine::RegisterType <GameEngine::Village> (context_);
    GameEngine::RegisterType <GameEngine::Caravan> (context_);
    GameEngine::RegisterType <GameEngine::Player> (context_);
    GameEngine::RegisterType <GameEngine::Map> (context_);
    GameEngine::RegisterType <StatesEngine::SceneContainer> (context_);
    GameEngine::RegisterType <GameState::GameInputListener> (context_);
    GameEngine::RegisterType <GameState::MapObjectsVisualizer> (context_);

    Urho3D::ResourceCache *cache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    InputMapper *inputMapper = new InputMapper (context_);
    inputMapper->LoadFromXML (cache->GetResource <Urho3D::XMLFile> ("XML/input_map.xml")->GetRoot ("document_root"));
    context_->RegisterSubsystem (inputMapper);
    GameState::SetupGameState (context_);
}

void Urho3DApplication::Stop ()
{

}

Urho3DApplication::~Urho3DApplication ()
{

}
URHO3D_DEFINE_APPLICATION_MAIN (Urho3DApplication)

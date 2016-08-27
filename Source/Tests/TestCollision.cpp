#include "TestCollision.hpp"
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/IO/Log.h>

#include <Urho3D/Urho2D/TileMap2D.h>
#include <Urho3D/Urho2D/TmxFile2D.h>
#include <Urho3D/Urho2D/SpriteSheet2D.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>

#include <StatesEngine/StatesEngine.hpp>
#include <StatesEngine/StateObjectsManager.hpp>

#include <Engine/Objects/Map.hpp>
#include <Engine/Objects/Village.hpp>
#include <Engine/Objects/TradeGoodsTypesContainer.hpp>
#include <Engine/Objects/Caravan.hpp>
#include <Engine/Objects/Player.hpp>

TestCollision::TestCollision (Urho3D::Context *context) : Urho3D::Application(context)
{

}

void TestCollision::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
}

void TestCollision::Start ()
{
    Urho3D::SetRandomSeed (Urho3D::Time::GetTimeSinceEpoch ());
    context_->RegisterSubsystem (this);
    Urho3D::Log *log = context_->GetSubsystem <Urho3D::Log> ();
    log->Write (Urho3D::LOG_INFO, "Application started...");

    StatesEngine::StatesEngine *statesEngine = new StatesEngine::StatesEngine (context_);
    statesEngine->Init ();
    context_->RegisterSubsystem (statesEngine);
    statesEngine->SetupState (Urho3D::SharedPtr <StatesEngine::StateObjectsManager> (new StatesEngine::StateObjectsManager (context_)));
    log->Write (Urho3D::LOG_INFO, "StatesEngine initialized...");

    GameEngine::RegisterType <GameEngine::TradeGoodsTypesContainer> (context_);
    GameEngine::RegisterType <GameEngine::Village> (context_);
    GameEngine::RegisterType <GameEngine::Caravan> (context_);
    GameEngine::RegisterType <GameEngine::Player> (context_);
    GameEngine::RegisterType <GameEngine::Map> (context_);
    log->Write (Urho3D::LOG_INFO, "GameEngine classes registered...");

    Urho3D::SharedPtr <GameEngine::Map> map (new GameEngine::Map (context_));
    log->Write (Urho3D::LOG_INFO, "Map created!");
    Urho3D::ResourceCache *cache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    map->CreateComponent <GameEngine::TradeGoodsTypesContainer> (
                cache->GetResource <Urho3D::XMLFile> ("XML/trade_goods_types.xml")->GetRoot ("document_root"));
    log->Write (Urho3D::LOG_INFO, "Trade goods types loaded...");

    Urho3D::SharedPtr <GameEngine::Village> village (new GameEngine::Village (context_));
    map->Add (village);
    log->Write (Urho3D::LOG_INFO, "Village added...");

    village->GetStorage ()->SetSlotsCount (24);
    village->SetPopulation (1000);
    village->SetProduction ("Food", 2.0f);
    village->GetBounds ()->position_ = Urho3D::Vector2 (1.0f, 1.0f);
    village->GetBounds ()->radius_ = 2.0f;
    log->Write (Urho3D::LOG_INFO, "Village configured...");

    Urho3D::SharedPtr <GameEngine::Caravan> caravan1 (new GameEngine::Caravan (context_));
    Urho3D::SharedPtr <GameEngine::Caravan> caravan2 (new GameEngine::Caravan (context_));
    Urho3D::SharedPtr <GameEngine::Caravan> caravan3 (new GameEngine::Caravan (context_));
    log->Write (Urho3D::LOG_INFO, "Caravans created...");

    map->Add (caravan1);
    map->Add (caravan2);
    map->Add (caravan3);
    map->Init ();
    log->Write (Urho3D::LOG_INFO, "Caravans added...");

    caravan1->GetBounds ()->position_ = Urho3D::Vector2 ();
    caravan1->GetBounds ()->radius_ = 0.5f;
    caravan2->GetBounds ()->position_ = Urho3D::Vector2 (2.5f, 2.5f);
    caravan2->GetBounds ()->radius_ = 0.5f;
    caravan3->GetBounds ()->position_ = Urho3D::Vector2 (4.0f, 1.0f);
    caravan3->GetBounds ()->radius_ = 0.5f;
    log->Write (Urho3D::LOG_INFO, "Caravans configured...");

    log->SetLevel (Urho3D::LOG_DEBUG);
    map->SetIsUseLog (true);
    map->Update (1 / 60.0f);

    if (!village->IsCaravanIn (caravan1.Get ()))
        ErrorExit ("First collision is incorrect!");
    if (!village->IsCaravanIn (caravan2.Get ()))
        ErrorExit ("Second collision is incorrect!");
    if (village->IsCaravanIn (caravan3.Get ()))
        ErrorExit ("Third collision is incorrect!");
    if (caravan1->GetDistination () != village.Get ())
        ErrorExit ("First caravan destination is incorrect!");
    if (caravan2->GetDistination () != village.Get ())
        ErrorExit ("Second caravan destination is incorrect!");
    engine_->Exit ();
}

void TestCollision::Stop ()
{
    StatesEngine::StatesEngine *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngine> ();
    statesEngine->DisposeState ();
    context_->RemoveSubsystem ("StatesEngine");
}

TestCollision::~TestCollision ()
{

}
URHO3D_DEFINE_APPLICATION_MAIN (TestCollision)

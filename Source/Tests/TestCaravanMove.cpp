#include "TestCaravanMove.hpp"
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
#include <Engine/Objects/CaravanOrders.hpp>

TestCaravanMove::TestCaravanMove (Urho3D::Context *context) : Urho3D::Application(context)
{

}

void TestCaravanMove::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
}

void TestCaravanMove::Start ()
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

    Urho3D::SharedPtr <GameEngine::Player> player (new GameEngine::Player (context_, "Player"));
    map->Add (player);
    log->Write (Urho3D::LOG_INFO, "Player added...");
    Urho3D::SharedPtr <GameEngine::Caravan> caravan = player->CreateCaravan (map.Get ());
    caravan->GetBounds ()->position_ = Urho3D::Vector2 ();
    caravan->GetBounds ()->radius_ = 0.5f;
    map->Init ();
    log->Write (Urho3D::LOG_INFO, "Caravan added...");

    Urho3D::Vector2 moveTarget = Urho3D::Vector2 (Urho3D::Random (-10.0f, 10.0f), Urho3D::Random (-10.0f, 10.0f));
    Urho3D::VariantMap orderData;
    orderData [GameEngine::CaravanOrders::MoveData::Target] = moveTarget;
    caravan->Order (GameEngine::CaravanOrders::Move, orderData);
    log->Write (Urho3D::LOG_INFO, "Caravan order sended...");

    map->SetIsUseLog (true);
    log->SetLevel (Urho3D::LOG_DEBUG);
    for (int cycle = 0; cycle < 7200.0f; cycle++)
    {
        map->Update (1 / 60.0f);
        if ((caravan->GetBounds ()->position_ - moveTarget).Length () < caravan->GetBounds ()->radius_)
            cycle = 7200;;
    }

    if ((caravan->GetBounds ()->position_ - moveTarget).Length () < caravan->GetBounds ()->radius_)
        engine_->Exit ();
    else
        ErrorExit ("Caravan don't reached target after 2 minutes!");
}

void TestCaravanMove::Stop ()
{
    StatesEngine::StatesEngine *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngine> ();
    statesEngine->DisposeState ();
    context_->RemoveSubsystem ("StatesEngine");
}

TestCaravanMove::~TestCaravanMove ()
{

}
URHO3D_DEFINE_APPLICATION_MAIN (TestCaravanMove)

#include "TestOrdersSequence.hpp"
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

TestOrdersSequence::TestOrdersSequence (Urho3D::Context *context) : Urho3D::Application(context)
{

}

void TestOrdersSequence::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
}

void TestOrdersSequence::Start ()
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

    Urho3D::Vector2 moveTarget1 = Urho3D::Vector2 (-5.0f, 0.0f);
    Urho3D::Vector2 moveTarget2 = Urho3D::Vector2 (0.0f, 5.0f);
    Urho3D::Vector2 moveTarget3 = Urho3D::Vector2 (5.0f, 5.0f);
    log->Write (Urho3D::LOG_INFO, "Setted move targets...");

    Urho3D::SharedPtr <GameEngine::Player> player (new GameEngine::Player (context_, "Player"));
    map->Add (player);
    log->Write (Urho3D::LOG_INFO, "Player added...");
    Urho3D::SharedPtr <GameEngine::Caravan> caravan = player->CreateCaravan (map.Get ());
    caravan->GetBounds ()->position_ = Urho3D::Vector2 ();
    caravan->GetBounds ()->radius_ = 0.5f;
    map->Init ();
    log->Write (Urho3D::LOG_INFO, "Caravan added...");

    Urho3D::VariantMap orderData1;
    orderData1 [GameEngine::CaravanOrders::MoveData::Target] = moveTarget1;
    caravan->Order (GameEngine::CaravanOrders::Move, orderData1);

    Urho3D::VariantMap orderData2;
    orderData2 [GameEngine::CaravanOrders::MoveData::Target] = moveTarget2;
    caravan->Order (GameEngine::CaravanOrders::Move, orderData2);

    Urho3D::VariantMap orderData3;
    orderData3 [GameEngine::CaravanOrders::MoveData::Target] = moveTarget3;
    caravan->Order (GameEngine::CaravanOrders::Move, orderData3);
    log->Write (Urho3D::LOG_INFO, "Caravan orders sended...");

    map->SetIsUseLog (true);
    log->SetLevel (Urho3D::LOG_DEBUG);
    bool isOrder1Done = false;
    bool isOrder2Done = false;
    bool isOrder3Done = false;

    for (int cycle = 0; cycle < 14400.0f; cycle++)
    {
        map->Update (1 / 60.0f);
        if (!isOrder1Done && (caravan->GetBounds ()->position_ - moveTarget1).Length () < caravan->GetBounds ()->radius_)
            isOrder1Done = true;
        if (!isOrder2Done && (caravan->GetBounds ()->position_ - moveTarget2).Length () < caravan->GetBounds ()->radius_)
            isOrder2Done = true;
        if (!isOrder3Done && (caravan->GetBounds ()->position_ - moveTarget3).Length () < caravan->GetBounds ()->radius_)
            isOrder3Done = true;

        if (isOrder3Done && !isOrder1Done)
            cycle = 14400; // Third order done before first!
        else if (isOrder3Done && !isOrder2Done)
            cycle = 14400; // Third order done before second!
        else if (isOrder2Done && !isOrder1Done)
            cycle = 14400; // Second order done before first!
        else if (isOrder1Done && isOrder2Done && isOrder3Done)
            cycle = 14400; // All orders completed!
    }

    if (isOrder3Done && !isOrder1Done)
        ErrorExit ("Third order done before first!");
    else if (isOrder3Done && !isOrder2Done)
        ErrorExit ("Third order done before second!");
    else if (isOrder2Done && !isOrder1Done)
        ErrorExit ("Second order done before first, error!");
    else if (isOrder1Done && isOrder2Done && isOrder3Done)
        engine_->Exit ();
    else
        ErrorExit ("Caravan don't reached target after 2 minutes!");
}

void TestOrdersSequence::Stop ()
{
    StatesEngine::StatesEngine *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngine> ();
    statesEngine->DisposeState ();
    context_->RemoveSubsystem ("StatesEngine");
}

TestOrdersSequence::~TestOrdersSequence ()
{

}
URHO3D_DEFINE_APPLICATION_MAIN (TestOrdersSequence)

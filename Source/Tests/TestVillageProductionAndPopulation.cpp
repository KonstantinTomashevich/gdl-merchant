#include "TestVillageProductionAndPopulation.hpp"
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

TestVillageProductionAndPopulation::TestVillageProductionAndPopulation (Urho3D::Context *context) : Urho3D::Application(context)
{

}

void TestVillageProductionAndPopulation::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
}

void TestVillageProductionAndPopulation::Start ()
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

    const int villageStorageSlotsCount = 24;
    const int populationAtStart = 1000;
    const int foodProductionAtStart = 2.0f;
    const int maxTestCycles = 60.0f * 2.5f * 60.0f; // Village population increase time * 2.5 * 60 frames per second
    log->Write (Urho3D::LOG_INFO, "Constants configured...");

    Urho3D::SharedPtr <GameEngine::Village> village (new GameEngine::Village (context_));
    map->Add (village);
    map->Init ();
    log->Write (Urho3D::LOG_INFO, "Village added...");

    village->GetStorage ()->SetStorageSlotsCount (villageStorageSlotsCount);
    village->SetPopulation (populationAtStart);
    village->SetProduction ("Food", foodProductionAtStart);
    log->Write (Urho3D::LOG_INFO, "Village configured...");

    log->SetLevel (Urho3D::LOG_DEBUG);
    map->SetIsUseLog (true);
    for (int cycle = 0; cycle < maxTestCycles; cycle++)
        map->Update (1 / 60.0f);
    log->Write (Urho3D::LOG_INFO, "Testing time elapsed...");

    log->Write (Urho3D::LOG_INFO, Urho3D::String ("Population: ") + Urho3D::String (village->GetPopulation ())+
                Urho3D::String ("\n(At start: ") + Urho3D::String (populationAtStart) + Urho3D::String (")") +
                Urho3D::String ("\nProduction: ") + Urho3D::String (village->GetProductionOf ("Food")) +
                Urho3D::String ("\n(At start: ") + Urho3D::String (foodProductionAtStart) + Urho3D::String (")") +
                Urho3D::String ("\nFood on storage: ") + Urho3D::String (village->GetStorage ()->GetAmountOf ("Food")));

    if (village->GetPopulation () > populationAtStart && village->GetProductionOf ("Food") > foodProductionAtStart &&
            village->GetStorage ()->GetAmountOf ("Food") > foodProductionAtStart * 10.0f * 2.5f)
        engine_->Exit ();
    else
        ErrorExit ("Population and production don't increase as expected!");
}

void TestVillageProductionAndPopulation::Stop ()
{
    StatesEngine::StatesEngine *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngine> ();
    statesEngine->DisposeState ();
    context_->RemoveSubsystem ("StatesEngine");
}

TestVillageProductionAndPopulation::~TestVillageProductionAndPopulation ()
{

}
URHO3D_DEFINE_APPLICATION_MAIN (TestVillageProductionAndPopulation)

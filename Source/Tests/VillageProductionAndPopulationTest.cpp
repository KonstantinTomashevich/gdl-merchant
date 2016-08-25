#include "VillageProductionAndPopulationTest.hpp"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLElement.h>
#include <Urho3D/Resource/XMLFile.h>

Urho3D::String VillageProductionAndPopulationTest::CreateResultInfo()
{
    return Urho3D::String ("Population: ") + Urho3D::String (village_->GetPopulation ())+
            Urho3D::String ("\n(At start: ") + Urho3D::String (populationAtStart_) + Urho3D::String (")") +
            Urho3D::String ("\nProduction: ") + Urho3D::String (village_->GetProductionOf ("Food")) +
            Urho3D::String ("\n(At start: ") + Urho3D::String (productionAtStart_) + Urho3D::String (")") +
            Urho3D::String ("\nFood on storage: ") + Urho3D::String (village_->GetStorage ()->GetAmountOf ("Food"));
}

VillageProductionAndPopulationTest::VillageProductionAndPopulationTest (Urho3D::Context *context) : TestingEngine::Test (context)
{
    name_ = "Village production and population";
    lastCycle_ = 0;
    maxCycles_ = 60.0f * 2.5f * 60.0f; // Village population increase time * 2.5 * 60 frames per second
    populationAtStart_ = 1000;
    productionAtStart_ = 2.0f;
}

void VillageProductionAndPopulationTest::Init ()
{
    Urho3D::SetRandomSeed (Urho3D::Time::GetTimeSinceEpoch ());
    map_ = Urho3D::SharedPtr <GameEngine::Map> (new GameEngine::Map (context_));
    Urho3D::ResourceCache *cache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    map_->CreateComponent <GameEngine::TradeGoodsTypesContainer> (
                cache->GetResource <Urho3D::XMLFile> ("XML/trade_goods_types.xml")->GetRoot ("document_root"));

    village_ = Urho3D::SharedPtr <GameEngine::Village> (new GameEngine::Village (context_));
    map_->Add (village_);
    map_->Init ();
    village_->GetStorage ()->SetStorageSlotsCount (24);
    village_->SetPopulation (populationAtStart_);
    village_->SetProduction ("Food", productionAtStart_);
}

void VillageProductionAndPopulationTest::Update (float timeStep)
{
    if (lastCycle_ < maxCycles_)
        for (int cycle = 0; cycle < 100; cycle++)
        {
            lastCycle_++;
            map_->Update (1 / 60.0f);
        }
    else
    {
        if (village_->GetPopulation () > populationAtStart_ && village_->GetProductionOf ("Food") > productionAtStart_ &&
                village_->GetStorage ()->GetAmountOf ("Food") > productionAtStart_ * 10.0f * 2.5f)
        {
            TEST_PASSED (CreateResultInfo ());
        }
        else
        {
            TEST_FAILED (CreateResultInfo ());
        }
    }
}

VillageProductionAndPopulationTest::~VillageProductionAndPopulationTest ()
{

}


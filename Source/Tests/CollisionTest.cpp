#include "CollisionTest.hpp"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLElement.h>
#include <Urho3D/Resource/XMLFile.h>

CollisionTest::CollisionTest (Urho3D::Context *context) : TestingEngine::Test (context)
{
    name_ = "Collsion test";
}

void CollisionTest::Init ()
{
    Urho3D::SetRandomSeed (Urho3D::Time::GetTimeSinceEpoch ());
    map_ = Urho3D::SharedPtr <GameEngine::Map> (new GameEngine::Map (context_));
    Urho3D::ResourceCache *cache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    map_->CreateComponent <GameEngine::TradeGoodsTypesContainer> (
                cache->GetResource <Urho3D::XMLFile> ("XML/trade_goods_types.xml")->GetRoot ("document_root"));

    village_ = Urho3D::SharedPtr <GameEngine::Village> (new GameEngine::Village (context_));
    caravan1_ = Urho3D::SharedPtr <GameEngine::Caravan> (new GameEngine::Caravan (context_));
    caravan2_ = Urho3D::SharedPtr <GameEngine::Caravan> (new GameEngine::Caravan (context_));
    caravan3_ = Urho3D::SharedPtr <GameEngine::Caravan> (new GameEngine::Caravan (context_));

    map_->Add (village_);
    map_->Add (caravan1_);
    map_->Add (caravan2_);
    map_->Add (caravan3_);
    map_->Init ();

    village_->GetStorage ()->SetStorageSlotsCount (24);
    village_->SetPopulation (1000);
    village_->SetProduction ("Food", 2.0f);

    village_->GetBounds ()->position_ = Urho3D::Vector2 (1.0f, 1.0f);
    village_->GetBounds ()->radius_ = 2.0f;
    caravan1_->GetBounds ()->position_ = Urho3D::Vector2 ();
    caravan1_->GetBounds ()->radius_ = 0.5f;

    caravan2_->GetBounds ()->position_ = Urho3D::Vector2 (2.5f, 2.5f);
    caravan2_->GetBounds ()->radius_ = 0.5f;
    caravan3_->GetBounds ()->position_ = Urho3D::Vector2 (4.0f, 1.0f);
    caravan3_->GetBounds ()->radius_ = 0.5f;
    map_->Update (1/ 60.0f);

    if (!village_->IsCaravanIn (caravan1_.Get ()))
    {
        TEST_FAILED ("Collision 1 don't works correctly!");
    }

    if (!village_->IsCaravanIn (caravan2_.Get ()))
    {
        TEST_FAILED ("Collision 2 don't works correctly!");
    }

    if (village_->IsCaravanIn (caravan3_.Get ()))
    {
        TEST_FAILED ("Collision 3 don't works correctly!");
    }

    if (caravan1_->GetDistination () != village_.Get ())
    {
        TEST_FAILED ("Caravan1 destination is incorrect!");
    }

    if (caravan2_->GetDistination () != village_.Get ())
    {
        TEST_FAILED ("Caravan2 destination is incorrect!");
    }

    if (caravan3_->GetDistination () != 0)
    {
        TEST_FAILED ("Caravan3 destination is incorrect!");
    }
    TEST_PASSED ("All collisions works correcly!");
}

void CollisionTest::Update (float timeStep)
{

}

CollisionTest::~CollisionTest ()
{

}


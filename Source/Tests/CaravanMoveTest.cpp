#include "CaravanMoveTest.hpp"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLElement.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Engine/Objects/CaravanOrders.hpp>

Urho3D::String CaravanMoveTest::CreateResultInfo()
{
    return Urho3D::String ("Caravan position: ") + Urho3D::String (caravan_->GetPosition ()) +
            Urho3D::String ("\nTarget position: ") + Urho3D::String (moveTarget_);
}

CaravanMoveTest::CaravanMoveTest (Urho3D::Context *context) : TestingEngine::Test (context)
{
    name_ = "Caravan move";
    lastCycle_ = 0;
    maxCycles_ = 7200.0f;
}

void CaravanMoveTest::Init ()
{
    // TODO: Test will be rewrited after 2d navigation implementation!
    Urho3D::SetRandomSeed (Urho3D::Time::GetTimeSinceEpoch ());
    map_ = Urho3D::SharedPtr <GameEngine::Map> (new GameEngine::Map (context_));
    Urho3D::ResourceCache *cache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    map_->CreateComponent <GameEngine::TradeGoodsTypesContainer> (
                cache->GetResource <Urho3D::XMLFile> ("XML/trade_goods_types.xml")->GetRoot ("document_root"));

    player_ = Urho3D::SharedPtr <GameEngine::Player> (new GameEngine::Player (context_, "Player"));
    map_->Add (player_);
    caravan_ = player_->CreateCaravan (map_.Get ());
    map_->Init ();

    moveTarget_ = Urho3D::Vector2 (Urho3D::Random (-10.0f, 10.0f), Urho3D::Random (-10.0f, 10.0f));
    Urho3D::VariantMap orderData;
    orderData [GameEngine::CaravanOrders::MoveData::Target] = moveTarget_;
    caravan_->Order (GameEngine::CaravanOrders::Move, orderData);
}

void CaravanMoveTest::Update (float timeStep)
{
    if (lastCycle_ < maxCycles_)
        for (int cycle = 0; cycle < 100; cycle++)
        {
            lastCycle_++;
            map_->Update (1 / 60.0f);
            if ((caravan_->GetPosition () - moveTarget_).Length () < 0.5f)
            {
                TEST_PASSED (CreateResultInfo ());
            }
        }
    else
    {
        TEST_FAILED (CreateResultInfo ());
    }
}

CaravanMoveTest::~CaravanMoveTest ()
{

}


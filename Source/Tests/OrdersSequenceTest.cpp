#include "OrdersSequenceTest.hpp"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLElement.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Engine/Objects/CaravanOrders.hpp>

Urho3D::String OrdersSequenceTest::CreateResultInfo()
{
    return Urho3D::String ("Caravan position: ") + Urho3D::String (caravan_->GetPosition ()) +
            Urho3D::String ("\nIs order1 done = ") + Urho3D::String (order1Done_) +
            Urho3D::String ("\nIs order2 done = ") + Urho3D::String (order2Done_) +
            Urho3D::String ("\nIs order3 done = ") + Urho3D::String (order3Done_);
}

OrdersSequenceTest::OrdersSequenceTest (Urho3D::Context *context) : TestingEngine::Test (context)
{
    name_ = "Orders sequence";
    lastCycle_ = 0;
    maxCycles_ = 14400.0f;
    moveTarget1_ = Urho3D::Vector2 (-5.0f, 0.0f);
    moveTarget2_ = Urho3D::Vector2 (0.0f, 5.0f);
    moveTarget3_ = Urho3D::Vector2 (5.0f, 5.0f);
    order1Done_ = false;
    order2Done_ = false;
    order3Done_ = false;
}

void OrdersSequenceTest::Init ()
{
    Urho3D::SetRandomSeed (Urho3D::Time::GetTimeSinceEpoch ());
    map_ = Urho3D::SharedPtr <GameEngine::Map> (new GameEngine::Map (context_));
    Urho3D::ResourceCache *cache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    map_->CreateComponent <GameEngine::TradeGoodsTypesContainer> (
                cache->GetResource <Urho3D::XMLFile> ("XML/trade_goods_types.xml")->GetRoot ("document_root"));

    player_ = Urho3D::SharedPtr <GameEngine::Player> (new GameEngine::Player (context_, "Player"));
    map_->Add (player_);
    caravan_ = player_->CreateCaravan (map_.Get ());
    map_->Init ();

    Urho3D::VariantMap orderData1;
    orderData1 [GameEngine::CaravanOrders::MoveData::Target] = moveTarget1_;
    caravan_->Order (GameEngine::CaravanOrders::Move, orderData1);

    Urho3D::VariantMap orderData2;
    orderData2 [GameEngine::CaravanOrders::MoveData::Target] = moveTarget2_;
    caravan_->Order (GameEngine::CaravanOrders::Move, orderData2);

    Urho3D::VariantMap orderData3;
    orderData3 [GameEngine::CaravanOrders::MoveData::Target] = moveTarget3_;
    caravan_->Order (GameEngine::CaravanOrders::Move, orderData3);
}

void OrdersSequenceTest::Update (float timeStep)
{
    if (lastCycle_ < maxCycles_)
        for (int cycle = 0; cycle < 100; cycle++)
        {
            lastCycle_++;
            map_->Update (1 / 60.0f);

            if ((caravan_->GetPosition () - moveTarget1_).Length () < 0.5f)
                order1Done_ = true;
            if ((caravan_->GetPosition () - moveTarget2_).Length () < 0.5f)
                order2Done_ = true;
            if ((caravan_->GetPosition () - moveTarget3_).Length () < 0.5f)
                order3Done_ = true;

            if (!order1Done_ && order2Done_)
            {
                TEST_FAILED (CreateResultInfo () + Urho3D::String ("\nOrder 2 done before order 1!"));
            }

            if (!order1Done_ && order3Done_)
            {
                TEST_FAILED (CreateResultInfo () + Urho3D::String ("\nOrder 3 done before order 1!"));
            }

            if (!order2Done_ && order3Done_)
            {
                TEST_FAILED (CreateResultInfo () + Urho3D::String ("\nOrder 3 done before order 2!"));
            }

            if (order1Done_ && order2Done_ && order3Done_)
            {
                TEST_PASSED (CreateResultInfo ());
            }
        }
    else
    {
        TEST_FAILED (CreateResultInfo ());
    }
}

OrdersSequenceTest::~OrdersSequenceTest ()
{

}


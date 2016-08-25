#pragma once
#include <Urho3D/Core/Context.h>
#include <TestingEngine/Test.hpp>
#include <Engine/Objects/Map.hpp>
#include <Engine/Objects/Player.hpp>
#include <Engine/Objects/Caravan.hpp>

class OrdersSequenceTest : public TestingEngine::Test
{
URHO3D_OBJECT (OrdersSequenceTest, TestingEngine::Test)
protected:
    Urho3D::SharedPtr <GameEngine::Map> map_;
    Urho3D::SharedPtr <GameEngine::Player> player_;
    Urho3D::SharedPtr <GameEngine::Caravan> caravan_;
    float lastCycle_;
    float maxCycles_;
    Urho3D::Vector2 moveTarget1_;
    Urho3D::Vector2 moveTarget2_;
    Urho3D::Vector2 moveTarget3_;
    bool order1Done_;
    bool order2Done_;
    bool order3Done_;
    Urho3D::String CreateResultInfo ();
public:
    OrdersSequenceTest (Urho3D::Context *context);
    virtual void Init ();
    virtual void Update (float timeStep);
    virtual ~OrdersSequenceTest ();
};

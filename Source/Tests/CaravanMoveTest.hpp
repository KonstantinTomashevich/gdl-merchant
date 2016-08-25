#pragma once
#include <Urho3D/Core/Context.h>
#include <TestingEngine/Test.hpp>
#include <Engine/Objects/Map.hpp>
#include <Engine/Objects/Player.hpp>
#include <Engine/Objects/Caravan.hpp>

class CaravanMoveTest : public TestingEngine::Test
{
URHO3D_OBJECT (CaravanMoveTest, TestingEngine::Test)
protected:
    Urho3D::SharedPtr <GameEngine::Map> map_;
    Urho3D::SharedPtr <GameEngine::Player> player_;
    Urho3D::SharedPtr <GameEngine::Caravan> caravan_;
    float lastCycle_;
    float maxCycles_;
    Urho3D::Vector2 moveTarget_;
    Urho3D::String CreateResultInfo ();
public:
    CaravanMoveTest (Urho3D::Context *context);
    virtual void Init ();
    virtual void Update (float timeStep);
    virtual ~CaravanMoveTest ();
};

#pragma once
#include <Urho3D/Core/Context.h>
#include <TestingEngine/Test.hpp>
#include <Engine/Objects/Map.hpp>
#include <Engine/Objects/Village.hpp>
#include <Engine/Objects/Caravan.hpp>

class CollisionTest : public TestingEngine::Test
{
URHO3D_OBJECT (CollisionTest, TestingEngine::Test)
protected:
    Urho3D::SharedPtr <GameEngine::Map> map_;
    Urho3D::SharedPtr <GameEngine::Village> village_;
    Urho3D::SharedPtr <GameEngine::Caravan> caravan1_;
    Urho3D::SharedPtr <GameEngine::Caravan> caravan2_;
    Urho3D::SharedPtr <GameEngine::Caravan> caravan3_;
public:
    CollisionTest (Urho3D::Context *context);
    virtual void Init ();
    virtual void Update (float timeStep);
    virtual ~CollisionTest ();
};

#pragma once
#include <Urho3D/Core/Context.h>
#include <TestingEngine/Test.hpp>
#include <Engine/Objects/Map.hpp>
#include <Engine/Objects/Village.hpp>

class VillageProductionAndPopulationTest : public TestingEngine::Test
{
URHO3D_OBJECT (VillageProductionAndPopulationTest, TestingEngine::Test)
protected:
    Urho3D::SharedPtr <GameEngine::Map> map_;
    Urho3D::SharedPtr <GameEngine::Village> village_;
    float lastCycle_;
    float maxCycles_;
    int populationAtStart_;
    float productionAtStart_;
    Urho3D::String CreateResultInfo ();
public:
    VillageProductionAndPopulationTest (Urho3D::Context *context);
    virtual void Init ();
    virtual void Update (float timeStep);
    virtual ~VillageProductionAndPopulationTest ();
};

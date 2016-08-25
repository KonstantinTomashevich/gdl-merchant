#include "Urho3DApplication.hpp"
#include <Engine/Engine.hpp>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Scene/Node.h>
#include <StatesEngine/StatesEngine.hpp>
#include <StatesEngine/StateObjectsManager.hpp>

#include "VillageProductionAndPopulationTest.hpp"
#include "CollisionTest.hpp"
#include "CaravanMoveTest.hpp"
#include "OrdersSequenceTest.hpp"

Urho3DApplication::Urho3DApplication (Urho3D::Context *context) : Urho3D::Application (context), testsRunner_ (new TestingEngine::TestsRunner (context))
{

}

void Urho3DApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
}

void Urho3DApplication::Start ()
{
    StatesEngine::StatesEngine *statesEngine = new StatesEngine::StatesEngine (context_);
    statesEngine->Init ();
    context_->RegisterSubsystem (statesEngine);
    statesEngine->SetupState (Urho3D::SharedPtr <StatesEngine::StateObjectsManager> (new StatesEngine::StateObjectsManager (context_)));
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->SetIsUseLog (true);

    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseMode (Urho3D::MM_FREE);
    input->SetMouseVisible (true);
    context_->RegisterSubsystem (this);

    GameEngine::RegisterType <GameEngine::TradeGoodsTypesContainer> (context_);
    GameEngine::RegisterType <GameEngine::Village> (context_);
    GameEngine::RegisterType <GameEngine::Caravan> (context_);
    GameEngine::RegisterType <GameEngine::Player> (context_);
    GameEngine::RegisterType <GameEngine::Map> (context_);

    context_->GetSubsystem <Urho3D::Log> ()->SetLevel (Urho3D::LOG_DEBUG);
    testsRunner_->AddTest (Urho3D::SharedPtr <TestingEngine::Test> (new VillageProductionAndPopulationTest (context_)));
    testsRunner_->AddTest (Urho3D::SharedPtr <TestingEngine::Test> (new CollisionTest (context_)));
    testsRunner_->AddTest (Urho3D::SharedPtr <TestingEngine::Test> (new CaravanMoveTest (context_)));
    testsRunner_->AddTest (Urho3D::SharedPtr <TestingEngine::Test> (new OrdersSequenceTest (context_)));
    testsRunner_->Subscirbe ();
    testsRunner_->ShowUi ();
}

void Urho3DApplication::Stop ()
{

}

Urho3DApplication::~Urho3DApplication ()
{

}
URHO3D_DEFINE_APPLICATION_MAIN (Urho3DApplication)

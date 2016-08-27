#include "TestXMLSerialization.hpp"
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
#include <Engine/Objects/CaravanOrders.hpp>

TestXMLSerialization::TestXMLSerialization (Urho3D::Context *context) : Urho3D::Application(context)
{

}

void TestXMLSerialization::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
}

void TestXMLSerialization::Start ()
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

    const int caravansCount = 4;
    {
        Urho3D::SharedPtr <GameEngine::Village> village1 (new GameEngine::Village (context_));
        map->Add (village1);
        log->Write (Urho3D::LOG_INFO, "Village1 added...");

        village1->GetStorage ()->SetSlotsCount (24);
        village1->SetPopulation (1000);
        village1->SetProduction ("Food", 2.0f);
        village1->GetBounds ()->position_ = Urho3D::Vector2 (1.0f, 1.0f);
        village1->GetBounds ()->radius_ = 2.0f;
        village1->SetName ("Village1");
        log->Write (Urho3D::LOG_INFO, "Village1 configured...");

        Urho3D::SharedPtr <GameEngine::Village> village2 (new GameEngine::Village (context_));
        map->Add (village2);
        log->Write (Urho3D::LOG_INFO, "Village2 added...");

        village2->GetStorage ()->SetSlotsCount (24);
        village2->SetPopulation (2000);
        village2->SetProduction ("Food", 5.0f);
        village2->GetBounds ()->position_ = Urho3D::Vector2 (15.0f, -10.0f);
        village2->GetBounds ()->radius_ = 3.5f;
        village2->SetName ("Village2");
        log->Write (Urho3D::LOG_INFO, "Village2 configured...");

        Urho3D::SharedPtr <GameEngine::Player> player1 (new GameEngine::Player (context_, Urho3D::String ("player1"), 567.0f,
                                                                                Urho3D::Color (0.5f, 0.5f, 0.5f, 1.0f)));
        map->Add (player1);
        log->Write (Urho3D::LOG_INFO, "Player1 configured...");

        Urho3D::SharedPtr <GameEngine::Player> player2 (new GameEngine::Player (context_, Urho3D::String ("player2"), 786.0f,
                                                                                Urho3D::Color (0.2f, 0.8f, 0.2f, 1.0f)));
        map->Add (player2);
        log->Write (Urho3D::LOG_INFO, "Player2 configured...");

        Urho3D::Vector <Urho3D::SharedPtr <GameEngine::Caravan> > caravans;
        for (int index = 0; index < caravansCount; index++)
            caravans.Push ((Urho3D::Random () > 0.5f ? player2 : player1)->CreateCaravan (map.Get (), Urho3D::Random (5, 15)));
        log->Write (Urho3D::LOG_INFO, "Caravans added...");

        for (int index = 0; index < caravansCount; index++)
        {
            caravans.At (index)->GetBounds ()->position_ = Urho3D::Vector2 (Urho3D::Random (-15.0f, 15.0f), Urho3D::Random (-15.0f, 15.0f));
            caravans.At (index)->GetBounds ()->radius_ = Urho3D::Random ();
        }
        log->Write (Urho3D::LOG_INFO, "Caravans configured...");
        map->Init ();

        for (int index = 0; index < caravansCount; index++)
        {
            for (int orderIndex = 0; orderIndex < 5; orderIndex++)
            {
                Urho3D::VariantMap data;
                data [GameEngine::CaravanOrders::MoveData::Target] =
                        Urho3D::Vector2 (Urho3D::Random (-15.0f, 15.0f), Urho3D::Random (-15.0f, 15.0f));
                caravans.At (index)->Order (GameEngine::CaravanOrders::Move, data);
            }
        }
        log->Write (Urho3D::LOG_INFO, "Caravans orders sended...");
    }
    log->SetLevel (Urho3D::LOG_DEBUG);
    map->SetIsUseLog (true);
    for (int cycle = 0; cycle < 7200; cycle++)
        map->Update (1 / 60.0f);

    Urho3D::XMLFile *file = new Urho3D::XMLFile (context_);
    Urho3D::XMLElement root = file->CreateRoot ("root");
    log->Write (Urho3D::LOG_INFO, "Saving to xml...");
    map->SaveToXML (root);
    log->Write (Urho3D::LOG_INFO, "Saved to xml:\n" + file->ToString ());

    map->RemoveAll ("any");
    map->RemoveAllComponents ("any");
    if (map->CountOf ("any") > 0)
        ErrorExit ("RemoveAll don't clears map!");
    if (map->CountOfComponents ("any") > 0)
        ErrorExit ("RemoveAllComponents don't clears map!");

    log->Write (Urho3D::LOG_INFO, "Loading from xml...");
    map->LoadFromXML (root.GetChild ("map"));
    log->Write (Urho3D::LOG_INFO, "Loaded from xml...");
    delete file;

    for (int cycle = 0; cycle < 7200; cycle++)
        map->Update (1 / 60.0f);
    if (map->CountOf ("Village") != 2)
        ErrorExit ("Count of villages before saving != after loading");
    if (map->CountOf ("Player") != 2)
        ErrorExit ("Count of players before saving != after loading");
    if (map->CountOf ("Caravan") != caravansCount)
        ErrorExit ("Count of caravans before saving != after loading");
    engine_->Exit ();
}

void TestXMLSerialization::Stop ()
{
    StatesEngine::StatesEngine *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngine> ();
    statesEngine->DisposeState ();
    context_->RemoveSubsystem ("StatesEngine");
}

TestXMLSerialization::~TestXMLSerialization ()
{

}
URHO3D_DEFINE_APPLICATION_MAIN (TestXMLSerialization)

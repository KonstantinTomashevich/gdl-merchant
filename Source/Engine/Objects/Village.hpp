#pragma once
#include <Engine/Utils/Trader.hpp>
#include "Caravan.hpp"
#include "MapObject.hpp"

namespace GameEngine
{
class Village : public MapObject, public Trader
{
URHO3D_OBJECT (Village, MapObject)
protected:
    int population_;
    Urho3D::Vector2 populationIncrease_;
    float populationIncreaseTimestep_;
    float timeFromLastPopulationIncrease_;

    float produceTimestep_;
    float timeFromLastProduce_;
    Urho3D::String name_;
    Urho3D::HashMap <Urho3D::StringHash, float> production_;
    Urho3D::Vector <Caravan *> caravansIn_;

    virtual TradeGoodsTypesContainer *GetTradeGoodsTypesContainer ();
    virtual void SetPlayersGold (float gold);
    virtual void ProcessCollide (MapObject *other);
    virtual void Product ();
    virtual void IncreasePopulation ();
public:
    Village (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep);
    virtual bool Dispose ();

    virtual bool LoadFromXML (Urho3D::XMLElement rootElement);
    virtual bool ProcessEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    Urho3D::String GetName ();
    void SetName (Urho3D::String name);
    float GetProductionOf (Urho3D::StringHash type);
    Urho3D::Vector <Urho3D::StringHash> GetProductionTypes ();
    void SetProduction (Urho3D::StringHash type, float countPerSecond, bool forceWithoutDebug = false);

    int GetCountOfCaravansIn ();
    Caravan *GetCaravan (int index);
    bool IsCaravanIn (Caravan *caravan);
    float GetProduceTimestep ();
    void SetProduceTimestep (float produceTimestep);
    float GetTimeBeforeNextProduce ();

    int GetPopulation ();
    void SetPopulation (int population);
    Urho3D::Vector2 GetPopulationIncrease ();
    void SetPopulationIncrease (Urho3D::Vector2 populationIncrease);
    float GetPopulationIncreaseTimestep ();
    void SetPopulationIncreaseTimestep (float populationIncreaseTimestep);
    float GetTimeBeforeNextPopulationIncrease ();
    virtual ~Village ();
};
}

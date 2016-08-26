#include "Village.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include <Engine/Engine.hpp>
#include <StatesEngine/StatesEngine.hpp>

namespace GameEngine
{
TradeGoodsTypesContainer *Village::GetTradeGoodsTypesContainer ()
{
    return ( (Map *) parent_)->GetComponent <TradeGoodsTypesContainer> ().Get ();
}

void Village::SetPlayersGold (float gold)
{
    assert (false); // Will be never called, because village is neutral (can not be owned by players)!
}

void Village::ProcessCollide (MapObject *other)
{
    if (other->GetTypeInfo ()->IsTypeOf <Caravan> ())
        caravansIn_.Push ( (Caravan *) other);
}

void Village::Product ()
{
    timeFromLastProduce_ = 0.0f;
    for (int index = 0; index < production_.Keys ().Size (); index++)
    {
        if (production_.Values ().At (index) > 0.0f)
        {
            TradeGoodsType *type = &(GetTradeGoodsTypesContainer ()->GetByHash (production_.Keys ().At (index)));
            float willBeProducted = produceTimestep_ * production_.Values ().At (index);
            float canBeProducted = 0.0f;

            if (type->WhatRequiredForProduction ().Empty ())
                canBeProducted = willBeProducted + 1.0f;
            else
            {
                canBeProducted = std::numeric_limits <float> ().max ();
                for (int reqIndex = 0; reqIndex < type->WhatRequiredForProduction ().Size (); reqIndex++)
                {
                    float canBeProductedFromThis = storage_.GetAmountOf (type->WhatRequiredForProduction ().Keys ().At (reqIndex)) /
                            type->WhatRequiredForProduction ().Values ().At (reqIndex);

                    if (canBeProductedFromThis < canBeProducted)
                        canBeProducted = canBeProductedFromThis;
                }
            }

            if  (willBeProducted > canBeProducted)
                willBeProducted = canBeProducted;
            if (!type->WhatRequiredForProduction ().Empty ())
                for (int reqIndex = 0; reqIndex < type->WhatRequiredForProduction ().Size (); reqIndex++)
                    storage_.Throw (&GetTradeGoodsTypesContainer ()->GetByHash (type->WhatRequiredForProduction ().Keys ().At (reqIndex)),
                                    willBeProducted * type->WhatRequiredForProduction ().Values ().At (reqIndex));
            storage_.Store (type, willBeProducted);
        }
    }
}

void Village::IncreasePopulation ()
{
    timeFromLastPopulationIncrease_ = 0.0f;
    population_ *= (1.0f + Urho3D::Random (populationIncrease_.x_, populationIncrease_.y_));
    for (int index = 0; index < production_.Size (); index++)
        production_ [production_.Keys ().At (index)] =
                production_ [production_.Keys ().At (index)] * (1.0f + Urho3D::Random (populationIncrease_.x_, populationIncrease_.y_));
}

Village::Village (Urho3D::Context *context) : MapObject (context), Trader (), production_ ()
{
    produceTimestep_ = 10.0f;
    populationIncreaseTimestep_ = 60.0f;
    populationIncrease_ = Urho3D::Vector2 (0.01f, 0.05f);
    timeFromLastProduce_ = 0.0f;
    timeFromLastPopulationIncrease_ = 0.0f;
}

bool Village::Init ()
{
    if (parent_->GetTypeName () == "Map")
    {
        ready_ = true;
        return true;
    }
    else
        return false;
}

bool Village::Update (float timeStep)
{
    caravansIn_.Clear ();
    ProcessDeferredEvents ();
    timeFromLastProduce_ += timeStep;
    if (timeFromLastProduce_ > produceTimestep_)
        Product ();

    timeFromLastPopulationIncrease_ += timeStep;
    if (timeFromLastPopulationIncrease_ > populationIncreaseTimestep_)
        IncreasePopulation ();
    //TODO: Send caravan to marketplace if count of empty storage slots < count of production types.
    return true;
}

bool Village::Dispose ()
{
    if (ready_)
    {
        ready_ = false;
        caravansIn_.Clear ();
        storage_.ClearStorage ();
        production_.Clear ();
        return true;
    }
    else
        return false;
}

bool Village::LoadFromXML (Urho3D::XMLElement rootElement)
{
    if (!MapObject::LoadFromXML (rootElement))
        return false;
    //TODO: Implement this.
    return true;
}

Urho3D::XMLElement Village::SaveToXML(Urho3D::XMLElement &parentElement)
{
    //TODO: Implement this.
}

bool Village::ProcessEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (eventType == Events::ObjectsCollide)
    {
        if ( ( (MapObject *) eventData [Events::ObjectsCollideData::ObjectA].GetPtr ()) == ( (MapObject *) this))
            ProcessCollide ((MapObject *) eventData [Events::ObjectsCollideData::ObjectB].GetPtr ());
        else
            ProcessCollide ((MapObject *) eventData [Events::ObjectsCollideData::ObjectA].GetPtr ());
        return true;
    }
    return false;
}

Urho3D::String Village::GetName ()
{
    return name_;
}

void Village::SetName (Urho3D::String name)
{
    name_ = name;
}

float Village::GetProductionOf (Urho3D::StringHash type)
{
    return production_ [type];
}

Urho3D::Vector<Urho3D::StringHash> Village::GetProductionTypes ()
{
    return production_.Keys ();
}

void Village::SetProduction (Urho3D::StringHash type, float countPerSecond, bool forceWithoutDebug)
{
    if (!forceWithoutDebug)
    {
#ifndef NDEBUG
    // For validating type because if container can't find this type, program will be stoped with "Assertion failed".
    GetTradeGoodsTypesContainer ()->GetByHash (type);
#endif
    assert (countPerSecond >= 0.0f);
    }
    production_ [type] = countPerSecond;
}

int Village::GetCountOfCaravansIn ()
{
    return caravansIn_.Size ();
}

Caravan *Village::GetCaravan(int index)
{
    return caravansIn_.At (index);
}

bool Village::IsCaravanIn(Caravan *caravan)
{
    return caravansIn_.Contains (caravan);
}

float Village::GetProduceTimestep ()
{
    return produceTimestep_;
}

void Village::SetProduceTimestep (float produceTimestep)
{
    produceTimestep_ = produceTimestep;
}

float Village::GetTimeBeforeNextProduce ()
{
    return (produceTimestep_ - timeFromLastProduce_);
}

int Village::GetPopulation ()
{
    return population_;
}

void Village::SetPopulation (int population)
{
    population_ = population;
}

Urho3D::Vector2 Village::GetPopulationIncrease ()
{
    return populationIncrease_;
}

void Village::SetPopulationIncrease (Urho3D::Vector2 populationIncrease)
{
    populationIncrease_ = populationIncrease;
}

float Village::GetPopulationIncreaseTimestep ()
{
    return populationIncreaseTimestep_;
}

void Village::SetPopulationIncreaseTimestep (float populationIncreaseTimestep)
{
    populationIncreaseTimestep_ = populationIncreaseTimestep;
}

float Village::GetTimeBeforeNextPopulationIncrease ()
{
    return (populationIncreaseTimestep_ - timeFromLastPopulationIncrease_);
}

Village::~Village ()
{

}
}


#include "Caravan.hpp"
#include "Village.hpp"
#include "Events.hpp"
#include "CaravanOrders.hpp"
#include <Engine/Engine.hpp>

namespace GameEngine
{
void Caravan::MoveToPoint (float timeStep, Urho3D::Vector2 point)
{
    Urho3D::Vector2 distance = point - GetPosition ();
    float speed = maxSpeed_ - fullStorageSpeedDecrease_ *
            ( (float) (storage_.GetStorageSlotsCount () - storage_.GetStorageEmptySlotsCount ()) / (float) storage_.GetStorageSlotsCount ());
    Urho3D::Vector2 move = distance;
    move *= (speed * timeStep / distance.Length ());
    SetPosition (GetPosition () + move);
}

bool Caravan::ProcessFirstOrder (float timeStep)
{
    assert (!orders_.Empty ());
    if (orders_.Front ().first_ == CaravanOrders::Move)
        return ProcessAsMoveOrder (timeStep);
    else if (orders_.Front ().first_ == CaravanOrders::Follow)
        return ProcessAsFollowOrder (timeStep);
    else if (orders_.Front ().first_ == CaravanOrders::Attack)
        return ProcessAsAttackOrder (timeStep);
    else if (orders_.Front ().first_ == CaravanOrders::StoreTo)
        return ProcessAsStoreToOrder (timeStep);
    else if (orders_.Front ().first_ == CaravanOrders::ExchangeFrom)
        return ProcessAsExchangeFromOrder (timeStep);
    else if (orders_.Front ().first_ == CaravanOrders::Buy)
        return ProcessAsBuyOrder (timeStep);
    else if (orders_.Front ().first_ == CaravanOrders::Sell)
        return ProcessAsSellOrder (timeStep);
    else if (orders_.Front ().first_ == CaravanOrders::CreateMarketplaceTrader)
        return ProcessAsCreateMarketplaceTraderOrder (timeStep);
    else
    {
        // Unknown order! Situation will be reported!
        assert (0);
        return false;
    }
}

bool Caravan::ProcessAsMoveOrder (float timeStep)
{
    //TODO: Reimplement after 2d navigation implementation!
    MoveToPoint (timeStep, orders_.Front ().second_ [CaravanOrders::MoveData::Target].GetVector2 ());
    return ((GetPosition () - orders_.Front ().second_ [CaravanOrders::MoveData::Target].GetVector2 ()).Length () < 0.25f);
}

bool Caravan::ProcessAsFollowOrder (float timeStep)
{
    //TODO: Implement this.
}

bool Caravan::ProcessAsAttackOrder (float timeStep)
{
    //TODO: Implement this.
}

bool Caravan::ProcessAsStoreToOrder (float timeStep)
{
    //TODO: Implement this.
}

bool Caravan::ProcessAsExchangeFromOrder (float timeStep)
{
    //TODO: Implement this.
}

bool Caravan::ProcessAsBuyOrder (float timeStep)
{
    //TODO: Implement this.
}

bool Caravan::ProcessAsSellOrder (float timeStep)
{
    //TODO: Implement this.
}

bool Caravan::ProcessAsCreateMarketplaceTraderOrder (float timeStep)
{
    //TODO: Implement this.
}

TradeGoodsTypesContainer *Caravan::GetTradeGoodsTypesContainer ()
{
    return ( (Map *) parent_)->GetComponent <TradeGoodsTypesContainer> ().Get ();
}

void Caravan::SetPlayersGold (float gold)
{
    playerData_->SetGold (gold);
}

Caravan::Caravan (Urho3D::Context *context) : MapObject (context), orders_ ()
{
    destination_ = 0;
    maxSpeed_ = 3.0f;
    fullStorageSpeedDecrease_ = 0.75f;
}

bool Caravan::Init ()
{
    if (parent_->GetTypeName () == "Map")
    {
        ready_ = true;
        return true;
    }
    else
        return false;
}

bool Caravan::Update (float timeStep)
{
    destination_ = 0;
    ProcessDeferredEvents ();
    if (!orders_.Empty ())
        if (ProcessFirstOrder (timeStep))
            orders_.Remove (orders_.At (0));
    return true;
}

bool Caravan::Dispose ()
{
    if (ready_)
    {
        ready_ = false;
        destination_ = 0;
        orders_.Clear ();
        deferredEvents_.Clear ();
        return true;
    }
    else
        return false;
}

bool Caravan::LoadFromXML (Urho3D::XMLElement rootElement)
{
    if (!MapObject::LoadFromXML (rootElement))
        return false;
    //TODO: Implement this.
    return true;
}

Urho3D::XMLElement Caravan::SaveToXML (Urho3D::XMLElement &parentElement)
{
    //TODO: Implement this.
}

bool Caravan::ProcessEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (eventType == Events::ObjectsCollide)
    {
        if ( ( (MapObject *) eventData [Events::ObjectsCollideData::ObjectA].GetPtr ()) == ( (MapObject *) this))
        {
            if ( ( (MapObject *) eventData [Events::ObjectsCollideData::ObjectB].GetPtr ())->GetTypeInfo ()->IsTypeOf <Village> ())
            {
                destination_ = ( (Village *) eventData [Events::ObjectsCollideData::ObjectB].GetPtr ());
                return true;
            }
            else
                return false;
        }
        else if ( ( (MapObject *) eventData [Events::ObjectsCollideData::ObjectA].GetPtr ())->GetTypeInfo ()->IsTypeOf <Village> ())
        {
            destination_ = ( (Village *) eventData [Events::ObjectsCollideData::ObjectA].GetPtr ());
        }
        else
                return false;
    }

    if (eventType == Events::Order)
    {
        if (eventData [Events::OrderData::IsDeletePrevious].GetBool ())
            ClearOrders ();
        Order (eventData [Events::OrderData::Type].GetStringHash (), eventData);
        return true;
    }
    return false;
}

void Caravan::ClearOrders ()
{
    orders_.Clear ();
}

void Caravan::Order (Urho3D::StringHash orderType, Urho3D::VariantMap data)
{
    orders_.Push (Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> (orderType, data));
}

int Caravan::GetCountOfOrders ()
{
    return orders_.Size ();
}

Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> &Caravan::GetOrder (int index)
{
    assert (index < GetCountOfOrders ());
    return orders_.At (index);
}

Village *Caravan::GetDistination()
{
    return destination_;
}

Player *Caravan::GetPlayer ()
{
    return player_;
}

void Caravan::SetPlayer (Player *player, PlayerProtectedData *playerData)
{
    assert (player);
    assert (playerData);
    player_ = player;
    playerData_ = playerData;
}

Caravan::~Caravan ()
{

}
}


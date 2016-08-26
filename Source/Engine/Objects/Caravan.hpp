#pragma once
#include "MapObject.hpp"
#include <Engine/Utils/Trader.hpp>
#include "Map.hpp"
#include "Player.hpp"

namespace GameEngine
{
class Village;
class Caravan : public MapObject, public Trader
{
URHO3D_OBJECT (Caravan, MapObject)
protected:
    Player *player_;
    PlayerProtectedData *playerData_;
    Urho3D::Vector <Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> > orders_;
    Village *destination_;
    float maxSpeed_;
    float fullStorageSpeedDecrease_;
    void MoveToPoint (float timeStep, Urho3D::Vector2 point);

    // This functions will return true if order completed and will be deleted now!
    virtual bool ProcessFirstOrder (float timeStep);
    virtual bool ProcessAsMoveOrder (float timeStep);
    virtual bool ProcessAsFollowOrder (float timeStep);
    virtual bool ProcessAsAttackOrder (float timeStep);
    virtual bool ProcessAsStoreToOrder (float timeStep);
    virtual bool ProcessAsExchangeFromOrder (float timeStep);
    virtual bool ProcessAsBuyOrder (float timeStep);
    virtual bool ProcessAsSellOrder (float timeStep);
    virtual bool ProcessAsCreateMarketplaceTraderOrder (float timeStep);

    virtual TradeGoodsTypesContainer *GetTradeGoodsTypesContainer ();
    virtual void SetPlayersGold (float gold);
public:
    Caravan (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep);
    virtual bool Dispose ();

    virtual bool LoadFromXML (Urho3D::XMLElement rootElement);
    virtual bool SaveToXML (Urho3D::XMLElement &parentElement);
    virtual bool ProcessEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void ClearOrders ();
    void Order (Urho3D::StringHash orderType, Urho3D::VariantMap data);
    int GetCountOfOrders();
    Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> &GetOrder (int index);
    Village *GetDistination ();
    Player *GetPlayer ();
    void SetPlayer (Player *player, PlayerProtectedData *playerData);
    virtual ~Caravan ();
};
}

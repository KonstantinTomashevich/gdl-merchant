#pragma once
#include <Engine/Objects/Player.hpp>
#include <Urho3D/Math/StringHash.h>
#include "Storage.hpp"
#include "Ownable.hpp"
#include <Engine/Objects/TradeGoodsTypesContainer.hpp>

namespace GameEngine
{
class Trader : public Ownable
{
protected:
    Storage storage_;
    float buyingCostModifer_;
    float sellingCostModifer_;
    virtual TradeGoodsTypesContainer *GetTradeGoodsTypesContainer () = 0;
    virtual void SetPlayersGold (float gold) = 0;
public:
    Trader (Player* player = 0);
    Storage *GetStorage ();
    float GetBuyingCostModifer ();
    float GetSellingCostModifer ();
    void SetBuyingCostModifer (float buyingCostModifer);
    void SetSellingCostModifer (float sellingCostModifer);

    bool RequestSell (Urho3D::StringHash type, float amount, float reward, Trader *buyer);
    bool IsCanSell (Urho3D::StringHash type, float amount);
    bool IsCanBuy (Urho3D::StringHash type, float amount, float cost);
    bool Buy (Urho3D::StringHash type, float amount, float cost);
    float GetBasicCost (bool isSell, Urho3D::StringHash type, float amount);
    virtual ~Trader ();
};
}

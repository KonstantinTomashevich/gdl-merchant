#include "Trader.hpp"
#include <Engine/Engine.hpp>

namespace GameEngine
{
Trader::Trader (Player *player) : Ownable (player)
{
    buyingCostModifer_ = 1.0f;
    sellingCostModifer_ = 1.25f;
}

Storage *Trader::GetStorage ()
{
    return &storage_;
}

float Trader::GetBuyingCostModifer ()
{
    return buyingCostModifer_;
}

float Trader::GetSellingCostModifer ()
{
    return sellingCostModifer_;
}

void Trader::SetBuyingCostModifer (float buyingCostModifer)
{
    buyingCostModifer_ = buyingCostModifer;
}

void Trader::SetSellingCostModifer (float sellingCostModifer)
{
    sellingCostModifer_ = sellingCostModifer;
}

bool Trader::RequestSell (Urho3D::StringHash type, float amount, float reward, Trader *buyer)
{
    assert (amount > 0);
    assert (reward > 0);
    assert (buyer);

    if (!IsCanSell (type, amount))
        return false;
    if (player_)
        SetPlayersGold (player_->GetGold () + reward);

    bool throwResult = storage_.Throw (&GetTradeGoodsTypesContainer ()->GetByHash (type), amount);
    assert (throwResult);
    return buyer->Buy (type, amount, reward);
}

bool Trader::IsCanSell (Urho3D::StringHash type, float amount)
{
    return storage_.IsCanThrow (&GetTradeGoodsTypesContainer ()->GetByHash (type), amount);
}

bool Trader::IsCanBuy (Urho3D::StringHash type, float amount, float cost)
{
    if (!storage_.IsCanStore (&GetTradeGoodsTypesContainer ()->GetByHash (type), amount))
        return false;

    if (!player_)
        return true;

    if (player_->GetGold () < cost)
        return false;
    else
        return true;
}

bool Trader::Buy (Urho3D::StringHash type, float amount, float cost)
{
    bool isCanBuy = IsCanBuy (type, amount, cost);
    assert (isCanBuy);
    if (!isCanBuy)
        return false;

    bool storeResult = storage_.Store (&GetTradeGoodsTypesContainer ()->GetByHash (type), amount);
    assert (storeResult);
    if (player_)
        SetPlayersGold (player_->GetGold () - cost);
}

float Trader::GetBasicCost (bool isSell, Urho3D::StringHash type, float amount)
{
    return GetTradeGoodsTypesContainer ()->GetByHash (type).GetStandartCost () * amount *
            (isSell ? sellingCostModifer_ : buyingCostModifer_);
}

Trader::~Trader()
{

}
}

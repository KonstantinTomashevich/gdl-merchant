#include "TradeGoodsType.hpp"
namespace GameEngine
{
// Only for Urho3D::Vector.
TradeGoodsType::TradeGoodsType () : name_ ("NoName"), storagingCost_ (1.0f), standartCost_ (10.0f), productionCost_ (7.5f),
    icon_ (Urho3D::String::EMPTY), requiredForProduction_ ()
{

}

TradeGoodsType::TradeGoodsType(Urho3D::String name, float storagingCost, float standartCost,
                               float productionCost, Urho3D::String icon, Urho3D::HashMap<Urho3D::StringHash, float> &requiredForProduction) :
    name_ (name), storagingCost_ (storagingCost), standartCost_ (standartCost),
    productionCost_ (productionCost_), icon_ (icon), requiredForProduction_ (requiredForProduction)
{

}

Urho3D::String TradeGoodsType::GetName () const
{
    return name_;
}

float TradeGoodsType::GetStoragingCost () const
{
    return storagingCost_;
}

float TradeGoodsType::GetStandartCost () const
{
    return standartCost_;
}

float TradeGoodsType::GetProductionCost () const
{
    return productionCost_;
}

Urho3D::String TradeGoodsType::GetIcon () const
{
    return icon_;
}

Urho3D::HashMap <Urho3D::StringHash, float> TradeGoodsType::WhatRequiredForProduction () const
{
    return requiredForProduction_;
}

TradeGoodsType::~TradeGoodsType ()
{

}
}


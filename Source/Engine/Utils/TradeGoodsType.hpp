#pragma once
#include <Urho3D/Container/Str.h>
#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Container/HashMap.h>

namespace GameEngine
{
class TradeGoodsType
{
protected:
    Urho3D::String name_;
    float storagingCost_;
    float standartCost_;
    float productionCost_;
    Urho3D::String icon_;
    Urho3D::HashMap <Urho3D::StringHash, float> requiredForProduction_;
public:
    TradeGoodsType ();
    TradeGoodsType (Urho3D::String name, float storagingCost, float standartCost,
                    float productionCost, Urho3D::String icon, Urho3D::HashMap <Urho3D::StringHash, float> &requiredForProduction);

    Urho3D::String GetName () const;
    float GetStoragingCost () const;
    float GetStandartCost () const;
    float GetProductionCost () const;
    Urho3D::String GetIcon () const;
    Urho3D::HashMap<Urho3D::StringHash, float> WhatRequiredForProduction() const;
    virtual ~TradeGoodsType ();
};

inline bool operator == (const TradeGoodsType &lhs, const TradeGoodsType &rhs)
{
    return (lhs.GetName () == rhs.GetName () && lhs.GetStoragingCost ()== rhs.GetStandartCost () &&
            lhs.GetStandartCost () == rhs.GetStandartCost () && lhs.GetProductionCost () == rhs.GetProductionCost () &&
            lhs.WhatRequiredForProduction () == rhs.WhatRequiredForProduction ());
}

inline bool operator != (const TradeGoodsType &lhs, const TradeGoodsType &rhs)
{
    return !(lhs == rhs);
}
}

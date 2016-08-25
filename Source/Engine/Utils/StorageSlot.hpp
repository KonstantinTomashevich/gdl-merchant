#pragma once
#include <Urho3D/Math/StringHash.h>
#include "TradeGoodsType.hpp"

namespace GameEngine
{
static const float STORAGE_SLOT_MAX_SIZE = 128.0f;
static const Urho3D::StringHash STORAGE_SLOT_EMPTY ("empty");

class StorageSlot
{
protected:
    Urho3D::StringHash typeId_;
    float amount_;
public:
    StorageSlot ();
    StorageSlot (TradeGoodsType *type, float amount);
    Urho3D::StringHash GetTypeId ();
    float GetAmount ();
    void Set (TradeGoodsType *type, float amount);
    void SetEmpty ();
};
}

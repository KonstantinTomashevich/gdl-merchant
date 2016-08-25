#include "StorageSlot.hpp"
#include <Engine/Engine.hpp>

namespace GameEngine
{
StorageSlot::StorageSlot ()
{
    SetEmpty ();
}

StorageSlot::StorageSlot (TradeGoodsType *type, float amount)
{
    Set (type, amount);
}

Urho3D::StringHash StorageSlot::GetTypeId ()
{
    return typeId_;
}

float StorageSlot::GetAmount ()
{
    return amount_;
}

void StorageSlot::Set (TradeGoodsType *type, float amount)
{
    assert (Urho3D::StringHash (type->GetName ()) != STORAGE_SLOT_EMPTY);
    assert (amount * type->GetStoragingCost () <= STORAGE_SLOT_MAX_SIZE);
    if (amount * type->GetStoragingCost () <= STORAGE_SLOT_MAX_SIZE)
    {
        typeId_ = Urho3D::StringHash (type->GetName ());
        amount_ = amount;
    }
}

void StorageSlot::SetEmpty()
{
    typeId_ = STORAGE_SLOT_EMPTY;
    amount_ = 0.0f;
}
}


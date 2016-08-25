#include "Storage.hpp"
#include <Engine/Engine.hpp>

namespace GameEngine
{
Storage::Storage () : storageSlots_ ()
{

}

void Storage::ClearStorage()
{
    for (int index = 0; index < GetStorageSlotsCount (); index++)
        GetStorageSlot (index).SetEmpty ();
}

int Storage::GetStorageSlotsCount ()
{
    return storageSlots_.Size ();
}

void Storage::SetStorageSlotsCount (int count)
{
    assert (count > 0);
    while (count > storageSlots_.Size ())
        storageSlots_.Push (StorageSlot ());
    while (storageSlots_.Size () < count)
        storageSlots_.Pop ();
}

bool Storage::IsCanStore (TradeGoodsType *type, float amount)
{
    assert (amount > 0);
    float slotCapacity = STORAGE_SLOT_MAX_SIZE / type->GetStoragingCost ();
    float canStore = 0.0f;
    canStore += GetStorageEmptySlotsCount () * slotCapacity;
    for (int index = 0; index < GetStorageSlotsCount (); index++)
        if (GetStorageSlot (index).GetTypeId () == Urho3D::StringHash (type->GetName ()))
            canStore += (slotCapacity - GetStorageSlot (index).GetAmount ());
    return canStore >= amount;
}

bool Storage::Store (TradeGoodsType *type, float amount)
{
    assert (amount > 0);
    if (!IsCanStore (type, amount))
        return false;
    float slotCapacity = STORAGE_SLOT_MAX_SIZE / type->GetStoragingCost ();
    float willBeStored = amount;
    int slotIndex = 0;

    while (willBeStored > 0 && slotIndex < GetStorageSlotsCount ())
    {
        StorageSlot *slot = &GetStorageSlot (slotIndex);
        if (slot->GetTypeId () == Urho3D::StringHash (type->GetName ()) || slot->GetTypeId () == STORAGE_SLOT_EMPTY)
        {
            float toStore = slotCapacity - slot->GetAmount ();
            slot->Set (type, toStore);
            willBeStored -= toStore;
        }
        slotIndex++;
    }
    return willBeStored <= 0;
}

bool Storage::IsCanThrow (TradeGoodsType *type, float amount)
{
    assert (amount > 0);
    return GetAmountOf (Urho3D::StringHash (type->GetName ())) >= amount;
}

bool Storage::Throw (TradeGoodsType *type, float amount)
{
    assert (amount > 0);
    if (!IsCanThrow (type, amount))
        return false;
    float willBeThrowed = amount;
    int slotIndex = 0;
    while (willBeThrowed > 0 && slotIndex < GetStorageSlotsCount ())
    {
        StorageSlot *slot = &GetStorageSlot (slotIndex);
        if (slot->GetTypeId () == Urho3D::StringHash (type->GetName ()))
        {
            if (slot->GetAmount () > willBeThrowed)
            {
                slot->Set (type, slot->GetAmount () - willBeThrowed);
                willBeThrowed = 0.0f;
            }
            else
            {
                willBeThrowed -= slot->GetAmount ();
                slot->SetEmpty ();
            }
        }
        slotIndex++;
    }
    return willBeThrowed <= 0;
}

StorageSlot &Storage::GetStorageSlot (int index)
{
    assert (index >= 0);
    assert (index < GetStorageSlotsCount ());
    return storageSlots_.At (index);
}

void Storage::SetStorageSlot (int index, StorageSlot &slot)
{
    assert (index >= 0);
    assert (index < GetStorageSlotsCount ());
    storageSlots_.At (index) = slot;
}

int Storage::GetStorageEmptySlotsCount ()
{
    int count = 0;
    for (int index = 0; index < storageSlots_.Size (); index++)
        if (storageSlots_.At (index).GetTypeId () == STORAGE_SLOT_EMPTY)
            count++;
    return count;
}

StorageSlot &Storage::GetStorageFirstEmptySlot ()
{
    assert (GetStorageEmptySlotsCount ());
    for (int index = 0; index < storageSlots_.Size (); index++)
        if (storageSlots_.At (index).GetTypeId () == STORAGE_SLOT_EMPTY)
            return storageSlots_.At (index);

    // This will be never reached!
    assert (false);
}

int Storage::GetCountOfStorageSlotsWith (Urho3D::StringHash type)
{
    int count = 0;
    for (int index = 0; index < GetStorageSlotsCount (); index++)
        if (GetStorageSlot (index).GetTypeId () == type)
            count++;
    return count;
}

float Storage::GetAmountOf (Urho3D::StringHash type)
{
    float amount = 0.0f;
    for (int index = 0; index < GetStorageSlotsCount (); index++)
        if (GetStorageSlot (index).GetTypeId () == type)
            amount += GetStorageSlot (index).GetAmount ();
    return amount;
}

StorageSlot *Storage::GetFirstStorageSlotWith (Urho3D::StringHash type)
{
    for (int index = 0; index < GetStorageSlotsCount (); index++)
        if (GetStorageSlot (index).GetTypeId () == type)
            return &GetStorageSlot (index);
    return 0;
}

Storage::~Storage()
{

}
}


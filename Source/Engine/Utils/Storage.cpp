#include "Storage.hpp"
#include <Engine/Engine.hpp>
#include <Urho3D/Core/StringUtils.h>

namespace GameEngine
{
Storage::Storage () : storageSlots_ ()
{

}

void Storage::ClearStorage()
{
    for (int index = 0; index < GetStorageSlotsCount (); index++)
        GetStorageSlot (index)->SetEmpty ();
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
        if (GetStorageSlot (index)->GetTypeId () == Urho3D::StringHash (type->GetName ()))
            canStore += (slotCapacity - GetStorageSlot (index)->GetAmount ());
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
        StorageSlot *slot = GetStorageSlot (slotIndex);
        if (slot->GetTypeId () == Urho3D::StringHash (type->GetName ()) || slot->GetTypeId () == STORAGE_SLOT_EMPTY)
        {
            float canBeStored = slotCapacity - slot->GetAmount ();
            float toStore = canBeStored;
            if (canBeStored > willBeStored)
                toStore = willBeStored;
            slot->Set (type, slot->GetAmount () + toStore);
            willBeStored -= toStore;
        }
        slotIndex++;
    }
    return willBeStored <= 0;
}

bool Storage::IsCanThrow (TradeGoodsType *type, float amount)
{
    assert (type);
    assert (amount > 0);
    return GetAmountOf (Urho3D::StringHash (type->GetName ())) >= amount;
}

bool Storage::Throw (TradeGoodsType *type, float amount)
{
    assert (type);
    assert (amount > 0);
    if (!IsCanThrow (type, amount))
        return false;
    float willBeThrowed = amount;
    int slotIndex = 0;
    while (willBeThrowed > 0 && slotIndex < GetStorageSlotsCount ())
    {
        StorageSlot *slot = GetStorageSlot (slotIndex);
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

StorageSlot *Storage::GetStorageSlot (int index)
{
    assert (index >= 0);
    assert (index < GetStorageSlotsCount ());
    return &storageSlots_.At (index);
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

StorageSlot *Storage::GetStorageFirstEmptySlot()
{
    assert (GetStorageEmptySlotsCount ());
    for (int index = 0; index < storageSlots_.Size (); index++)
        if (storageSlots_.At (index).GetTypeId () == STORAGE_SLOT_EMPTY)
            return &storageSlots_.At (index);
    return 0;
}

int Storage::GetCountOfStorageSlotsWith (Urho3D::StringHash type)
{
    int count = 0;
    for (int index = 0; index < GetStorageSlotsCount (); index++)
        if (GetStorageSlot (index)->GetTypeId () == type)
            count++;
    return count;
}

float Storage::GetAmountOf (Urho3D::StringHash type)
{
    float amount = 0.0f;
    for (int index = 0; index < GetStorageSlotsCount (); index++)
        if (GetStorageSlot (index)->GetTypeId () == type)
            amount += GetStorageSlot (index)->GetAmount ();
    return amount;
}

StorageSlot *Storage::GetFirstStorageSlotWith (Urho3D::StringHash type)
{
    for (int index = 0; index < GetStorageSlotsCount (); index++)
        if (GetStorageSlot (index)->GetTypeId () == type)
            return GetStorageSlot (index);
    return 0;
}

void Storage::LoadStorageFromString (TradeGoodsTypesContainer *typesContainer, Urho3D::String string)
{
    assert (typesContainer);
    ClearStorage ();
    assert (!string.Empty ());
    Urho3D::Vector <Urho3D::String> splited = string.Split (';');
    assert (splited.Size () >= 2);
    int slotsCount = Urho3D::ToInt (splited.At (0));

    assert (splited.Size () == slotsCount + 1);
    if (splited.Size () != slotsCount + 1)
        return;
    SetStorageSlotsCount (slotsCount);

    for (int index = 0; index < slotsCount; index++)
    {
        Urho3D::String slotString = splited.At (index + 1);
        Urho3D::Vector <Urho3D::String> slotDef = slotString.Split ('=');
        assert (slotDef.Size () == 2);
        StorageSlot *slot = GetStorageSlot (index);
        if (Urho3D::StringHash (slotDef.At (0)) != STORAGE_SLOT_EMPTY)
            slot->Set (typesContainer->GetByHash (Urho3D::StringHash (slotDef.At (0))), Urho3D::ToFloat (slotDef.At (1)));
        else
            slot->SetEmpty ();
    }
}

Urho3D::String Storage::SaveStorageToString (TradeGoodsTypesContainer *typesContainer)
{
    assert (typesContainer);
    Urho3D::String string;
    string += Urho3D::String (GetStorageSlotsCount ()) + Urho3D::String (";");
    for (int index = 0; index < GetStorageSlotsCount (); index++)
    {
        StorageSlot *slot = GetStorageSlot (index);
        Urho3D::String typeName;
        if (slot->GetTypeId () != STORAGE_SLOT_EMPTY)
        {
            TradeGoodsType *type = typesContainer->GetByHash (slot->GetTypeId ());
            assert (type);
            if (type)
                typeName = type->GetName ();
            else
                typeName = "EMPTY";
        }
        else
            typeName = "EMPTY";
        string += typeName + Urho3D::String ("=") + Urho3D::String (slot->GetAmount ()) + Urho3D::String (";");
    }
    return string;
}

Storage::~Storage()
{

}
}


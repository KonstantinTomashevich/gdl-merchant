#pragma once
#include <Urho3D/Container/Vector.h>
#include <Engine/Objects/TradeGoodsTypesContainer.hpp>
#include "StorageSlot.hpp"

namespace GameEngine
{
class Storage
{
protected:
    Urho3D::Vector <StorageSlot> storageSlots_;
public:
    Storage ();
    void ClearStorage ();
    int GetStorageSlotsCount ();
    void SetStorageSlotsCount (int count);

    bool IsCanStore (TradeGoodsType *type, float amount);
    bool Store (TradeGoodsType *type, float amount);
    bool IsCanThrow (TradeGoodsType *type, float amount);
    bool Throw (TradeGoodsType *type, float amount);

    StorageSlot *GetStorageSlot (int index);
    void SetStorageSlot (int index, StorageSlot &slot);
    int GetStorageEmptySlotsCount ();
    StorageSlot *GetStorageFirstEmptySlot ();

    int GetCountOfStorageSlotsWith (Urho3D::StringHash type);
    float GetAmountOf (Urho3D::StringHash type);
    StorageSlot *GetFirstStorageSlotWith (Urho3D::StringHash type);
    void LoadStorageFromString (TradeGoodsTypesContainer *typesContainer, Urho3D::String string); //In format: "SLOTS_COUNT;SLOT_1;...;SLOT_N", each slot is "TYPE=AMOUNT"
    Urho3D::String SaveStorageToString (TradeGoodsTypesContainer *typesContainer);//In format: "SLOTS_COUNT;SLOT_1;...;SLOT_N", each slot is "TYPE=AMOUNT"
    virtual ~Storage ();
};
}

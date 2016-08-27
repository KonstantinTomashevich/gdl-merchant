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
    void Clear ();
    int GetSlotsCount ();
    void SetSlotsCount (int count);

    bool IsCanStore (TradeGoodsType *type, float amount);
    bool Store (TradeGoodsType *type, float amount);
    bool IsCanThrow (TradeGoodsType *type, float amount);
    bool Throw (TradeGoodsType *type, float amount);

    StorageSlot *GetSlot (int index);
    void SetSlot (int index, StorageSlot &slot);
    int GeteEmptySlotsCount ();
    StorageSlot *GetFirstEmptySlot ();

    int GetCountOfSlotsWith (Urho3D::StringHash type);
    float GetAmountOf (Urho3D::StringHash type);
    StorageSlot *GetFirstSlotWith (Urho3D::StringHash type);
    void LoadFromString (TradeGoodsTypesContainer *typesContainer, Urho3D::String string); //In format: "SLOTS_COUNT;SLOT_1;...;SLOT_N", each slot is "TYPE=AMOUNT"
    Urho3D::String SaveToString (TradeGoodsTypesContainer *typesContainer);//In format: "SLOTS_COUNT;SLOT_1;...;SLOT_N", each slot is "TYPE=AMOUNT"
    virtual ~Storage ();
};
}

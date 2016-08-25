#pragma once
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/XMLElement.h>
#include <Urho3D/Core/Variant.h>
#include <Engine/Interfaces/Component.hpp>
#include <Engine/Utils/TradeGoodsType.hpp>

namespace GameEngine
{
class TradeGoodsTypesContainer : public MapComponent
{
URHO3D_OBJECT (TradeGoodsTypesContainer, MapComponent)
protected:
    Urho3D::Vector <TradeGoodsType> types_;
public:
    TradeGoodsTypesContainer (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep) ;
    virtual bool Dispose ();
    virtual bool LoadFromXML (Urho3D::XMLElement rootElement);
    virtual bool ProcessEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    int GetCount ();
    TradeGoodsType &GetByIndex (int index);
    TradeGoodsType &GetByHash (Urho3D::StringHash nameHash);
    bool RemoveByIndex (int index);
    bool RemoveByHash (Urho3D::StringHash nameHash);
    bool Remove (TradeGoodsType& type);
    void Add (TradeGoodsType& type);
    virtual ~TradeGoodsTypesContainer ();
};
}

#include "TradeGoodsTypesContainer.hpp"
#include <Engine/Engine.hpp>
#include <Urho3D/IO/Log.h>

namespace GameEngine
{
TradeGoodsTypesContainer::TradeGoodsTypesContainer (Urho3D::Context *context) : MapComponent (context), types_ ()
{

}

bool TradeGoodsTypesContainer::Init ()
{
    if (!types_.Empty ())
    {
        ready_ = true;
        return true;
    }
    else
        return false;
}

bool TradeGoodsTypesContainer::Update (float timeStep)
{
    return true;
}

bool TradeGoodsTypesContainer::Dispose ()
{
    if (ready_)
    {
        ready_ = false;
        return true;
    }
    else
        return false;
}

bool TradeGoodsTypesContainer::LoadFromXML (Urho3D::XMLElement rootElement)
{
    assert (rootElement.HasChild ("type"));
    if (!rootElement.HasChild ("type"))
        return false;

    Urho3D::XMLElement typeXML = rootElement.GetChild ("type");
    while (typeXML.NotNull () && typeXML != Urho3D::XMLElement::EMPTY)
    {
        assert (typeXML.HasAttribute ("name"));
        assert (typeXML.HasAttribute ("storagingCost"));
        assert (typeXML.HasAttribute ("standartCost"));
        assert (typeXML.HasAttribute ("productionCost"));
        assert (typeXML.HasAttribute ("icon"));

        if (typeXML.HasAttribute ("name") && typeXML.HasAttribute ("storagingCost") && typeXML.HasAttribute ("standartCost") &&
                typeXML.HasAttribute ("productionCost") && typeXML.HasAttribute ("icon"))
        {
            Urho3D::String name = typeXML.GetAttribute ("name");
            float storagingCost = typeXML.GetFloat ("storagingCost");
            float standartCost = typeXML.GetFloat ("standartCost");
            float productionCost = typeXML.GetFloat ("productionCost");
            Urho3D::String icon = typeXML.GetAttribute ("icon");
            Urho3D::HashMap <Urho3D::StringHash, float> requiredForProduction;

            if (typeXML.HasChild ("requirmentsXML"))
            {
                Urho3D::XMLElement requirmentsXML = typeXML.GetChild ("requiredForProduction");
                Urho3D::XMLElement element = requirmentsXML.GetChild ("requirment");
                while (element.NotNull () && element != Urho3D::XMLElement::EMPTY)
                {
                    requiredForProduction [Urho3D::StringHash (element.GetAttribute ("name"))] = element.GetFloat ("amount");
                    element = element.GetNext ("requirment");
                }
            }

            TradeGoodsType type = TradeGoodsType (name, storagingCost, standartCost, productionCost, icon, requiredForProduction);
            Add (type);
        }
        typeXML = typeXML.GetNext ("type");
    }
    return true;
}

bool TradeGoodsTypesContainer::SaveToXML(Urho3D::XMLElement parentElement)
{
    //TODO: Implement this.
}

bool TradeGoodsTypesContainer::ProcessEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    return false;
}

int TradeGoodsTypesContainer::GetCount ()
{
    return types_.Size ();
}

TradeGoodsType &TradeGoodsTypesContainer::GetByIndex (int index)
{
    assert (index >= 0);
    assert (index < GetCount ());
    return types_.At (index);
}

TradeGoodsType &TradeGoodsTypesContainer::GetByHash (Urho3D::StringHash nameHash)
{
    assert (!types_.Empty ());
    for (int index = 0; index < types_.Size (); index++)
        if (Urho3D::StringHash (types_.At (index).GetName ()) == nameHash)
            return types_.At (index);

    // Will be never reached at normal situation.
    assert (false);
}

bool TradeGoodsTypesContainer::RemoveByIndex (int index)
{
    assert (index >= 0);
    assert (index < GetCount ());
    return types_.Remove (types_.At (index));
}

bool TradeGoodsTypesContainer::RemoveByHash (Urho3D::StringHash nameHash)
{
    assert (!types_.Empty ());
    Remove (GetByHash (nameHash));
}

bool TradeGoodsTypesContainer::Remove (TradeGoodsType &type)
{
    return types_.Remove (type);
}

void TradeGoodsTypesContainer::Add (TradeGoodsType &type)
{
    types_.Push (type);
}

TradeGoodsTypesContainer::~TradeGoodsTypesContainer()
{

}
}


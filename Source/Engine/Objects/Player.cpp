#include "Player.hpp"
#include <Engine/Engine.hpp>
#include "Caravan.hpp"

namespace GameEngine
{
void Player::OnComponentAdded (Urho3D::SharedPtr <PlayerComponent> component)
{
    if (component->GetPlayer () != this)
        component->SetPlayer (this, &protectedData_);
    if (!component->IsReady ())
        component->Init ();
}

void Player::OnComponentRemoved (Urho3D::SharedPtr <PlayerComponent> component)
{

}

void Player::OnComponentCreated (Urho3D::SharedPtr <PlayerComponent> component)
{

}

Player::Player (Urho3D::Context *context, Urho3D::String id, float gold, Urho3D::Color color) : Component (context),
    ComponentsHolder (context), protectedData_ (), relations_ ()
{
    protectedData_.Init (this);
    id_ = id;
    gold_ = gold;
    color_ = color;
}

bool Player::Init ()
{
    if (!ready_ && id_ != Urho3D::String::EMPTY)
    {
        for (int index = 0; index < components_.Size (); index++)
        {
            Urho3D::SharedPtr <PlayerComponent> component = components_.At (index);
            if (component->GetPlayer () != this)
                component->SetPlayer (this, &protectedData_);
            if (!component->IsReady ())
                component->Init ();
        }
        ready_ = true;
        return true;
    }
    else
    {
        ready_ = false;
        return false;
    }
}

bool Player::Update (float timeStep)
{
    ProcessDeferredEvents ();
    for (int index = 0; index < components_.Size (); index++)
    {
        Urho3D::SharedPtr <PlayerComponent> component = components_.At (index);
        if (component->IsWillBeUpdated () && component->IsReady ())
            component->Update (timeStep);
    }

    int index = 0;
    while (index < components_.Size ())
    {
        Urho3D::SharedPtr <PlayerComponent> component = components_.At (index);
        if (component->IsWillBeDeleted ())
        {
            component->Dispose ();
            components_.Remove (component);
            components_.Compact ();
        }
        else
            index++;
    }
    return true;
}

bool Player::Dispose ()
{
    if (ready_)
    {
        ready_ = false;
        for (int index = 0; index < components_.Size (); index++)
            if (components_.At (index)->IsReady ())
                components_.At (index)->Dispose ();
        return true;
    }
    else
        return false;
}

bool Player::LoadFromXML (Urho3D::XMLElement rootElement)
{
    assert (rootElement.HasAttribute ("id"));
    assert (rootElement.HasAttribute ("gold"));
    assert (rootElement.HasAttribute ("color"));
    if (!rootElement.HasAttribute ("id") || !rootElement.HasAttribute ("gold") || !rootElement.HasAttribute ("color"))
        return false;

    id_ = rootElement.GetAttribute ("id");
    gold_ = rootElement.GetFloat ("gold");
    color_ = rootElement.GetColor ("color");

    Urho3D::XMLElement componentXML = rootElement.GetChild ("component");
    while (componentXML.NotNull () && componentXML != Urho3D::XMLElement::EMPTY)
    {
        Urho3D::String type = componentXML.GetAttribute ("type");
        Urho3D::SharedPtr <PlayerComponent> component;
        component.StaticCast (context_->CreateObject (Urho3D::StringHash (type)));
        assert (component.NotNull ());
        if (component.NotNull ())
        {
            component->SetPlayer (this, &protectedData_);
            component->LoadFromXML (componentXML);
            AddComponent (component);
        }
        componentXML = componentXML.GetNext ("component");
    }

    if (rootElement.HasChild ("relation"))
    {
        relations_.Clear ();
        Urho3D::XMLElement relation = rootElement.GetChild ("relation");
        while (relation.NotNull () && relation != Urho3D::XMLElement::EMPTY)
        {
            relations_ [Urho3D::StringHash (relation.GetAttribute ("name"))] = relation.GetFloat ("value");
            relation = relation.GetNext ("relation");
        }
    }
}

bool Player::ProcessEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    bool isProcessed = false;
    for (int index = 0; index < components_.Size (); index++)
    {
        Urho3D::SharedPtr <PlayerComponent> component = components_.At (index);
        bool localResult = false;
        if (component->IsReady ())
            localResult = component->ProcessEvent (eventType, eventData);
        if (localResult)
            isProcessed = true;
    }
    return isProcessed;
}

Urho3D::String Player::GetId ()
{
    return id_;
}

float Player::GetGold ()
{
    return gold_;
}

Urho3D::Color Player::GetColor ()
{
    return color_;
}

float Player::GetRelationsWith (Urho3D::StringHash id)
{
    if (!relations_.Contains (id))
        relations_ [id] = 0.0f;
    return relations_ [id];
}

Urho3D::SharedPtr <Caravan> Player::CreateCaravan (Map *map, int storageSlots, Urho3D::Vector2 position)
{
    Urho3D::SharedPtr <Caravan> caravan = Urho3D::SharedPtr <Caravan> (new Caravan (context_));
    map->Add (caravan);
    caravan->SetPosition (position);
    caravan->GetStorage ()->SetStorageSlotsCount (storageSlots);
    caravan->SetPlayer (this, &protectedData_);
    return caravan;
}

Player::~Player ()
{

}
}

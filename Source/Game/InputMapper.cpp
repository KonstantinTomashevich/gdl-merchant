#include "InputMapper.hpp"
#include <Urho3D/Core/CoreEvents.h>
#include <Game/BuildConfig.hpp>

InputMapper::InputMapper (Urho3D::Context *context) : Urho3D::Object (context), actions_ ()
{
    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (InputMapper, OnUpdateEvent));
    SubscribeToEvent (Events::E_SET_ACTION_STATE, URHO3D_HANDLER (InputMapper, OnSetActionStateEvent));
    SubscribeToEvent (Events::E_TOGGLE_ACTION_STATE, URHO3D_HANDLER (InputMapper, OnToggleActionStateEvent));
}

void InputMapper::OnUpdateEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    for (int index = 0; index < actions_.Size (); index++)
        actions_.At (index).Update (context_->GetSubsystem <Urho3D::Input> ());
}

void InputMapper::OnSetActionStateEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::String name = eventData [Events::SetActionState::P_NAME].GetString ();
    assert (ContainsAction (name));
    if (ContainsAction (name))
    {
        Urho3D::VariantMap settingData = eventData [Events::SetActionState::P_STATE_CHANGE_DATA].GetVariantMap ();
        GetAction (name).SetState (eventData [Events::SetActionState::P_STATE].GetBool (), settingData);
    }
}

void InputMapper::OnToggleActionStateEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::String name = eventData [Events::ToggleActionState::P_NAME].GetString ();
    assert (ContainsAction (name));
    if (ContainsAction (name))
    {
        InputAction &action = GetAction (name);
        Urho3D::VariantMap settingData = eventData [Events::ToggleActionState::P_STATE_CHANGE_DATA].GetVariantMap ();
        action.SetState (!action.GetState (), settingData);
    }
}

bool InputMapper::ContainsAction (Urho3D::String name)
{
    for (int index = 0; index < actions_.Size (); index++)
        if (actions_.At (index).GetName () == name)
            return true;
    return false;
}

void InputMapper::AddAction(InputAction &action)
{
    if (!ContainsAction (action.name_))
        actions_.Push (action);
}

InputAction &InputMapper::GetAction (Urho3D::String name)
{
    assert (ContainsAction (name));
    for (int index = 0; index < actions_.Size (); index++)
        if (actions_.At (index).GetName () == name)
            return actions_.At (index);

    // Will be never reached!
    assert (false);
    return *(new InputAction ());
}

bool InputMapper::Is (Urho3D::String name)
{
    return GetAction (name).GetState ();
}

bool InputMapper::LoadFromXML (Urho3D::XMLElement element)
{
    assert (element.NotNull ());
    assert (element.HasChild ("action"));

    if (!element.HasChild ("action"))
        return false;
    Urho3D::XMLElement actionXML = element.GetChild ("action");
    while (actionXML.NotNull () && actionXML != Urho3D::XMLElement::EMPTY)
    {
        assert (actionXML.HasAttribute ("name"));
        if (actionXML.HasAttribute ("name"))
        {
            Urho3D::String name = actionXML.GetAttribute ("name");
            Urho3D::Vector <Urho3D::String> keys =
                    actionXML.HasAttribute ("keys") ? actionXML.GetAttribute ("keys").Split (';') : Urho3D::Vector <Urho3D::String> ();

            OnKeyReactionMode onKeyReactonMode;
            if (actionXML.HasAttribute ("onKeyReactionMode"))
            {
                Urho3D::String reactionMode = actionXML.GetAttribute ("onKeyReactionMode");
                if (reactionMode == "None")
                    onKeyReactonMode = KR_NONE;
                else if (reactionMode == "IfDown")
                    onKeyReactonMode = KR_IF_DOWN;
                else if (reactionMode == "IfNotDown")
                    onKeyReactonMode = KR_IF_NOT_DOWN;
                else if (reactionMode == "Toggle")
                    onKeyReactonMode = KR_TOGGLE;
            }
            else
                onKeyReactonMode = KR_NONE;

            bool canBeManuallyEdited =
                    actionXML.HasAttribute ("canBeManuallyEdited") ? actionXML.GetBool ("canBeManuallyEdited") : true;
            InputAction action = InputAction (name, keys, onKeyReactonMode, canBeManuallyEdited);
            AddAction (action);
        }
        actionXML = actionXML.GetNext ("action");
    }
}

InputMapper::~InputMapper()
{
    UnsubscribeFromAllEvents ();
}


void InputAction::UpdateByKeys (Urho3D::Input *input)
{
    if (onKeyReactionMode_ != KR_NONE)
    {
        int trueSituations = 0;
        for (int index = 0; index < keys_.Size (); index++)
        {
            if (onKeyReactionMode_ == KR_TOGGLE)
            {
                if (input->GetKeyPress (input->GetKeyFromName (keys_.At (index))))
                    trueSituations++;
            }
            else
            {
                bool isDown = input->GetKeyDown (input->GetKeyFromName (keys_.At (index)));
                if (onKeyReactionMode_ == KR_IF_DOWN && isDown)
                    trueSituations++;
                else if (onKeyReactionMode_ == KR_IF_NOT_DOWN && isDown)
                    trueSituations = -999;
                else if (onKeyReactionMode_ == KR_IF_NOT_DOWN)
                    trueSituations++;
            }
        }
        state_ = (trueSituations > 0);
    }
    else
        state_ = false;
    settedByKey_ = true;
    lastStateChangeData_ [IsActionStateSettedByKey] = true;
}

InputAction::InputAction () : name_ (Urho3D::String::EMPTY), keys_ (), onKeyReactionMode_ (KR_NONE), canBeManuallyEdited_ (true)
{
    state_ = false;
    settedByKey_ = true;
    isUpdateIfTrueAndNotSettedByKey_ = false;
    isUpdateIfFalseAndNotSettedByKey_ = true;
    lastStateChangeData_ = Urho3D::VariantMap ();
}

InputAction::InputAction (Urho3D::String name, Urho3D::Vector<Urho3D::String> keys,
                          OnKeyReactionMode onKeyReactionMode, bool canBeManuallyEdited) :
    name_ (name), keys_ (keys), onKeyReactionMode_ (onKeyReactionMode), canBeManuallyEdited_ (canBeManuallyEdited)
{
    state_ = false;
    settedByKey_ = true;
    isUpdateIfTrueAndNotSettedByKey_ = false;
    isUpdateIfFalseAndNotSettedByKey_ = true;
    lastStateChangeData_ = Urho3D::VariantMap ();
}

void InputAction::Update (Urho3D::Input *input)
{
    assert (input);
    if (settedByKey_ ||
            (!settedByKey_ && state_ && isUpdateIfTrueAndNotSettedByKey_) ||
            (!settedByKey_ && !state_ && isUpdateIfFalseAndNotSettedByKey_))
        UpdateByKeys (input);
}

Urho3D::String InputAction::GetName ()
{
    return name_;
}

Urho3D::Vector<Urho3D::String> InputAction::GetKeys ()
{
    return keys_;
}

OnKeyReactionMode InputAction::GetOnKeyReactionMode ()
{
    return onKeyReactionMode_;
}

bool InputAction::IsCanBeManuallyEdited ()
{
    return canBeManuallyEdited_;
}

bool InputAction::GetState ()
{
    return state_;
}

void InputAction::SetState (bool state, Urho3D::VariantMap &stateSettingData)
{
    assert (canBeManuallyEdited_);
    state_ = state;
    settedByKey_ = false;
    lastStateChangeData_ = stateSettingData;
    lastStateChangeData_ [IsActionStateSettedByKey] = false;
}

Urho3D::VariantMap &InputAction::GetLastStateChangeData()
{
    return lastStateChangeData_;
}

InputAction::~InputAction()
{

}

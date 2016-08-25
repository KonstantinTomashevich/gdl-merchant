#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Input/Input.h>

namespace Events
{
URHO3D_EVENT (E_SET_ACTION_STATE, SetActionState)
{
    URHO3D_PARAM (P_NAME, Name); // Urho3D::String.
    URHO3D_PARAM (P_STATE, State); // bool.
    URHO3D_PARAM (P_STATE_CHANGE_DATA, StateChangeData); // Urho3D::VariantMap.
}

URHO3D_EVENT (E_TOGGLE_ACTION_STATE, ToggleActionState)
{
    URHO3D_PARAM (P_NAME, Name); // Urho3D::String.
    URHO3D_PARAM (P_STATE_CHANGE_DATA, StateChangeData); // Urho3D::VariantMap.
}
}

enum OnKeyReactionMode
{
    KR_NONE = 0,
    KR_IF_DOWN = 1,
    KR_IF_NOT_DOWN = 2,
    KR_TOGGLE = 3
};


static const Urho3D::StringHash IsActionStateSettedByKey ("IsActionStateSettedByKey");
class InputAction
{
protected:
    bool state_;
    Urho3D::VariantMap lastStateChangeData_;
    bool settedByKey_;
    bool isUpdateIfTrueAndNotSettedByKey_;
    bool isUpdateIfFalseAndNotSettedByKey_;
    void UpdateByKeys (Urho3D::Input *input);
public:
    Urho3D::String name_;
    /// See https://wiki.libsdl.org/SDL_Keycode .
    Urho3D::Vector <Urho3D::String> keys_;
    OnKeyReactionMode onKeyReactionMode_;
    bool canBeManuallyEdited_;

    InputAction ();
    InputAction (Urho3D::String name, Urho3D::Vector <Urho3D::String> keys, OnKeyReactionMode onKeyReactionMode, bool canBeManuallyEdited);
    void Update (Urho3D::Input *input);
    Urho3D::String GetName ();
    Urho3D::Vector <Urho3D::String> GetKeys ();
    OnKeyReactionMode GetOnKeyReactionMode ();
    bool IsCanBeManuallyEdited ();
    bool GetState ();
    void SetState (bool state, Urho3D::VariantMap &stateSettingData);
    Urho3D::VariantMap &GetLastStateChangeData ();
    virtual ~InputAction ();
};

class InputMapper : public Urho3D::Object
{
URHO3D_OBJECT (InputMapper, Urho3D::Object)
protected:
    Urho3D::Vector <InputAction> actions_;
public:
    InputMapper (Urho3D::Context *context);
    void OnUpdateEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void OnSetActionStateEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void OnToggleActionStateEvent (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    bool ContainsAction (Urho3D::String name);
    void AddAction (InputAction &action);
    InputAction &GetAction (Urho3D::String name);
    bool Is (Urho3D::String name);
    bool LoadFromXML (Urho3D::XMLElement element);
    virtual ~InputMapper ();
};

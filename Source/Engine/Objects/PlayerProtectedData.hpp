#pragma once
#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Math/Color.h>

namespace GameEngine
{
class Player;
class PlayerProtectedData
{
friend class Player;
protected:
    Player *player_;
    void Init (Player *player);
public:
    PlayerProtectedData ();
    Urho3D::HashMap <Urho3D::StringHash, float> *GetRelationsMap ();
    void SetId (Urho3D::String id);
    void SetGold (float gold);
    void SetColor (Urho3D::Color color);
    virtual ~PlayerProtectedData ();
};
}

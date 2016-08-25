#include "PlayerProtectedData.hpp"
#include "Player.hpp"

namespace GameEngine
{
void PlayerProtectedData::Init (Player *player)
{
    player_ = player;
}

PlayerProtectedData::PlayerProtectedData ()
{

}

void PlayerProtectedData::SetRelation (Urho3D::StringHash id, float value)
{
    player_->relations_ [id] = value;
}

Urho3D::HashMap <Urho3D::StringHash, float> *PlayerProtectedData::GetRelationsMap ()
{
    return &player_->relations_;
}

void PlayerProtectedData::SetId (Urho3D::String id)
{
    player_->id_ = id;
}

void PlayerProtectedData::SetGold (float gold)
{
    player_->gold_ = gold;
}

void PlayerProtectedData::SetColor (Urho3D::Color color)
{
    player_->color_ = color;
}

PlayerProtectedData::~PlayerProtectedData ()
{

}
}


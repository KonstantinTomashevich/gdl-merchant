#include "Ownable.hpp"

namespace GameEngine
{
Ownable::Ownable (Player *player)
{
    player_ = player;
}

Player *Ownable::GetPlayer ()
{
    return player_;
}

void Ownable::SetPlayer (Player *player)
{
    player_ = player;
}

Ownable::~Ownable()
{

}
}


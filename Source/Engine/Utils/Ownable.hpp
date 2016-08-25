#pragma once
#include <Engine/Objects/Player.hpp>

namespace GameEngine
{
class Ownable
{
protected:
    Player *player_;
public:
    Ownable (Player *player = 0);
    Player *GetPlayer ();
    void SetPlayer (Player *player);
    virtual ~Ownable ();
};
}

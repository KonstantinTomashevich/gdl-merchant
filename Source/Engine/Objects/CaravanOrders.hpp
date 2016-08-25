#pragma once
#include <Urho3D/Math/StringHash.h>
#include <Engine/Engine.hpp>

namespace GameEngine
{
namespace CaravanOrders
{
GameEngineEvent (Move)
{
    /// Urho3D::Vector2.
    GameEngineEventParameter (Target);
}

GameEngineEvent (Follow)
{
    /// MapObject pointer.
    GameEngineEventParameter (Object);
}

GameEngineEvent (Attack)
{
    /// MapObject pointer.
    GameEngineEventParameter (Object);
}

GameEngineEvent (StoreTo)
{
    /// Storage pointer.
    GameEngineEventParameter (Storage);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (ExchangeFrom)
{
    /// Storage pointer.
    GameEngineEventParameter (Storage);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (Buy)
{
    /// Trader pointer.
    GameEngineEventParameter (Trader);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (Sell)
{
    /// Trader pointer.
    GameEngineEventParameter (Trader);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (CreateMarketplaceTrader)
{
    /// Marketplace pointer.
    GameEngineEventParameter (Marketplace);
    /// Urho3D::VariantVector. All variants will be Urho3D::VariantMap. Can be empty (if no missions).
    GameEngineEventParameter (MissionsData);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (WithWhat);
}
}
}

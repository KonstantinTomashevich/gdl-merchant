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
    /// int. Index of object in map.
    GameEngineEventParameter (ObjectIndex);
}

GameEngineEvent (Attack)
{
    /// int. Index of object in map.
    GameEngineEventParameter (ObjectIndex);
}

GameEngineEvent (StoreTo)
{
    /// int. Index of object in map. Object will inherit Trader, because trader contains storage.
    GameEngineEventParameter (ObjectWithStorageIndex);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (ExchangeFrom)
{
    /// int. Index of object in map. Object will inherit Trader, because trader contains storage.
    GameEngineEventParameter (ObjectWithStorageIndex);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (Buy)
{
    /// int. Index of object in map.
    GameEngineEventParameter (TraderObjectIndex);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (Sell)
{
    /// int. Index of object in map.
    GameEngineEventParameter (TraderObjectIndex);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (CreateMarketplaceTrader)
{
    /// int. Index of object in map.
    GameEngineEventParameter (MarketplaceTownIndex);
    /// Urho3D::VariantVector. All variants will be Urho3D::VariantMap. Can be empty (if no missions).
    GameEngineEventParameter (MissionsData);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (WithWhat);
}

GameEngineEvent (BuyFromMarketplace)
{
    /// int. Index of object in map.
    GameEngineEventParameter (MarketplaceTownIndex);
    /// int. Index of marketplace slot with trader.
    GameEngineEventParameter (TraderIndex);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (SellToMarketplace)
{
    /// int. Index of object in map.
    GameEngineEventParameter (MarketplaceTownIndex);
    //// int. Index of marketplace slot with trader.
    GameEngineEventParameter (TraderIndex);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}
}
}

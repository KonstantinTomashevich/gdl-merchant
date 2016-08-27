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
    /// int. Identifier number of object in map.
    GameEngineEventParameter (ObjectId);
}

GameEngineEvent (Attack)
{
    /// int. Identifier number of object in map.
    GameEngineEventParameter (ObjectId);
}

GameEngineEvent (StoreTo)
{
    /// int. Identifier number of object in map. Object will inherit Trader, because trader contains storage.
    GameEngineEventParameter (ObjectWithStorageId);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (ExchangeFrom)
{
    /// int. Identifier number of object in map. Object will inherit Trader, because trader contains storage.
    GameEngineEventParameter (ObjectWithStorageId);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (Buy)
{
    /// int. Identifier number of object in map.
    GameEngineEventParameter (TraderObjectId);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (Sell)
{
    /// int. Identifier number of object in map.
    GameEngineEventParameter (TraderObjectId);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (CreateMarketplaceTrader)
{
    /// int. Identifier number of object in map.
    GameEngineEventParameter (MarketplaceTownId);
    /// Urho3D::VariantVector. All variants will be Urho3D::VariantMap. Can be empty (if no missions).
    GameEngineEventParameter (MissionsData);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (WithWhat);
}

GameEngineEvent (BuyFromMarketplace)
{
    /// int. Identifier number of object in map.
    GameEngineEventParameter (MarketplaceTownId);
    /// int. Identifier number of marketplace trader.
    GameEngineEventParameter (TraderId);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}

GameEngineEvent (SellToMarketplace)
{
    /// int. Identifier number of object in map.
    GameEngineEventParameter (MarketplaceTownId);
    /// int. Identifier number of marketplace trader.
    GameEngineEventParameter (TraderId);
    /// Urho3D::VariantMap. All variants will be floats!
    GameEngineEventParameter (What);
}
}
}

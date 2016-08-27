#pragma once
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Container/Vector.h>
#include <StatesEngine/StateObject.hpp>

class TestXMLSerialization : public Urho3D::Application
{
URHO3D_OBJECT (TestXMLSerialization, Urho3D::Application)
public:
    TestXMLSerialization (Urho3D::Context *context);
    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
    virtual ~TestXMLSerialization();
};

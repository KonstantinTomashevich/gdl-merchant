#pragma once
#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <TestingEngine/TestsRunner.hpp>

class Urho3DApplication : public Urho3D::Application
{
URHO3D_OBJECT (Urho3DApplication, Application)
protected:
    Urho3D::SharedPtr <TestingEngine::TestsRunner> testsRunner_;
public:
    Urho3DApplication(Urho3D::Context *context);
    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
    virtual ~Urho3DApplication();
};

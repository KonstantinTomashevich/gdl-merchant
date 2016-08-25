#pragma once
#cmakedefine ENGINE_ENABLE_ASSERTION
#ifdef ENGINE_ENABLE_ASSERTION
#undef NDEBUG
#include <assert.h>
#endif

#include <Urho3D/Math/StringHash.h>
#define GameEngineEvent(name) \
    static const Urho3D::StringHash name (#name); \
    namespace name ## Data \

#define GameEngineEventParameter(name) \
    static const Urho3D::StringHash name (#name)


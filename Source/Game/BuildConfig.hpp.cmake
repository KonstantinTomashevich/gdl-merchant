#pragma once
#cmakedefine GAME_ENABLE_ASSERTION
#ifdef GAME_ENABLE_ASSERTION
#undef NDEBUG
#endif

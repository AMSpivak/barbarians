#ifndef GL_I_GL_GAME_STATE_ARENA
#define GL_I_GL_GAME_STATE_ARENA
#include "i_gl_game_state.h"
class IGlGameStateArena:IGlGameState
{
    ~IGlGameStateArena(){}
    void Draw();
    void Process();
};

#endif
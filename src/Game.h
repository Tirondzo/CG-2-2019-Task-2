#include <LiteMath.h>
using namespace LiteMath;

struct GameStateCurrent
{
    float2 mousePos;
    bool keys[1024];
    float gameTime;
};

class GameState : GameStateCurrent
{
public:
    GameState(GameStateCurrent curr, GameStateCurrent prev) :
                GameStateCurrent(curr),
                prev(prev){}

    GameStateCurrent prev;
};
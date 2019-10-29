#include "MyGame.h"

class MyGameScopeGuard
{
public:
  MyGameScopeGuard() { init_my_game(); }
  ~MyGameScopeGuard() { close_my_game(); }
};

int main()
{
  MyGameScopeGuard();
  get_my_game()->start();
}
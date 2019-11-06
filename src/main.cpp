#include "MyGame.h"

class MyGameScopeGuard
{
public:
  MyGameScopeGuard() { init_my_game(); }
  ~MyGameScopeGuard() { close_my_game(); }
};

int main()
{
  try {
    MyGameScopeGuard scope = MyGameScopeGuard();
    get_my_game()->start();
  } catch(std::exception &e) {
    std::cout << std::endl
              << "Exception happened! See details." << std::endl;
    std::cout << e.what() << std::endl;
  }
}
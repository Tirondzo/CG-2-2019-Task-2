#include "MyGame.h"

#include <memory>

std::unique_ptr<MyGame> my_game;
void init_my_game(){ my_game = std::make_unique<MyGame>(); }
void close_my_game(){ my_game.reset(); }
MyGame *get_my_game(){ return my_game.get(); }


// Main Game Logic and so on

#include "framework/Game.h"
#include "framework/Mesh.h"
#include "MyCameras.h"

GameWindow gw;
MyGame::MyGame()
{
  gw = GameWindow(800, 600, "CG/2 Task 2. 2019");

  // Init all resources at cetera...
}

MyGame::~MyGame()
{
  // Free all resources
}

void MyGame::start()
{
  // MainLoop
  while (!glfwWindowShouldClose(gw.get()))
  {
    gw.update();
    update(gw.getState());
  }
}

void update(const GameState &gs)
{

}
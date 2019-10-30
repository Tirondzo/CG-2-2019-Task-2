#pragma once

#include "framework/Game.h"
#include "framework/Mesh.h"
#include "framework/ShaderProgram.h"
#include "framework/HelperGL.h"
#include "MyCameras.h"
typedef ShaderProgram Shader;

class MyGame
{
  GameWindow* gw;
  FreeCamera* fc;
  Mesh* triangle;
  Shader* tri_sh;

  float4x4 projection;
  void update(const GameState &gs);
public:
  MyGame();
  ~MyGame();
  void start();
};

// Singleton
void init_my_game();
void close_my_game();
MyGame *get_my_game();

// Singleton means that there can be only one instance for all program. (single)
// MyGame is the exactly this case.
// So you can get acces to MyGame instance from any part of this code just by including "MyGame.h"
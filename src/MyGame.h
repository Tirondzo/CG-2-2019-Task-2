#pragma once

#include "framework/Game.h"
#include "framework/Objects3d.h"
#include "framework/Primitives.h"
#include "framework/ShaderProgram.h"
#include "framework/HelperGL.h"
#include "MyCameras.h"
typedef ShaderProgram Shader;

class MyGame
{
  // feel free to
  // improve this
  GameWindow* gw;
  FreeCamera* fc;
  TankCamera* tc;
  Mesh* triangle;
  Mesh* box_targ;
  Mesh* plane_gr;
  Model* my_tank;
  Shader* grid_s;
  Shader* simp_s;
  Shader* targ_s;
  Shader* tank_s;
  // links to res
  Camera* my_cam;

  // you can use smart pointers for the code above

  float4x4 projection;
  void update(const GameState &gs);
  void draw(const GameState &gs);
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
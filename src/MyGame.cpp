#include "MyGame.h"
#include <memory>

// Main Game Logic and so on



MyGame::MyGame()
{
  gw = new GameWindow(800, 600, "CG/2 Task 2. 2019");
  fc = new FreeCamera();
  triangle = CreateSimpleTriangleMesh();

  tri_sh = new ShaderProgram({
    {GL_VERTEX_SHADER, "shaders/vertex.glsl"},
    {GL_FRAGMENT_SHADER, "shaders/fragment.glsl"}
  }); GL_CHECK_ERRORS;

  projection = projectionMatrixTransposed(90.0, gw->getViewWidth()/gw->getViewHeight(), 0.1, 1000.0);

  // Init all resources at cetera...
}

MyGame::~MyGame()
{
  // Free all resources
  delete gw;
  delete fc;
  delete triangle;
  delete tri_sh;
}

void MyGame::update(const GameState &gs)
{
  glClearColor(0.1f, 0.6f, 0.8f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  fc->update(gs);
  tri_sh->StartUseShader();
  tri_sh->SetUniform("projection", projection);
  tri_sh->SetUniform("view", fc->getViewMatrix());
  triangle->draw();
  tri_sh->StopUseShader();
}

void MyGame::start()
{

  glEnable(GL_DEPTH_TEST);

  // MainLoop
  while (!glfwWindowShouldClose(gw->get()))
  {
    gw->update();
    update(gw->getState());
    glfwSwapBuffers(gw->get());
  }
}


// Singleton
std::unique_ptr<MyGame> my_game;
void init_my_game(){ my_game = std::make_unique<MyGame>(); }
void close_my_game(){ my_game.reset(); }
MyGame *get_my_game(){ return my_game.get(); }

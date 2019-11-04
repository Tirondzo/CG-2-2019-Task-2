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
  projection = transpose(projection);

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

void MyGame::start()
{

  glEnable(GL_DEPTH_TEST);

  // MainLoop
  while (!glfwWindowShouldClose(gw->get()))
  {
    gw->update();
    update(gw->getState());

    draw(gw->getState());
    glfwSwapBuffers(gw->get());
  }
}

bool key_released(const GameState &gs, int key)
{
  // key not pressed for now but was pressed in prev frame
  return !gs.keyboard[key] && gs.prev.keyboard[key];
}

bool mbtn_released(const GameState &gs, int key)
{
  return !gs.mouse[key] && gs.prev.mouse[key];
}

void MyGame::update(const GameState &gs)
{
  if (key_released(gs, GLFW_KEY_ESCAPE))
    glfwSetWindowShouldClose(gw->get(), GL_TRUE);

  if (mbtn_released(gs, GLFW_MOUSE_BUTTON_RIGHT))
    glfwSetInputMode(gw->get(), GLFW_CURSOR,
                     gs.cursorCaptured ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

  fc->update(gs);
}

void MyGame::draw(const GameState &gs)
{
  glClearColor(0.1f, 0.6f, 0.8f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  tri_sh->StartUseShader();
  tri_sh->SetUniform("projection", projection);
  tri_sh->SetUniform("view", fc->getViewMatrix());
  triangle->Draw();
  tri_sh->StopUseShader();
}


// Singleton
std::unique_ptr<MyGame> my_game;
void init_my_game(){ my_game = std::make_unique<MyGame>(); }
void close_my_game(){ my_game.reset(); }
MyGame *get_my_game(){ return my_game.get(); }

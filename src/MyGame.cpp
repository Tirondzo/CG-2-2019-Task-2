#include "MyGame.h"
#include <memory>
#include <algorithm>

#include "framework/LoaderGLTF.h"

// Main Game Logic and so on



MyGame::MyGame()
{
  gw = new GameWindow(800, 600, "CG/2 Task 2. 2019");
  fc = new FreeCamera();
  tc = new TankCamera();
  fc->setPos(float3(0,2,5));
  my_cam = fc;
  triangle = CreateSimpleTriangleMesh();
  plane_gr = CreateSimplePlaneMesh();
  box_targ = CreateSimpleBoxMesh();

  simp_s = new ShaderProgram({
    {GL_VERTEX_SHADER, "shaders/triangle/vertex.glsl"},
    {GL_FRAGMENT_SHADER, "shaders/triangle/fragment.glsl"}
  }); GL_CHECK_ERRORS;

  grid_s = new ShaderProgram({
    {GL_VERTEX_SHADER, "shaders/grid/vertex.glsl"},
    {GL_FRAGMENT_SHADER, "shaders/grid/fragment.glsl"}
  }); GL_CHECK_ERRORS;

  tank_s = new ShaderProgram({
    {GL_VERTEX_SHADER, "shaders/tank/vertex.glsl"},
    {GL_FRAGMENT_SHADER, "shaders/tank/fragment.glsl"}
  }); GL_CHECK_ERRORS;

  targ_s = new ShaderProgram({
    {GL_VERTEX_SHADER, "shaders/targets/vertex.glsl"},
    {GL_FRAGMENT_SHADER, "shaders/targets/fragment.glsl"}
  }); GL_CHECK_ERRORS;

  // debug model
  // my_tank = new GLTFModel("assets/Cube/Cube.gltf");

  my_tank = new GLTFModel("assets/Tank_M60A1_opt/tank_m60a1.gltf");

  // Tank model consists of {"Head", "Body"} meshes
  const auto& it = std::find_if(my_tank->GetMeshes().begin(), my_tank->GetMeshes().end(), [](Mesh *m){
    return m->GetName() == "Head";
  });
  if (it != my_tank->GetMeshes().end())
    tc->setPos(to_float3((*it)->GetTransform().get_col(3))); // set camera to tank Head pos

  // Create projection matrix
  projection = projectionMatrixTransposed(60.0, double(gw->getViewWidth())/gw->getViewHeight(), 0.1, 1000.0);
  projection = transpose(projection);

  // Init all resources at cetera...
}

MyGame::~MyGame()
{
  // Free all resources
  delete gw;
  delete fc;
  delete tc;
  delete simp_s;
  delete grid_s;
  delete targ_s;
  delete tank_s;
  delete my_tank;
  delete box_targ;
  delete plane_gr;
  delete triangle;
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

  if (key_released(gs, GLFW_KEY_F))
    my_cam = (my_cam == fc) ? (Camera*)tc : (Camera*)fc;

  if (key_released(gs, GLFW_KEY_SPACE))
  {
    float3 pos = my_cam->getPos();
    std::cout << "Camera pos: " << pos.x << ' ' << pos.y << ' ' << pos.z << std::endl;
  }

  if (mbtn_released(gs, GLFW_MOUSE_BUTTON_RIGHT))
    glfwSetInputMode(gw->get(), GLFW_CURSOR,
                     gs.cursorCaptured ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

  my_cam->update(gs);
}

void MyGame::draw(const GameState &gs)
{
  glClearColor(0.1f, 0.6f, 0.8f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float4x4 view = my_cam->getViewMatrix();

  grid_s->StartUseShader();
  grid_s->SetUniform("projection", projection);
  grid_s->SetUniform("view", view);
  plane_gr->Draw();
  grid_s->StopUseShader();

  targ_s->StartUseShader();
  targ_s->SetUniform("projection", projection);
  targ_s->SetUniform("view", view);
  box_targ->DrawInstanced(5);
  targ_s->StopUseShader();

  tank_s->StartUseShader();
  tank_s->SetUniform("projection", projection);
  tank_s->SetUniform("view", view);
  for (Mesh* m : my_tank->GetMeshes())
  {
    float4x4 mt = m->GetTransform();
    if (m->GetName() == "Head")
      mt = mul(mt, rotate_Y_4x4(tc->getYPR().x));
    tank_s->SetUniform("model", mt);
    m->Draw();
  }
  tank_s->StopUseShader();
}


// Singleton
std::unique_ptr<MyGame> my_game;
void init_my_game(){ my_game = std::make_unique<MyGame>(); }
void close_my_game(){ my_game.reset(); }
MyGame *get_my_game(){ return my_game.get(); }

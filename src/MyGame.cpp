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

  // [TASK] Create projection matrix here and store it in 'projection' variable
  // [HELP] ratio is double(gw->getViewWidth())/gw->getViewHeight()


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

#define SHADER_ZONE(shader) ShaderZone ShaderZoneGuardName ## __LINE__(this, shader);

void MyGame::draw(const GameState &gs)
{
  glClearColor(0.1f, 0.6f, 0.8f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float4x4 view = my_cam->getViewMatrix();

  {
    SHADER_ZONE(simp_s);
    // [TASK] Draw triangle here

  }

  {
    SHADER_ZONE(grid_s);
    // [TASK] Draw plane_gr here

  }

  {
    SHADER_ZONE(targ_s);
    // [TASK] Draw Instanced box_targ here

  }

  {
    SHADER_ZONE(tank_s);
    for (Mesh* m : my_tank->GetMeshes())
    {
      // [TASK] Set model transform here
      // [HELP] m->GetTransform() gets local transform

      // [TASK] Set cannon rotation here
      // [HELP] m->GetName() gets mesh name
      // [HELP] tc->getYPR().x gets tank camera Yaw angle
      // [HELP] rotate_Y_4x4 calculates rotation matrix around Y axis

      // [HELP] tank_s->SetUniform("model", ...); sets model matrix transform

      // [TASK] Draw Mesh here

    }
  }

}

// Shader Zone helper
MyGame::ShaderZone::ShaderZone(MyGame *g, Shader *shader) : shader(shader), parent(g)
{
  shader->StartUseShader();
  // [TASK] Set parent->projection matrix here

  // [TASK] Set parent->my_cam->getViewMatrix() here

}

MyGame::ShaderZone::~ShaderZone()
{
  shader->StopUseShader();
}


// Singleton
std::unique_ptr<MyGame> my_game;
void init_my_game(){ my_game = std::make_unique<MyGame>(); }
void close_my_game(){ my_game.reset(); }
MyGame *get_my_game(){ return my_game.get(); }

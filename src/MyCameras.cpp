#include "MyCameras.h"

void FreeCamera::update(const GameState &gs)
{
  // We do camera movements if only cursor captured
  // We also check previous state to prevent "camera jump" when we back into app
  if (!gs.cursorCaptured || !gs.prev.cursorCaptured)
    return;

  double dyaw = gs.mouseX - gs.prev.mouseX;
  double dpitch = gs.mouseY - gs.prev.mouseY;
  const double sensitive = 0.0125;

  float3 pos = this->getPos();
  this->rotateYPR(float3(dyaw, dpitch, 0.0f) * sensitive);

  double deltaTime = gs.gameTime - gs.prev.gameTime;
  double speed = 1.0 + (gs.keyboard[GLFW_KEY_LEFT_SHIFT]-gs.keyboard[GLFW_KEY_LEFT_CONTROL])*.9f;

  int dfront = gs.keyboard[GLFW_KEY_W] - gs.keyboard[GLFW_KEY_S];
  int dright = gs.keyboard[GLFW_KEY_D] - gs.keyboard[GLFW_KEY_A];
  int dup    = gs.keyboard[GLFW_KEY_E] - gs.keyboard[GLFW_KEY_C];
  int dup_w  = gs.keyboard[GLFW_KEY_Q] - gs.keyboard[GLFW_KEY_Z];

  // Why we use viewMatrix rows?
  // https://www.3dgep.com/understanding-the-view-matrix/#FPS_Camera
  // Our view matrix is (TR)^-1 where T is translation and R is rotation.
  // View matrix is used to transform from world-space to view-space.
  // We need to calculate world-space deltaPos from view-space deltas.
  // So how to get it? Let's take world right vector, apply inv(view) matrix and get the result
  // Ok. (viewMatrix)^-1 * [1,0,0,0] = transpose(viewMatrix) * [1,0,0,0] ... so it is viewMatrix.row(0)
  // Why transpose when we need inverse? Because it is orthonormal!
  float3 deltaPos = to_float3(
                    dfront * -this->viewMatrix.get_row(2) +
                    dright * this->viewMatrix.get_row(0) +
                    dup    * this->viewMatrix.get_row(1)) +
                    dup_w  * this->world_up; // up in world coordinates
  // With translation part:
  // (R|T) inverse is (tr(R)|-T)
  // (0|1)            (    0|1 )

  // We need deltaTime for fps independence
  deltaPos *= speed * deltaTime;

  this->setPos(pos + deltaPos);
}

void TankCamera::update()
{
  // Arcball camera
  float4x4 view = mul(rotate_Y_4x4(-this->yaw_pitch_roll.x - M_PI_2), translate4x4(pos));
  view = mul(translate4x4(delta), view);
  // inverse of orthonormal matrix with additional translate column
  float4 col4 = view.get_col(3);
  view.set_col(3, float4(-col4.x,-col4.y,-col4.z,col4.w));
  this->viewMatrix = transpose3x3(view);
}

void TankCamera::update(const GameState &gs)
{
  if (!gs.cursorCaptured || !gs.prev.cursorCaptured)
    return;

  double dyaw = gs.mouseX - gs.prev.mouseX;
  if (dyaw == 0.0) return;
  const double sensitive = 0.0125;

  this->yaw_pitch_roll.x += dyaw*sensitive;
  update();
}
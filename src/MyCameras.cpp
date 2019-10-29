#include "MyCameras.h"

void FreeCamera::update(GameState gs)
{
  // We do camera movements if only cursor captured
  // We also check previous state to prevent "camera jump" when we back into app
  if (!gs.cursorCaptured || !gs.prev.cursorCaptured)
    return;

  double dyaw = gs.mouseX - gs.prev.mouseX;
  double dpitch = gs.mouseY - gs.prev.mouseY;
  const double sensitive = 0.0125;

  this->rotateYPR(float3(dyaw, dpitch, 0.0f) * sensitive);

  double deltaTime = gs.gameTime - gs.prev.gameTime;
  double speed = 1.0 + (gs.keyboard[GLFW_KEY_LEFT_SHIFT]-gs.keyboard[GLFW_KEY_LEFT_CONTROL])*.9f;

  int dfront = gs.keyboard[GLFW_KEY_W] - gs.keyboard[GLFW_KEY_S];
  int dright = gs.keyboard[GLFW_KEY_D] - gs.keyboard[GLFW_KEY_A];
  int dup    = gs.keyboard[GLFW_KEY_E] - gs.keyboard[GLFW_KEY_C];
  int dup_w  = gs.keyboard[GLFW_KEY_Q] - gs.keyboard[GLFW_KEY_Z];
  float4x4 viewMatrix = this->viewMatrix;

  // Why we use viewMatrix columns?
  // Let's find out. For example we want to get camera front vector.
  // So how to get it? Let's take world front vector, apply view matrix and get the result
  // Ok. viewMatrix * [1,0,0,0] ... so it is viewMatrix.col(0)
  float3 deltaPos = to_float3(
                    dfront * this->viewMatrix.get_col(0) +
                    dright * this->viewMatrix.get_col(1) +
                    dup    * this->viewMatrix.get_col(2)) +
                    dup_w  * this->world_up;

  // We need deltaTime for fps independence
  deltaPos *= speed * deltaTime;

  this->setPos(getPos() + deltaPos);
}
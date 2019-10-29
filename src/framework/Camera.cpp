#include "Camera.h"

Camera::Camera(float3 world_up, float3 world_right, float3 world_front)
{
  this->world_up = world_up;
  this->world_right = world_right;
  this->world_front = world_front;
}

Camera& GimbalCamera::rotateYPR(float yaw, float pitch, float roll)
{
  yaw_pitch_roll += float3(yaw, pitch, roll);

  float4 prevPos = viewMatrix.get_col(3);
  float4x4 viewMatrix = (mul(rotate_Y_4x4(-yaw_pitch_roll.x), rotate_X_4x4(yaw_pitch_roll.y)));
  viewMatrix = (mul(rotate_Z_4x4(yaw_pitch_roll.z), viewMatrix));
  viewMatrix.set_col(3, prevPos);

  this->viewMatrix = viewMatrix;
  return *this;
}
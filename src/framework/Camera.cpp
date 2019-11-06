#include "Camera.h"

Camera::Camera(float3 world_up, float3 world_right, float3 world_front)
{
  this->world_up = world_up;
  this->world_right = world_right;
  this->world_front = world_front;
}

float3 Camera::getPos() const {
  float3 local_coords = to_float3(viewMatrix.get_col(3));
  return -mul3x3(transpose(this->viewMatrix), local_coords);
}
Camera& Camera::setPos(float3 val) { // val in world coords
  float3 local_coords = -mul3x3((this->viewMatrix), val);
  this->viewMatrix.set_col(3, to_float4(local_coords, 1.0));
  return *this;
}

Camera& GimbalCamera::rotateYPR(float yaw, float pitch, float roll)
{
  yaw_pitch_roll += float3(yaw, pitch, roll);
  yaw_pitch_roll.y = clamp(yaw_pitch_roll.y, -M_PI_2, M_PI_2);

  float3 prev_pos = getPos();
  float4x4 view_matrix = (mul(rotate_Y_4x4(yaw_pitch_roll.x), rotate_X_4x4(yaw_pitch_roll.y)));
  view_matrix = (mul(rotate_Z_4x4(yaw_pitch_roll.z), view_matrix));
  view_matrix = transpose(view_matrix);

  this->viewMatrix = view_matrix;
  setPos(prev_pos);
  return *this;
}
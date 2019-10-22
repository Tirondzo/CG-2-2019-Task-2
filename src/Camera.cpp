#include "Camera.h"

Camera::Camera(float3 a_pos, float3 a_up, float3 a_front, GLfloat a_yaw, GLfloat a_pitch,
               GLfloat a_moveSpeed, GLfloat a_mouseSensitivity, GLfloat a_zoom) : pos(a_pos), front(a_front), up(a_up),
               yaw(a_yaw), pitch(a_pitch), moveSpeed(a_moveSpeed), mouseSensitivity(a_mouseSensitivity), zoom(a_zoom)
{
  right   = normalize(cross(a_up, a_front));
  worldUp = up;
}



float4x4 Camera::GetViewMatrix() const
{
  return lookAtTransposed(pos, pos + front, up);
}

void Camera::ProcessKeyboard(Movement_Direction dir, GLfloat deltaTime)
{
  GLfloat velocity = moveSpeed * deltaTime;
  if (dir == FORWARD)
    pos += front * velocity;
  if (dir == BACKWARD)
    pos -= front * velocity;
  if (dir == LEFT)
    pos -= right * velocity;
  if (dir == RIGHT)
    pos += right * velocity;
}

void Camera::ProcessMouseMove(GLfloat deltaX, GLfloat deltaY, GLboolean limitPitch)
{
  deltaX *= mouseSensitivity;
  deltaY *= mouseSensitivity;

  yaw += deltaX;
  pitch += deltaY;

  if (limitPitch)
  {
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
  }

  updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat deltaY)
{
  if (zoom >= 1.0f && zoom <= 45.0f)
    zoom -= deltaY;
  if (zoom <= 1.0f)
    zoom = 1.0f;
  if (zoom >= 45.0f)
    zoom = 45.0f;
}



void Camera::updateCameraVectors()
{
  float3 tmpFront;

  tmpFront.x = cos(DEG_TO_RAD*yaw) * cos(DEG_TO_RAD*pitch);
  tmpFront.y = sin(DEG_TO_RAD*pitch);
  tmpFront.z = sin(DEG_TO_RAD*yaw) * cos(DEG_TO_RAD*pitch);

  front = normalize(tmpFront);
  right = normalize(cross(front, worldUp));
  up    = normalize(cross(right, front));
}
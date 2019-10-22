#ifndef CAMERA_H
#define CAMERA_H

///////////////////////
// Camera class based on samples from learnopengl.com

#include "common.h"

#include "LiteMath.h"
using namespace LiteMath;

enum Movement_Direction
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

class Camera
{
public:

  float3 pos;
  float3 front;
  float3 up;
  float3 right;

  GLfloat yaw;
  GLfloat pitch;

  GLfloat moveSpeed;
  GLfloat mouseSensitivity;
  GLfloat zoom;

  Camera(float3 a_pos = float3(0.0f, 0.0f, 0.0f), float3 a_up = float3(0.0f, 1.0f, 0.0f),
         float3 a_front = float3(0.0f, 0.0f, -1.0f), GLfloat a_yaw = -90.0f, GLfloat a_pitch = 0.0f,
         GLfloat a_moveSpeed = 3.0f, GLfloat a_mouseSensitivity = 0.1f, GLfloat a_zoom = 45.0f);

  virtual ~Camera() {};

  float4x4 GetViewMatrix() const;

  void ProcessKeyboard(Movement_Direction dir, GLfloat deltaTime);
  void ProcessMouseMove(GLfloat deltaX, GLfloat deltaY, GLboolean limitPitch = true);
  void ProcessMouseScroll(GLfloat deltaY);

private:
  void updateCameraVectors();
  float3 worldUp;

};


#endif
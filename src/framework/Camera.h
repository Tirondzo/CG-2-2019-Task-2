#pragma once

#include "Game.h"

#include <LiteMath.h>
using namespace LiteMath;

// Camera Interface
class Camera
{
protected:
  float3 world_up, world_right, world_front;
  float4x4 viewMatrix;
public:
  Camera(float3 world_up = float3(0,1,0),
         float3 world_right = float3(1,0,0),
         float3 world_front = float3(0,0,1));

  float3 getPos() const { return to_float3(viewMatrix.get_col(3)); }
  Camera& setPos(float3 val) { viewMatrix.set_col(3, to_float4(val, viewMatrix.row[3].w)); return *this; }

  virtual Camera& rotateYPR(float yaw, float pitch, float roll) = 0;
  Camera& rotateYPR(float3 ypr) { return this->rotateYPR(ypr.x, ypr.y, ypr.z); }

  virtual float4x4 getViewMatrix() const = 0;
  virtual void update(const GameState &gs) {};
};

// Simpliest camera based on Yaw Pitch Roll angles
class GimbalCamera : public Camera{
protected:
  float3 yaw_pitch_roll;
public:
  using Camera::Camera;
  using Camera::rotateYPR;
  Camera& rotateYPR(float yaw, float pitch, float roll) override;
  float4x4 getViewMatrix() const override { return viewMatrix; };

  float3 getYPR() const { return yaw_pitch_roll; };
};

// Camera based on quaternions.
// It solves a lot of problems such as Gimbal Lock and animations.
// class QuaternionCamera : Camera {};

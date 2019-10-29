#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include "Game.h"

#include <LiteMath.h>
using namespace LiteMath;

// Camera Interface
class Camera
{
protected:
  float3 world_up, world_right, world_front;
  float3 pos;
  float4x4 viewMatrix;
public:
  Camera(float3 world_up, float3 world_right, float3 world_front);

  float3 getPos() const { return pos; }
  Camera& setPos(float3 val) { pos = val; return *this; }

  virtual Camera& rotateYPR(float yaw, float pitch, float roll) = 0;
  Camera& rotateYPR(float3 ypr) { return this->rotateYPR(ypr.x, ypr.y, ypr.z); }

  virtual float4x4 getViewMatrix() const = 0;
  virtual void update(GameState gs) {};
};

// Simpliest camera based on Yaw Pitch Roll angles
class GimbalCamera : Camera{
protected:
  float3 yaw_pitch_roll;
public:
  Camera& rotateYPR(float yaw, float pitch, float roll) override;
  float4x4 getViewMatrix() const override { return viewMatrix; };

  float3 getYPR() const { return yaw_pitch_roll; };
};

// Camera based on quaternions.
// It solves a lot of problems such as Gimbal Lock and animations.
// class QuaternionCamera : Camera {};

// Game cameras:

class FreeCamera : GimbalCamera {};
class TankCamera : GimbalCamera {};

#endif
#pragma once

#include "framework/Camera.h"

// Game cameras:

class FreeCamera : public GimbalCamera {
public:
    void update(const GameState &gs) override;
};
class TankCamera : public GimbalCamera {
    const float3 delta = float3(0, 1, 3);
    float3 pos;
    void update();
public:
    float3 getPos() const override { return pos; }
    Camera& setPos(float3 val) override { pos = val; update(); return *this; };
    void update(const GameState &gs) override;
};
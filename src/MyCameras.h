#pragma once

#include "framework/Camera.h"

// Game cameras:

class FreeCamera : public GimbalCamera {
public:
    void update(const GameState &gs) override;
};
class TankCamera : public GimbalCamera {};
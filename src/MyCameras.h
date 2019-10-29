#include "framework/Camera.h"

// Game cameras:

class FreeCamera : public GimbalCamera {
    void update(GameState gs) override;
};
class TankCamera : public GimbalCamera {};
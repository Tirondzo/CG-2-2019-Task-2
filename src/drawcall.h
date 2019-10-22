#pragma once

#include "ShaderProgram.h"
#include "Camera.h"
#include "mesh.h"

void DrawSimpleTriangle(ShaderProgram &debug_program,
                        Camera &camera,
                        uint32_t width,
                        uint32_t height);

void DrawMesh(ShaderProgram &program,
    Camera &camera,
    std::unique_ptr<Mesh> &mesh,
    uint32_t width,
    uint32_t height,
    float deltaTime);

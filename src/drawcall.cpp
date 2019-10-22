#include "drawcall.h"

void DrawSimpleTriangle(ShaderProgram &debug_program,
                        Camera &camera,
                        uint32_t width,
                        uint32_t height)
{
    debug_program.StartUseShader();

    DrawTriangle();

    debug_program.StopUseShader();
}

void DrawMesh(ShaderProgram &program,
              Camera &camera,
              std::unique_ptr<Mesh> &mesh,
              uint32_t width,
              uint32_t height,
              float deltaTime)
{
    program.StartUseShader();

    program.SetUniform("color_texture", 0);

    //if (mesh->GetName() == "Aircraft_propeller") {
    //}

    program.StopUseShader();
}
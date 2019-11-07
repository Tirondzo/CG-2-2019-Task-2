#version 330 core
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// [TASK] Add uniform mat4 projection here
// [TASK] Add view matrix here, same as projection

void main()
{
  // [TASK] Use projection matrix
  // [TASK] Use view matrix
  gl_Position = vec4(vertex, 1.0f);
}
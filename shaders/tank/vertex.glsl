#version 330 core
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;


out vec2 vTexCoords;
out vec3 vNormal;

uniform mat4 projection = mat4(1);
uniform mat4 view = mat4(1);
// [TASK] Add model matrix here

void main()
{
  gl_Position = vec4(vertex, 1.0f);

  // Pass texture coords next to fragment shader
  vTexCoords = texCoords;
  // [TASK] Use model matrix
  gl_Position = projection*view*gl_Position;
}
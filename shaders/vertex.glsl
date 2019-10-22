#version 330 core
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;


out vec2 vTexCoords;
out vec3 vNormal;

void main()
{
  gl_Position = vec4(vertex, 1.0f);

  // Передает текстурные координаты дальше в Fragment шейдер
  vTexCoords = texCoords;
}
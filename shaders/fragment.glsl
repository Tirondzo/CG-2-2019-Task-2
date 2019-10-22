#version 330 core
in vec2 vTexCoords;
in vec3 vNormal;

out vec4 color;

uniform sampler2D color_texture;

void main()
{
  color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
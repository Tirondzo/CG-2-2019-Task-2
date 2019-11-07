#version 330 core
in vec2 vTexCoords;
in vec3 vNormal;

out vec4 color;

uniform sampler2D diffuse_tex;

void main()
{
  vec3 c = vec3(1);
  // [TASK] add textures
  // [HELP] look at texture method, diffuse_tex and vTexCoords
  color = vec4(c, 1.0f);
}
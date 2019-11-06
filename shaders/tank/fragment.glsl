#version 330 core
in vec2 vTexCoords;
in vec3 vNormal;

out vec4 color;

uniform sampler2D diffuse_tex;

void main()
{
  vec4 c = texture(diffuse_tex, vTexCoords);
  color = vec4(c.rgb, 1.0f);
}
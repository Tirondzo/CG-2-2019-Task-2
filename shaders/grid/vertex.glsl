#version 330 core
layout(location = 0) in vec3 vertex;

out vec2 pos;

uniform mat4 projection = mat4(1);
uniform mat4 view = mat4(1);

void main()
{
  vec3 vpos = vertex * 100.0; // scale plane x100
  // pass vertex position to fragment shader
  pos = vpos.xz; // y is constant for plane

  gl_Position = vec4(vpos, 1.0f);
  gl_Position = projection*view*gl_Position;
}
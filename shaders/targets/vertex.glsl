#version 330 core
layout(location = 0) in vec3 vertex;

out vec3 pos;

uniform mat4 projection = mat4(1);
uniform mat4 view = mat4(1);

// https://thebookofshaders.com/10/
vec2 random2 (float s) {
    return fract(sin(vec2(s,s*3.1415+0.127) * vec2(12.9898,78.233))*43758.5453123);
}

void main()
{
  // pass vertex position to fragment shader, use them as tex. coords
  pos = vertex;

  vec3 vpos = vertex;
  vpos.y += 1.0; // move boxes up
  vpos.x *= 0.1; // scale by x
  vpos.yz *= 0.5; // make them smaller


  vpos.x += 12.0;
  vpos.xz += vec2(random2(gl_InstanceID)-0.5) * 10.0;

  gl_Position = vec4(vpos, 1.0f);
  gl_Position = projection*view*gl_Position;
}
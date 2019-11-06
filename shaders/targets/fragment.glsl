#version 330 core
in vec3 pos;

out vec4 color;

void main()
{
  vec3 c = vec3(1.0);
  float l = length(pos.yz)*4.0;
  float p = fract(l)-0.5;
  // color = vec4(vec3(p), 1.0); return; // uncomment and see what happens
  p = smoothstep(0.2, 0.25, abs(p));
  p *= step(l, 3.5);
  c = vec3(1.0, 1.0 - p, 1.0 - p);
  color = vec4(c, 1.0);
}
#version 330 core
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;


out vec2 vTexCoords;
out vec3 vFragPosition;
out vec3 vNormal;

uniform vec2 plane_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

float rand(vec2 n) { 
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(vec2 p){
    vec2 ip = floor(p);
    vec2 u = fract(p);
    u = u*u*(3.0-2.0*u);
    
    float res = mix(
        mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
        mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
    return res*res;
}

void main()
{
  vec3 position = vertex;
  position.y = noise((vertex.xz + plane_pos) / 250) * 40 + noise((vertex.xz + vec2(-123, 210) + plane_pos) / 20) * 10; 

  gl_Position = projection * view * model * vec4(position, 1.0f);

  vTexCoords = texCoords;
  vFragPosition = vec3(model * vec4(position, 1.0f));
  vNormal = mat3(transpose(inverse(model))) * normal;
}
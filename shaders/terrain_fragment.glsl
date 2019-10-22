#version 330 core
in vec3 vFragPosition;
in vec2 vTexCoords;
in vec3 vNormal;

out vec4 color;

void main()
{
    vec3 lightDir = normalize(vec3(1.0f, 1.0f, 0.0f)); 

    vec3 col = vec3(0.4f, 0.4f, 0.4f);

    float kd = max(dot(vNormal, lightDir), 0.0);
    col = kd * col;
    
    
    float dist = gl_FragCoord.z / gl_FragCoord.w;
    
    vec3 fog_color = vec3(0.1f, 0.6f, 0.8f);

    float fog_density = 0.003f;
    float fog_factor = 1.0 / exp(dist * fog_density);
    fog_factor = clamp( fog_factor, 0.0, 1.0 );

    col = mix(fog_color, col, fog_factor);
    
    color = vec4(col, 1.0f);
}
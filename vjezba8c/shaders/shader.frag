#version 330 core
out vec4 FragColor;

uniform mat3 materialProps;
uniform mat3 light;

in VS_OUT
{
 vec3 N;
 vec3 L;
 vec3 V;
} fs_in;

void main()
{
    vec3 N = normalize(fs_in.N);
    vec3 L = normalize(fs_in.L);
    vec3 V = normalize(fs_in.V);
    vec3 R = reflect(-L, N);

    vec4 color = vec4(1.0f, 0.30f, 0.30f, 0.0f);

    float coef = dot(N, L);
    float coef2 = dot(R, V);

    float Ir = (light[0][0] * materialProps[0][0]) + (light[1][0] * materialProps[1][0]) * coef + (light[2][0] * materialProps[2][0]) * coef2;
    float Ig = (light[0][1] * materialProps[0][1]) + (light[1][1] * materialProps[1][1]) * coef + (light[2][1] * materialProps[2][1]) * coef2;
    float Ib = (light[0][2] * materialProps[0][2]) + (light[1][2] * materialProps[1][0]) * coef + (light[2][2] * materialProps[2][2]) * coef2;

    FragColor = vec4(color.x * Ir, color.y * Ig, color.z * Ib, 1.0f);  
} 
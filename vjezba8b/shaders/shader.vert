#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;

uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;

uniform mat3 materialProps;
uniform mat3 light;

uniform vec3 lightPos;
uniform vec3 eye;

out fData 
{
    float Ir;
    float Ig;
    float Ib;
} params;

void main()
{
    vec4 P = matView * matModel * vec4(aPos, 1.0f);

    vec3 N = normalize(mat3(matView * matModel) * normal);
    vec3 L = normalize(lightPos - P.xyz);
    vec3 R = normalize(reflect(-L, N));
    vec3 V = normalize(eye - vec3(P));

    float coef = dot(N, L);
    if(coef < 0.0f)
        coef = 0.0f;
    float coef2 = pow(dot(R, V), 10.05);
    if(coef2 < 0.0f)
        coef2 = 0.0f;

    params.Ir = (light[0][0] * materialProps[0][0]) + (light[1][0] * materialProps[1][0]) * coef + (light[2][0] * materialProps[2][0]) * coef2;
    params.Ig = (light[0][1] * materialProps[0][1]) + (light[1][1] * materialProps[1][1]) * coef + (light[2][1] * materialProps[2][1]) * coef2;
    params.Ib = (light[0][2] * materialProps[0][2]) + (light[1][2] * materialProps[1][0]) * coef + (light[2][2] * materialProps[2][2]) * coef2;
    gl_Position = matProjection * P;
}
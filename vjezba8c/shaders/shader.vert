#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;

uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;

uniform vec4 lightPos;
uniform vec3 eyeView;

out VS_OUT
{
 vec3 N;
 vec3 L;
 vec3 V;
} vs_out;

void main()
{
    vec4 P = matModel * vec4(aPos, 1.0f);
    vs_out.N = vec3(matModel * vec4(normal, 1.0f));
    vs_out.L = vec3(lightPos) - P.xyz;
    vs_out.V = vec3(-P.xyz);

    gl_Position = matProjection * matView * matModel * vec4(aPos, 1.0);
}
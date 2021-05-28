#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCord;

uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;
uniform vec3 eyeView;

out VS_OUT
{
 vec3 N;
 vec3 eye;
 vec3 V;
} vs_out;
out vec2 UV;

void main()
{
    mat4 normalMatrix = transpose(inverse(matView* matModel));
    vec4 vx = normalize(matView * matModel * vec4(aPos, 1.0f));
    vs_out.V = vec3(vx.xyz / vx.w);
    vec4 eyex = normalize(matView * matModel * vec4(eyeView, 1.0f));
    vs_out.eye = vec3(eyex.xyz / eyex.w);

    vs_out.N = normalize(vec3(normalMatrix * vec4(normal, 1.0f)));
    gl_Position = matProjection * matView * matModel * vec4(aPos, 1.0);
    UV = texCord;
}
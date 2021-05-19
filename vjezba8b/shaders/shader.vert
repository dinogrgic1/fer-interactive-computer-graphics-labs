#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;

uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;

uniform mat3 materialProps;
uniform mat3 light;

uniform vec4 lightPos;
uniform vec3 eyeView;

out fData 
{
    float Ir;
    float Ig;
    float Ib;
} params;

void main()
{
    vec4 color = vec4(1.0f, 0.30f, 0.30f, 0.0f);
    
    vec4 toSource = vec4(lightPos.x - normal.x, lightPos.y - normal.y, lightPos.z - normal.z, 1.0f);
    vec4 toSourceReflect = reflect(toSource, vec4(normal, 1.0f));

    float coef = dot(toSource, vec4(normal, 1.0f));
    float coef2 = dot(toSourceReflect, vec4(eyeView, 1.0f));

    params.Ir = (light[0][0] * materialProps[0][0]) + (light[1][0] * materialProps[1][0]) * coef + (light[2][0] * materialProps[2][0]) * coef2;
    params.Ig = (light[0][1] * materialProps[0][1]) + (light[1][1] * materialProps[1][1]) * coef + (light[2][1] * materialProps[2][1]) * coef2;
    float Ib = (light[0][2] * materialProps[0][2]) + (light[1][2] * materialProps[1][0]) * coef + (light[2][2] * materialProps[2][2]) * coef2;
    
    gl_Position = matProjection * matView* matModel * vec4(aPos, 1.0);
}
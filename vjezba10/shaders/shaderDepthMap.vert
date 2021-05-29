#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 matProjection;
uniform mat4 matView;
uniform mat4 matModel;

void main()
{
    gl_Position =  matProjection * matView * matModel  * vec4(aPos, 1.0);
}
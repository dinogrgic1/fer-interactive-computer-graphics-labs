#version 330 core
out vec4 FragColor;

in fData
{
    vec3 normal;
    vec4 color;
} frag;

void main()
{
    FragColor = frag.color;  
} 
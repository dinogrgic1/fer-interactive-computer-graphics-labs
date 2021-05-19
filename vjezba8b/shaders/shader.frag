#version 330 core
out vec4 FragColor;
in fData 
{
    float Ir;
    float Ig;
    float Ib;
} params;

void main()
{
    vec3 color = vec3(1.0f, 0.0f, 0.0f);
    FragColor = vec4(color.x * params.Ir, color.y * params.Ig, color.z * params.Ib, 1.0f);  
} 
#version 330 core
out vec4 FragColor;

uniform mat3 materialProps;
uniform mat3 light;

uniform vec3 lightPos;
uniform sampler2D texImage;

in vec2 UV;
in VS_OUT
{
    vec3 N;
    vec3 eye;
    vec3 V;
} fs_in;

void main()
{
    vec3 color = texture(texImage, UV).rgb;
    vec3 N = normalize(fs_in.N);
    vec3 L = normalize(lightPos - fs_in.V);
    vec3 V = normalize(fs_in.eye - fs_in.V);
    vec3 R = normalize(reflect(-L, N));

    float coef = dot(N, L);
    if(coef < 0.0f) {
        coef = 0.0f;
    }
    float coef2 = pow(dot(R, V), 10.05f);
    if(coef2 < 0.0f) {
        coef2 = 0.0f;
    }

    float Ir = (light[0][0] * materialProps[0][0]) + (light[1][0] * materialProps[1][0]) * coef + (light[2][0] * materialProps[2][0]) * coef2;
    float Ig = (light[0][1] * materialProps[0][1]) + (light[1][1] * materialProps[1][1]) * coef + (light[2][1] * materialProps[2][1]) * coef2;
    float Ib = (light[0][2] * materialProps[0][2]) + (light[1][2] * materialProps[1][0]) * coef + (light[2][2] * materialProps[2][2]) * coef2;

    FragColor = vec4(color.x * Ir, color.y * Ig, color.z * Ib, 0.0f);
}
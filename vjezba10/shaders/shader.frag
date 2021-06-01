#version 330 core
out vec4 FragColor;

uniform mat3 materialProps;
uniform mat3 light;
uniform vec3 lightPos;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

in vec2 UV;
in vec4 V_1;
in VS_OUT
{
    vec3 N;
    vec3 eye;
    vec3 V;
} fs_in;

void main()
{
    vec3 V_1_normlized = V_1.xyz / V_1.w;
    V_1_normlized = V_1_normlized * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, V_1_normlized.xy).r;
    float currentDepth = V_1_normlized.z;

    vec3 L = normalize(lightPos - fs_in.V);
    vec3 N = normalize(fs_in.N);

    float shadow = 1.0f - pow(length(V_1.xy / V_1.w), 2);
    if(shadow < 0.0f) {
        shadow = 0.0f;
    }

//    if(currentDepth > closestDepth) {
//        shadow = 0.0f;
//    }

    //float bias = 0.005 * tan(acos(dot(N, L)));
    float bias = max(0.05 * (1.0 - dot(N, L)), 0.005);

    if (currentDepth - bias > closestDepth){
        //shadow = 0.0;

        vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(shadowMap, V_1_normlized.xy + vec2(x, y) * texelSize).r;
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
        shadow /= 9.0;
        shadow = 1.0 - shadow;
    }

    vec3 color = texture(diffuseTexture, UV).rgb;
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

    float Ir = (light[0][0] * materialProps[0][0]) + shadow * ((light[1][0] * materialProps[1][0]) * coef + (light[2][0] * materialProps[2][0]) * coef2);
    float Ig = (light[0][1] * materialProps[0][1]) + shadow * ((light[1][1] * materialProps[1][1]) * coef + (light[2][1] * materialProps[2][1]) * coef2);
    float Ib = (light[0][2] * materialProps[0][2]) + shadow * ((light[1][2] * materialProps[1][0]) * coef + (light[2][2] * materialProps[2][2]) * coef2);

    FragColor = vec4(color.x * Ir, color.y * Ig, color.z * Ib, 0.0f);
}
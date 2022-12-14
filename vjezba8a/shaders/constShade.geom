#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 matProjection;
uniform mat4 matView;

uniform vec3 eye;
uniform vec3 lightPos;

uniform mat3 materialProps;
uniform mat3 light;

out fData 
{
    vec3 normal;
    vec4 color;
} frag;

void main() {
    vec4 objColor = vec4(0.50f, 0.19f, 0.0f, 0.0);
    vec3 v_x = vec3(gl_in[0].gl_Position.xyz);
    vec3 v_x1 = vec3(gl_in[1].gl_Position.xyz);
    vec3 v_x2 = vec3(gl_in[2].gl_Position.xyz);

    vec3 c = (v_x + v_x1 + v_x2) / 3;

    vec3 N = normalize(cross((v_x1 - v_x), (v_x2 - v_x)));
    vec3 L = normalize(lightPos - c);
    vec3 R = normalize(reflect(L, N));
    vec3 V = normalize(eye - c);

    float coef = dot(L, N);
    if(coef < 0.0f) {
        coef = 0.0f;
    }

    float coef2 = pow(dot(R, V), 10.5f);
    if(coef2 < 0.0f) {
        return;
    }

    float Ir = (light[0][0] * materialProps[0][0]) + (light[1][0] * materialProps[1][0]) * coef + (light[2][0] * materialProps[2][0]) * coef2;
    float Ig = (light[0][1] * materialProps[0][1]) + (light[1][1] * materialProps[1][1]) * coef + (light[2][1] * materialProps[2][1]) * coef2;
    float Ib = (light[0][2] * materialProps[0][2]) + (light[1][2] * materialProps[1][0]) * coef + (light[2][2] * materialProps[2][2]) * coef2;

    frag.color = vec4(objColor.x * Ir, objColor.y * Ig, objColor.z * Ib, 0.0f);
    gl_Position = matProjection * matView * vec4(v_x, 1.0f);
    EmitVertex();
    
    frag.color = vec4(objColor.x * Ir, objColor.y * Ig, objColor.z * Ib, 0.0f);
    gl_Position = matProjection * matView * vec4(v_x1, 1.0f);
    EmitVertex();
    
    frag.color = vec4(objColor.x * Ir, objColor.y * Ig, objColor.z * Ib, 0.0f);
    gl_Position = matProjection * matView * vec4(v_x2, 1.0f);
    EmitVertex();
    EndPrimitive();
}
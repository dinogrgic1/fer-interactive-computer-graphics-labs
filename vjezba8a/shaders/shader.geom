#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 matProjection;
uniform mat4 matView;
uniform vec3 eyeView;
uniform vec4 lightPos;

uniform mat3 materialProps;
uniform mat3 light;

out fData 
{
    vec3 normal;
    vec4 color;
} frag;

void main() {
    vec3 v_x = vec3(gl_in[0].gl_Position.xyz);
    vec3 v_x1 = vec3(gl_in[1].gl_Position.xyz);
    vec3 v_x2 = vec3(gl_in[2].gl_Position.xyz);

    vec3 c = (v_x + v_x1 + v_x2) / 3;
    
    vec4 toSource = vec4(lightPos.x - c.x, lightPos.y - c.y, lightPos.z - c.z, 1.0f);
    vec3 normal = normalize(cross((v_x1 - v_x), (v_x2 - v_x)));

    vec4 toSourceReflect = reflect(toSource, vec4(normal, 1.0f));
    vec4 objColor = vec4(0.0f, 0.39f, 0.0f, 0.0);

    float coef = dot(toSource, vec4(normal, 1.0f));
    float coef2 = dot(toSourceReflect, vec4(eyeView, 1.0f));

    float Ir = (light[0][0] * materialProps[0][0]) + (light[1][0] * materialProps[1][0]) * coef + (light[2][0] * materialProps[2][0]) * pow(coef2, 2.0f);
    float Ig = (light[0][1] * materialProps[0][1]) + (light[1][1] * materialProps[1][1]) * coef + (light[2][1] * materialProps[2][1]) * pow(coef2, 2.0f);
    float Ib = (light[0][2] * materialProps[0][2]) + (light[1][2] * materialProps[1][0]) * coef + (light[2][2] * materialProps[2][2]) * pow(coef2, 2.0f);

    frag.color = vec4(objColor.x + 0.1f * Ir, objColor.y * Ig, objColor.z * Ib, 0.0f);
    frag.normal = normal;
    gl_Position = matProjection * matView * vec4(v_x, 1.0f);
    EmitVertex();
    
    frag.color = vec4(objColor.x * Ir, objColor.y * Ig, objColor.z * Ib, 0.0f);
    frag.normal = normal;
    gl_Position = matProjection * matView * vec4(v_x1, 1.0f);
    EmitVertex();
    
    frag.color = vec4(objColor.x * Ir, objColor.y * Ig, objColor.z * Ib, 0.0f);
    frag.normal = normal;
    gl_Position = matProjection * matView * vec4(v_x2, 1.0f);
    EmitVertex();
    EndPrimitive();
}